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

#include "Lexer.h"
#include "Logger.h"
#include "Token.h"

#include <sstream>
#include <string>
#include <unordered_map>
namespace
{

bool isDigit(char c)
{
    return c >= '0' && c <= '9';
}

bool isWhitespace(char c)
{
    return c == ' ' || c == '\r' || c == '\t' || c == '\n';
}

const std::unordered_map<std::string_view, pep::TokenType> KeywordsMap{ { "And", pep::TokenType::And },
                                                                        { "Feature:", pep::TokenType::Feature },
                                                                        { "Background:", pep::TokenType::Background },
                                                                        { "Scenario:", pep::TokenType::Scenario },
                                                                        { "ScenarioOutline:",
                                                                          pep::TokenType::ScenarioOutline },
                                                                        { "Examples", pep::TokenType::Examples },
                                                                        { "Given", pep::TokenType::Given },
                                                                        { "When", pep::TokenType::When },
                                                                        { "Then", pep::TokenType::Then } };

} // namespace

namespace pep
{

Lexer::Lexer(std::string_view source)
    : m_source(std::move(source))
{
}

std::vector<Token> Lexer::tokenize()
{
    std::vector<Token> tokens{};

    while (true)
    {
        auto tok = getNextToken();
        m_start = m_current;
        tokens.emplace_back(tok);
        if (tok.type == TokenType::Eof)
        {
            break;
        }
    }
    for (auto tok : tokens)
    {
        std::stringstream ss;
        ss << tok;
        Logger::debug(std::format("Got Token: {}", ss.str()));
    }
    return tokens;
}

char Lexer::advance()
{
    return m_source.at(m_current++);
}

Token Lexer::getNextToken()
{
    if (isAtEnd())
    {
        return Token{ Eof, std::monostate{}, "", m_line };
    }

    using enum TokenType;
    char c = advance();
    Logger::debug(std::format("Evaluating {}", c));
    switch (c)
    {
    case '|':
        return Token{ .type = Pipe, .lineNo = m_line };
    case '!':
        if (match('-') && peekNext() == '-')
        {
            advanceUntilEndOfLine();
            return rerun();
        }
        return Token{ .type = Identifier, .location = "!", .lineNo = m_line };
    case ' ':
    case '\r':
    case '\t':
        // Ignore whitespace.
        return rerun();
    case '\n':
        return Token{ .type = Eol, .location = "\n", .lineNo = m_line++ };

    // Literals
    // case '"':
    //     return getStringToken();
    default:
        if (isDigit(c))
        {
            return getNumberToken();
        }
        return getIdentifierToken();
    }
    Logger::error(std::format("[Line {}] Unexpected character -> {}.", m_line, c));
    return Token{ .type = Error, .location = std::string{ c }, .lineNo = m_line }; // Maybe throw here???
}

Token Lexer::rerun()
{
    m_start = m_current;
    return getNextToken();
}

Token Lexer::getNumberToken()
{
    // In Pepino every number is double!
    while (isDigit(peek()))
    {
        advance();
    }

    // Look for a fractional part.
    if (peek() == '.' && isDigit(peekNext()))
    {
        // Consume the "."
        advance();

        while (isDigit(peek()))
        {
            advance();
        }
    }

    auto str = std::string{ m_source.substr(m_start, m_current - m_start) };
    try
    {
        std::size_t index;
        double value = std::stod(str.data(), &index);
        return Token{ Number, std::move(value), "", m_line }; // Maybe throw here???
    }
    catch (const std::invalid_argument& e)
    {
        std::cerr << "Invalid argument: " << e.what() << std::endl;
        return Token{ Error, std::monostate{}, str, m_line }; // Maybe throw here???
    }
    catch (const std::out_of_range& e)
    {
        std::cerr << "Out of range: " << e.what() << std::endl;
        return Token{ Error, std::monostate{}, str, m_line }; // Maybe throw here???
    }
}

Token Lexer::getIdentifierToken()
{
    while (!isWhitespace(peek()) && !isAtEnd() && !match('|') && !isDigit(peek()))
    {
        advance();
    }

    auto text = std::string{ m_source.substr(m_start, m_current - m_start) };
    auto type = TokenType::Identifier;
    if (KeywordsMap.contains(text))
    {
        type = KeywordsMap.at(text);
        return Token{ type };
    }
    return Token{ type, text };
}

bool Lexer::isAtEnd()
{
    return m_current >= m_source.size();
}

bool Lexer::match(char next)
{
    if (isAtEnd())
    {
        return false;
    }
    if (m_source.at(m_current) != next)
    {
        return false;
    }
    return true;
}

void Lexer::advanceUntilEndOfLine()
{
    constexpr char NewLine = '\n';
    while (peek() != NewLine && !isAtEnd())
    {
        advance();
    }
}

char Lexer::peek()
{
    constexpr char NullTerminator = '\0';
    if (isAtEnd())
    {
        return NullTerminator;
    }
    return m_source.at(m_current);
}

char Lexer::peekNext()
{
    constexpr char NullTerminator = '\0';
    if (m_current + 1 >= m_source.size())
    {
        return NullTerminator;
    }
    return m_source.at(m_current + 1);
}

} // namespace pep