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

#include <ostream>
#include <string_view>
#include <variant>

namespace pep
{

enum TokenType
{
    // Single-character tokens.
    Pipe,

    // One or two character tokens.
    Comment,

    // Literals.
    Identifier,
    String,
    Number,

    // Keywords.
    Feature,
    Background,
    Scenario,
    ScenarioOutline,
    Examples,
    Given,
    When,
    Then,
    And,

    Eof,

    Error,
};

struct Token
{
    using LiteralValues = std::variant<std::monostate, std::string, double>;
    TokenType type;
    LiteralValues literal = std::monostate{};
    std::string_view location{};
    unsigned int lineNo{};

    std::string print() const;
};
bool operator==(const Token& lhs, const Token& rhs);
std::ostream& operator<<(std::ostream& os, const Token& me);

} // namespace pep