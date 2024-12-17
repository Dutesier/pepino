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
 ******************************************************************************/

#include "Parser.h"

#include "Logger.h"
#include "Token.h"
#include <memory>
#include <optional>
#include <vector>

namespace pep
{

Parser::Parser(std::vector<Token> tokens)
    : m_tokens(std::move(tokens))
{
}

// Implementation
// -----------------------------------------------------------------------------

StatementUPTR Parser::parseFeatureStatement()
{
    // Expect first token to be 'Feature'
    if (!match({ TokenType::Feature }))
    {
        throw ParserException("Expected 'Feature' keyword at start.");
    }
    consumeOrThrow(TokenType::Colon, "Expected ':' after 'Feature'");
    Token featureNameToken = advance();
    if (featureNameToken.type != TokenType::STRING_LITERAL)
    {
        throw ParserException("Expected feature name after 'Feature:'");
    }

    auto featureStmt = std::make_unique<FeatureStatement>();
    featureStmt->name = featureNameToken.lexeme;

    // Now parse optional background or scenario statements
    while (!isAtEnd())
    {
        if (match({ TokenType::Background }))
        {
            auto background = parseBackgroundStatement();
            featureStmt->children.push_back(std::move(background));
        }
        else if (match({ TokenType::Scenario }))
        {
            auto scenario = parseScenarioStatement();
            featureStmt->children.push_back(std::move(scenario));
        }
        else if (match({ TokenType::ScenarioOutline }))
        {
            auto outline = parseScenarioOutlineStatement();
            featureStmt->children.push_back(std::move(outline));
        }
        else
        {
            // If none matched, we might break or skip unrecognized tokens
            break;
        }
    }

    return featureStmt;
}

StatementUPTR Parser::parseBackgroundStatement()
{
    // We already consumed the Background token
    consumeOrThrow(TokenType::COLON, "Expected ':' after 'Background'");
    auto backgroundStmt = std::make_unique<BackgroundStatement>();

    // Parse step statements until we see a token that suggests we're done
    while (!isAtEnd())
    {
        // If next token is Scenario or ScenarioOutline or EOF, we stop
        if (check(TokenType::Scenario) || check(TokenType::ScenarioOutline) || isAtEnd())
        {
            break;
        }
        // Otherwise parse a step
        backgroundStmt->steps.push_back(std::unique_ptr<Statement>(parseStepStatement().release()));
    }

    return backgroundStmt;
}

StatementUPTR Parser::parseScenarioStatement()
{
    // We already consumed Scenario token
    consumeOrThrow(TokenType::COLON, "Expected ':' after 'Scenario'");
    Token scenarioNameToken = advance();
    if (scenarioNameToken.type != TokenType::STRING_LITERAL)
    {
        throw ParserException("Expected scenario name after 'Scenario:'");
    }

    auto scenarioStmt = std::make_unique<ScenarioStatement>();
    scenarioStmt->name = scenarioNameToken.lexeme;

    // Parse step statements
    while (!isAtEnd())
    {
        if (check(TokenType::Scenario) || check(TokenType::ScenarioOutline) || check(TokenType::Feature) ||
            check(TokenType::Eof))
        {
            break;
        }
        scenarioStmt->steps.push_back(std::unique_ptr<Statement>(parseStepStatement().release()));
    }

    return scenarioStmt;
}

StatementUPTR Parser::parseScenarioOutlineStatement()
{
    consumeOrThrow(TokenType::COLON, "Expected ':' after 'Scenario Outline'");
    Token outlineNameToken = advance();
    if (outlineNameToken.type != TokenType::STRING_LITERAL)
    {
        throw ParserException("Expected outline name after 'Scenario Outline:'");
    }

    auto outlineStmt = std::make_unique<ScenarioOutlineStatement>();
    outlineStmt->name = outlineNameToken.lexeme;

    // Parse the steps until we see 'Examples' or scenario boundary tokens
    while (!isAtEnd())
    {
        if (check(TokenType::Examples) || check(TokenType::Scenario) || check(TokenType::ScenarioOutline) ||
            check(TokenType::Feature) || check(TokenType::Eof))
        {
            break;
        }
        outlineStmt->steps.push_back(std::unique_ptr<Statement>(parseStepStatement().release()));
    }

    // If we see 'Examples', parse an ExamplesStatement
    if (match({ TokenType::Examples }))
    {
        outlineStmt->examples = parseExamplesStatement();
    }

    return outlineStmt;
}

StatementUPTR Parser::parseExamplesStatement()
{
    consumeOrThrow(TokenType::COLON, "Expected ':' after 'Examples'");
    auto examplesStmt = std::make_unique<ExamplesStatement>();
    // Usually, the next lines might be a table:
    // Examples:
    //   | x | y | result |
    //   | 42| 24| 66     |

    while (!isAtEnd())
    {
        if (check(TokenType::Scenario) || check(TokenType::ScenarioOutline) || check(TokenType::Feature) ||
            check(TokenType::Eof))
        {
            break;
        }

        // parse a row:
        // match TokenType::Pipe, then read until the next EOL or second Pipe
        // For demonstration, let's assume each row is on its own line
        std::vector<std::string> rowValues;

        // this is a naive approach, you’d likely loop until you see EOL or isAtEnd()
        while (!check(TokenType::END_OF_LINE) && !isAtEnd())
        {
            if (match({ TokenType::Pipe }))
            {
                continue;
            }
            if (peek().type == TokenType::STRING_LITERAL)
            {
                rowValues.push_back(advance().lexeme);
            }
            else
            {
                break; // or skip unknown tokens
            }
        }
        // consume EOL
        match({ TokenType::END_OF_LINE });

        // If it's the first row we parse, treat it as headers
        // If we've already got headers, treat subsequent rows as data
        if (examplesStmt->headers.empty())
        {
            examplesStmt->headers = rowValues;
        }
        else
        {
            examplesStmt->rows.push_back(rowValues);
        }
    }

    return examplesStmt;
}

StatementUPTR Parser::parseStepStatement()
{
    // Gherkin step keywords: GIVEN, WHEN, THEN, AND, BUT
    Token keywordToken = advance();
    std::string keyword;
    switch (keywordToken.type)
    {
    case TokenType::Given:
        keyword = "Given";
        break;
    case TokenType::When:
        keyword = "When";
        break;
    case TokenType::Then:
        keyword = "Then";
        break;
    case TokenType::And:
        keyword = "And";
        break;
    // case TokenType::BUT:
    //     keyword = "But";
    //     break;
    default:
        throw ParserException("Expected step keyword (Given/When/Then/And/But?).");
    }

    // The rest of the line is the step text
    std::string stepText;
    while (!check(TokenType::END_OF_LINE) && !isAtEnd())
    {
        stepText += advance().lexeme + " ";
    }
    // consume EOL
    match({ TokenType::END_OF_LINE });

    auto stepStmt = std::make_unique<StepStatement>();
    stepStmt->keyword = keyword;
    stepStmt->text = stepText;
    return stepStmt;
}

//
// Utility methods
//
Token Parser::peek()
{
    return m_tokens[m_current];
}

Token Parser::previous()
{
    return m_tokens[m_current - 1];
}

bool Parser::isAtEnd()
{
    return peek().type == TokenType::Eof;
}

bool Parser::check(TokenType type)
{
    if (isAtEnd())
        return false;
    return peek().type == type;
}

bool Parser::match(std::initializer_list<TokenType> types)
{
    for (auto t : types)
    {
        if (check(t))
        {
            advance();
            return true;
        }
    }
    return false;
}

Token Parser::advance()
{
    if (!isAtEnd())
        m_current++;
    return previous();
}

Token Parser::consumeOrThrow(TokenType type, const std::string& error_msg)
{
    if (check(type))
        return advance();
    throw ParserException(error_msg + " Found token: " + peek().lexeme);
}

void Parser::synchronize()
{
    // Not always crucial for Gherkin. In typical usage, you might skip tokens
    // until you see a well-known marker like `Scenario`, `Feature`, etc.
    advance();

    while (!isAtEnd())
    {
        if (previous().type == TokenType::END_OF_LINE)
            return;

        switch (peek().type)
        {
        case TokenType::Feature:
        case TokenType::Scenario:
        case TokenType::ScenarioOutline:
        case TokenType::Background:
            return;
        default:
            break;
        }

        advance();
    }
}

Parser::ParserException Parser::error(const Token& token, const std::string& msg)
{
    return ParserException(msg + " at token: " + token.lexeme);
}

} // namespace pep