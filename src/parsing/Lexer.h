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

#include "Token.h"
#include <string>
#include <vector>

namespace pep
{
class Lexer
{
public:
    explicit Lexer(const std::string& source);
    std::vector<Token> tokenize();

private:
    std::string m_source;
    size_t m_current = 0;
    int m_line = 1;

    char peek() const;
    char advance();
    bool isAtEnd() const;
    void addToken(TokenType type, const std::string& lexeme, std::vector<Token>& tokens);
};

} // namespace pep
