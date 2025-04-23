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

#include "Lexer.h"
#include "../Logger.h"
#include "Token.h"
#include <string_view>

namespace pep
{

namespace
{
TokenType keywordType(const std::string& lexeme)
{
    if (lexeme == "Feature")
        return TokenType::Feature;
    if (lexeme == "Background")
        return TokenType::Background;
    if (lexeme == "Scenario")
        return TokenType::Scenario;
    if (lexeme == "ScenarioOutline")
        return TokenType::ScenarioOutline;
    if (lexeme == "Examples")
        return TokenType::Examples;
    if (lexeme == "Given")
        return TokenType::Given;
    if (lexeme == "When")
        return TokenType::When;
    if (lexeme == "Then")
        return TokenType::Then;
    if (lexeme == "And")
        return TokenType::And;
    if (lexeme == "But")
        return TokenType::But;
    return TokenType::StringLiteral;
}
} // namespace

Lexer::Lexer(const std::string& source) : m_source(source)
{
}

std::vector<Token> Lexer::tokenize()
{
    std::vector<Token> tokens;
    while (!isAtEnd())
    {
        char c = advance();
        if (c == '\n')
        {
            tokens.push_back({TokenType::EOL, "\\n", m_line++});
        }
        else if (std::isspace(c))
        {
            continue;
        }
        else if (c == '@')
        {
            // Parse a tag (e.g., "@smoke")
            std::string tag(1, c);
            while (!isAtEnd() && !std::isspace(peek()))
            {
                tag.push_back(advance());
            }
            tokens.push_back({TokenType::Tag, tag, m_line});
        }
        else if (c == ':')
        {
            tokens.push_back({TokenType::Colon, ":", m_line});
        }
        else if (c == '|')
        {
            tokens.push_back({TokenType::Pipe, "|", m_line});
        }
        else if (c == '<')
        {
            tokens.push_back({TokenType::LeftAngle, "<", m_line});
        }
        else if (c == '>')
        {
            tokens.push_back({TokenType::RightAngle, ">", m_line});
        }
        else
        {
            std::string literal;
            literal.push_back(c);
            while (!isAtEnd() && !std::isspace(peek()) && (peek() != '>') &&
                   (peek() != '<') && (peek() != ':'))
            {
                literal.push_back(advance());
            }
            auto type = keywordType(literal);
            if (type == TokenType::Scenario)
            {
                // Check if next word is "Outline"
                constexpr std::string_view Outline = " Outline";
                if (m_current + Outline.size() < m_source.size() &&
                    m_source.substr(m_current, Outline.size()) == Outline)
                {
                    literal += Outline.data();
                    m_current += Outline.size();
                    type = TokenType::ScenarioOutline;
                }
            }
            tokens.push_back({type, literal, m_line});
        }
    }
    tokens.push_back({TokenType::EndOfFile, "", m_line});
    Logger::debug("Tokens:");
    for (auto& token : tokens)
    {
        Logger::debug(tokenAsString(token));
    }
    return tokens;
}

char Lexer::peek() const
{
    if (m_current < m_source.size())
        return m_source[m_current];
    return '\0';
}

char Lexer::advance()
{
    return m_source[m_current++];
}

bool Lexer::isAtEnd() const
{
    return m_current >= m_source.size();
}

void Lexer::addToken(TokenType type, const std::string& lexeme,
                     std::vector<Token>& tokens)
{
    tokens.push_back({type, lexeme, m_line});
}

} // namespace pep
