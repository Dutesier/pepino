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

#pragma once

#include "BaseExpression.h"
#include "BaseStatement.h"
#include "Token.h"

#include <vector>

namespace pep
{

using ExpressionUPTR = std::unique_ptr<Expression>;
using StatementUPTR = std::unique_ptr<Statement>;

class Parser
{
public:
    Parser(std::vector<Token> tokens)
        : m_tokens(std::move(tokens))
    {
    }

    // Top-level parse entry point:
    // Parse a single FeatureStatement, or a list of statements (features).
    StatementUPTR parse()
    {
        // In Gherkin, typically one "Feature" per file
        return parseFeatureStatement();
    }

    class ParserException;

private:
    unsigned int m_current = 0;
    std::vector<Token> m_tokens;

    // --- Gherkin-specific parse methods ---
    StatementUPTR parseFeatureStatement();
    StatementUPTR parseBackgroundStatement();
    StatementUPTR parseScenarioStatement();
    StatementUPTR parseScenarioOutlineStatement();
    StatementUPTR parseExamplesStatement();
    StatementUPTR parseStepStatement();

    // Utility methods
    Token peek();
    Token previous();
    bool isAtEnd();
    bool match(std::initializer_list<TokenType> types);
    bool check(TokenType type);
    Token advance();

    Token consumeOrThrow(TokenType type, const std::string& msg);
    void synchronize();

    ParserException error(const Token& token, const std::string& msg);

public:
    class ParserException : public std::runtime_error
    {
    public:
        ParserException(const std::string& msg)
            : std::runtime_error(msg)
        {
        }
    };
};

} // namespace pep
