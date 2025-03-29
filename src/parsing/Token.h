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
#pragma once

#include <string>

namespace pep
{

enum class TokenType
{
    // Structural keywords.
    Feature,
    Background,
    Scenario,
    ScenarioOutline,
    Examples,

    // Steps
    Given,
    When,
    Then,
    And,
    But,

    // Punctuation
    Colon,
    Pipe, // for examples table columns

    // Others
    Tag,           // lines starting with '@'
    StringLiteral, // quoted strings or the remaining text of a line
    EOL,           // end-of-line
    EndOfFile,

    // For placeholders (e.g., <var>)
    LeftAngle,
    RightAngle,
    Placeholder,

    // Identifier (for unquoted names)
    Identifier
};

struct Token
{
    TokenType type;
    std::string lexeme;
    int line;

    bool operator==(const Token& other) const = default;
};

std::string tokenAsString(const Token& token);

bool isKeyword(const TokenType& type);
bool isStep(const TokenType& type);

} // namespace pep
