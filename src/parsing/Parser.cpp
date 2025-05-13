/******************************************************************************
 * Project:  Pepino
 * Brief:    A C++ Cucumber interpreter.
 *
 * This software is provided "as is," without warranty of any kind, express
 * or implied, including but not limited to the warranties of merchantability,
 * fitness for a particular purpose, and noninfringement. In no event shall
 * the authors or copyright holders be liable for any claim, damages, or
 * other liability, whether in an action of contract, tort, or otherwise,
 * arising from, out of, or in connection with the software or the use or
 * other dealings in the software.
 *
 * Author:   Dutesier
 *
 *******************************************************************************/

#include "Parser.h"
#include "../Logger.h"
#include "Token.h"

#include <memory>
#include <numeric>
#include <vector>

namespace pep
{

Parser::Parser(const std::vector<Token>& tokens) : m_tokens(tokens)
{
}

bool Parser::isAtEnd() const
{
    return peek().type == TokenType::EndOfFile;
}

const Token& Parser::peek() const
{
    return m_tokens[m_current];
}

const Token& Parser::previous() const
{
    return m_tokens[m_current - 1];
}

const Token& Parser::advance()
{
    if (!isAtEnd())
        m_current++;
    return previous();
}

bool Parser::match(TokenType type)
{
    if (peek().type != type)
        return false;
    advance();
    return true;
}

void Parser::advanceEmptyLines()
{
    while (peek().type == TokenType::EOL)
    {
        advance();
    }
}

std::string Parser::consumeLiteralUntilEOL()
{
    std::vector<Token> text;

    while (!isAtEnd() && peek().type != TokenType::EOL)
    {
        text.push_back(advance());
    }
    std::string literal = std::accumulate(
        std::next(text.begin()), text.end(), text.front().lexeme,
        [](const std::string& a, const Token& b) {
            if (b.type != TokenType::StringLiteral)
            {
                throw std::runtime_error("Expected StringLiteral, got: " +
                                         tokenAsString(b));
            }
            return a + " " + b.lexeme;
        });
    return literal;
}

const Token& Parser::consume(TokenType type, const std::string& message)
{
    if (peek().type == type)
    {
        return advance();
    }
    Logger::error(
        "Expected token of type: " + tokenAsString(Token{.type = type, .lexeme = {}, .line = {}}) +
        ", got: " + tokenAsString(peek()));
    throw std::runtime_error(message + " at line " +
                             std::to_string(peek().line));
}

std::vector<std::string> Parser::parseTags()
{
    std::vector<std::string> tags;
    while (match(TokenType::Tag))
    {
        tags.push_back(previous().lexeme);
    }
    return tags;
}

std::unique_ptr<FeatureStatement> Parser::parseFeature()
{
    auto feature = std::make_unique<FeatureStatement>();
    advanceEmptyLines();
    feature->tags = parseTags();
    advanceEmptyLines();

    consume(TokenType::Feature, "Expected 'Feature' keyword");
    if (previous().lexeme != "Feature")
    {
        throw std::runtime_error("Expected 'Feature' keyword, got " +
                                 previous().lexeme);
    }
    consume(TokenType::Colon, "Expected ':' after 'Feature'");
    feature->name = consumeLiteralUntilEOL();

    // Ignore description lines.
    Logger::debug("Advancing empty lines after feature name");
    advanceEmptyLines();
    while (!isAtEnd() && peek().type == TokenType::StringLiteral)
    {
        auto tok = advance();
        advanceEmptyLines();
        Logger::debug("Ignoring description line: " + tok.lexeme);
    }
    Logger::debug("Finished parsing feature name and description");

    // Parse children: background, scenarios, and scenario outlines.
    std::vector<std::string> nextTags;
    while (!isAtEnd())
    {
        if (match(TokenType::Background))
        {
            feature->background = parseBackgroundStatement();
        }
        else if (match(TokenType::Scenario))
        {
            auto scenario = parseScenarioStatement();
            if (!nextTags.empty())
            {
                scenario->tags = nextTags;
                nextTags.clear();
            }
            feature->scenarios.push_back(std::move(scenario));
        }
        else if (match(TokenType::ScenarioOutline))
        {
            auto scenarioOutlines = parseScenarioOutlineStatement();
            if (!nextTags.empty())
            {
                scenarioOutlines->tags = nextTags;
                nextTags.clear();
            }
            feature->scenarioOutlines.push_back(std::move(scenarioOutlines));
        }
        else if (peek().type == TokenType::Tag)
        {
            // If we encounter a tag, we can assume it's for the next scenario
            // or outline.
            nextTags = parseTags();
            Logger::debug("Storing tag: ");
            for (auto& tag : nextTags)
            {
                Logger::debug(tag);
            }
        }
        else
        {
            // Skip unknown tokens - TODO throw here.
            Logger::warn("Skipping unknown token: " + peek().lexeme);
            advance();
        }
    }
    return feature;
}

std::unique_ptr<BackgroundStatement> Parser::parseBackgroundStatement()
{
    auto background = std::make_unique<BackgroundStatement>();
    consume(TokenType::Colon, "Expected ':' after 'Background'");
    // Parse steps until a new keyword or EOF.
    advanceEmptyLines();
    while (!isAtEnd() && isStep(peek().type))
    {
        background->steps.push_back(parseStepStatement());
    }
    return background;
}

std::unique_ptr<ScenarioStatement> Parser::parseScenarioStatement()
{
    auto scenario = std::make_unique<ScenarioStatement>();
    // A scenario might have preceding tags.
    // scenario->tags = parseTags();
    consume(TokenType::Colon, "Expected ':' after 'Scenario'");
    scenario->name = consumeLiteralUntilEOL();
    advanceEmptyLines();
    while (!isAtEnd() && isStep(peek().type))
    {
        scenario->steps.push_back(parseStepStatement());
    }
    return scenario;
}

std::unique_ptr<ScenarioOutlineStatement> Parser::
    parseScenarioOutlineStatement()
{
    auto outline = std::make_unique<ScenarioOutlineStatement>();
    // outline->tags = parseTags();
    consume(TokenType::Colon, "Expected ':' after 'ScenarioOutline'");
    outline->name = consumeLiteralUntilEOL();
    advanceEmptyLines();
    while (!isAtEnd() && isStep(peek().type))
    {
        outline->steps.push_back(parseStepStatement());
    }

    if (match(TokenType::Examples))
    {
        outline->examples = parseExamplesStatement();
    }
    return outline;
}

std::vector<std::string> Parser::parseTableRow()
{
    std::vector<std::string> row;
    // Each row must start with a Pipe.
    consume(TokenType::Pipe, "Expected '|' at start of examples table row");

    while (!isAtEnd() && peek().type != TokenType::EOL)
    {
        if (peek().type == TokenType::Pipe)
        {
            Logger::debug("Empty cell found");
            advance();
            row.push_back("");
            continue;
        }
        if (peek().type == TokenType::StringLiteral)
        {
            Logger::debug("Cell found: " + peek().lexeme);
            row.push_back(advance().lexeme);
        }
        else
        {
            Logger::warn("Unexpected token in table row: " + peek().lexeme);
            break;
        }
        if (peek().type == TokenType::Pipe)
        {
            advance(); // consume the pipe
        }
        else
        {
            // If we don't find a pipe, we should break out of the loop.
            Logger::warn("Expected '|' after cell value, found: " +
                         peek().lexeme);
            break;
        }
    }
    match(TokenType::EOL);
    return row;
}

// Parses an Examples statement.
// Format (after consuming the "Examples" keyword and colon):
//   [Optional EOL tokens]
//   | header1 | header2 | ... |
//   | value1  | value2  | ... |
//   | value3  | value4  | ... |
// Returns an ExamplesStatement AST node.
std::unique_ptr<ExamplesStatement> Parser::parseExamplesStatement()
{
    auto examples = std::make_unique<ExamplesStatement>();

    consume(TokenType::Colon, "Expected ':' after 'Examples'");

    // Optionally consume any EOL tokens immediately after the "Examples:"
    // header.
    while (match(TokenType::EOL))
    {
    }

    // There must be at least one row (the header row).
    if (peek().type != TokenType::Pipe)
    {
        throw std::runtime_error(
            "Expected examples table row starting with '|'");
    }

    // Parse header row.
    examples->headers = parseTableRow();
    const auto rowSize = examples->headers.size();

    // Parse additional rows.
    while (!isAtEnd() && peek().type == TokenType::Pipe)
    {
        auto row = parseTableRow();
        if (row.size() != rowSize)
        {
            throw std::runtime_error("Row size mismatch in examples table");
        }
        // Only add non-empty rows.
        if (!row.empty())
        {
            examples->rows.push_back(row);
        }
    }
    advanceEmptyLines();
    return examples;
}

std::unique_ptr<StepStatement> Parser::parseStepStatement()
{
    auto step = std::make_unique<StepStatement>();
    if (!isStep(peek().type))
    {
        throw std::runtime_error("Expected a step keyword");
    }
    step->keyword = advance().lexeme;

    // Consume the text in this  line. Check for <variable> placeholders.
    while (!isAtEnd() && peek().type != TokenType::EOL)
    {
        if (peek().type == TokenType::StringLiteral)
        {
            step->text.push_back(advance());
        }
        else if (peek().type == TokenType::LeftAngle)
        {
            advance();
            auto var =
                consume(TokenType::StringLiteral, "Expected variable name")
                    .lexeme;
            consume(TokenType::RightAngle, "Expected '>' after variable name");
            step->text.push_back({TokenType::Placeholder, var, peek().line});
            step->variables[var] = ""; // Store the variable name.
        }
        else
        {
            Logger::warn("Unexpected token in step text: " + peek().lexeme);
            break;
        }
    }
    advanceEmptyLines();

    return step;
}

} // namespace pep
