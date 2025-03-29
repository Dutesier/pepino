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

#include "Token.h"

namespace pep
{

std::string tokenAsString(const Token& token)
{
    switch (token.type)
    {
    case TokenType::Background:
        return "Background";
    case TokenType::Feature:
        return "Feature";
    case TokenType::Scenario:
        return "Scenario";
    case TokenType::ScenarioOutline:
        return "ScenarioOutline";
    case TokenType::Examples:
        return "Examples";
    case TokenType::Given:
        return "Given";
    case TokenType::When:
        return "When";
    case TokenType::Then:
        return "Then";
    case TokenType::And:
        return "And";
    case TokenType::But:
        return "But";
    case TokenType::Colon:
        return ":";
    case TokenType::Pipe:
        return "|";
    case TokenType::Tag:
        return "Tag->" + token.lexeme;
    case TokenType::StringLiteral:
        return "Literal->" + token.lexeme;
    case TokenType::EOL:
        return "\\n";
    case TokenType::EndOfFile:
        return "EOF";
    case TokenType::LeftAngle:
        return "<";
    case TokenType::RightAngle:
        return ">";
    case TokenType::Identifier:
        return "Identifier->" + token.lexeme;
    case TokenType::Placeholder:
        return "Placeholder->" + token.lexeme;
    default:
        return "Unknown";
    }
}

bool isKeyword(const TokenType& type)
{
    switch (type)
    {
    case TokenType::Feature:
    case TokenType::Background:
    case TokenType::Scenario:
    case TokenType::ScenarioOutline:
    case TokenType::Examples:
    case TokenType::Given:
    case TokenType::When:
    case TokenType::Then:
    case TokenType::And:
    case TokenType::But:
        return true;
    default:
        return false;
    }
}

bool isStep(const TokenType& type)
{
    switch (type)
    {
    case TokenType::Given:
    case TokenType::When:
    case TokenType::Then:
    case TokenType::And:
    case TokenType::But:
        return true;
    default:
        return false;
    }
}

} // namespace pep