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

const Token& Parser::consume(TokenType type, const std::string& message)
{
    Logger::debug("Consuming token: " + peek().lexeme + " of type: " + tokenAsString(peek()));
    if (peek().type == type)
    {
        Logger::debug("Token type matches: " + tokenAsString(peek()) + "-" + tokenAsString(Token{.type = type}));
        return advance();
    }
    Logger::error("Expected token of type: " + tokenAsString(Token{.type = type}) + ", got: " + tokenAsString(peek()));
    throw std::runtime_error(message + " at line " + std::to_string(peek().line));
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
    feature->tags = parseTags();

    consume(TokenType::Feature, "Expected 'Feature' keyword");
    if (previous().lexeme != "Feature")
    {
        throw std::runtime_error("Expected 'Feature' keyword, got " + previous().lexeme);
    }
    consume(TokenType::Colon, "Expected ':' after 'Feature'");
    feature->name = consume(TokenType::StringLiteral, "Expected feature name").lexeme;

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
            feature->scenarios.push_back(parseScenarioStatement());
        }
        else if (match(TokenType::ScenarioOutline))
        {
            feature->scenarioOutlines.push_back(parseScenarioOutlineStatement());
        }
        else if (match(TokenType::Tag))
        {
            nextTags = parseTags();
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
    scenario->tags = parseTags();
    consume(TokenType::Colon, "Expected ':' after 'Scenario'");
    scenario->name = consume(TokenType::StringLiteral, "Expected scenario name").lexeme;
    while (!isAtEnd() && isStep(peek().type))
    {
        scenario->steps.push_back(parseStepStatement());
    }
    return scenario;
}

std::unique_ptr<ScenarioOutlineStatement> Parser::parseScenarioOutlineStatement()
{
    auto outline = std::make_unique<ScenarioOutlineStatement>();
    outline->tags = parseTags();
    consume(TokenType::Colon, "Expected ':' after 'ScenarioOutline'");
    outline->name = consume(TokenType::StringLiteral, "Expected scenario outline name").lexeme;
    while (!isAtEnd() && isStep(peek().type))
    {
        outline->steps.push_back(parseStepStatement());
    }

    if (match(TokenType::StringLiteral) && previous().lexeme == "Examples")
    {
        outline->examples = parseExamplesStatement();
    }
    return outline;
}

std::unique_ptr<ExamplesStatement> Parser::parseExamplesStatement()
{
    auto examples = std::make_unique<ExamplesStatement>();
    consume(TokenType::Colon, "Expected ':' after 'Examples'");
    // Simplified: Assuming headers are a single string literal token
    examples->headers.push_back(consume(TokenType::StringLiteral, "Expected header").lexeme);
    // TODO: Parsing of table rows.
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
    step->text = consume(TokenType::StringLiteral, "Expected step text").lexeme;
    return step;
}

} // namespace pep