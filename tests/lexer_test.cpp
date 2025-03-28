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

#include "../src/parsing/Lexer.h"

#include <gtest/gtest.h>
#include <iomanip>

using namespace pep;

// Test 1: Empty Input
TEST(LexerTest, EmptyInput)
{
    std::string input = "";
    Lexer lexer(input);
    auto tokens = lexer.tokenize();

    // With empty input, only an EndOfFile token should be produced.
    ASSERT_EQ(tokens.size(), 1);
    EXPECT_EQ(tokens[0].type, TokenType::EndOfFile);
}

// Test 2: Whitespace and Newlines
TEST(LexerTest, WhitespaceAndNewlines)
{
    std::string input = "   \n   \n";
    Lexer lexer(input);
    auto tokens = lexer.tokenize();

    // Expect two EOL tokens (one for each newline) and an EndOfFile token.
    ASSERT_EQ(tokens.size(), 3);
    EXPECT_EQ(tokens[0].type, TokenType::EOL);
    EXPECT_EQ(tokens[1].type, TokenType::EOL);
    EXPECT_EQ(tokens[2].type, TokenType::EndOfFile);
}

// Test 3: Tag Token
TEST(LexerTest, TagToken)
{
    std::string input = "@smoke";
    Lexer lexer(input);
    auto tokens = lexer.tokenize();

    // The first token should be a Tag with the lexeme "@smoke", followed by EndOfFile.
    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].type, TokenType::Tag);
    EXPECT_EQ(tokens[0].lexeme, "@smoke");
    EXPECT_EQ(tokens[1].type, TokenType::EndOfFile);
}

// Test 4: Feature Declaration
TEST(LexerTest, FeatureDeclaration)
{
    std::string input = "Feature : Login\n";
    Lexer lexer(input);
    auto tokens = lexer.tokenize();

    // Expected tokens:
    // 0: "Feature" as StringLiteral,
    // 1: ":" as Colon,
    // 2: "Login" as StringLiteral,
    // 3: EOL,
    // 4: EndOfFile.
    ASSERT_GE(tokens.size(), 5);
    EXPECT_EQ(tokens[0].type, TokenType::Feature);
    EXPECT_EQ(tokens[0].lexeme, "Feature");
    EXPECT_EQ(tokens[1].type, TokenType::Colon);
    EXPECT_EQ(tokens[1].lexeme, ":");
    EXPECT_EQ(tokens[2].type, TokenType::StringLiteral);
    EXPECT_EQ(tokens[2].lexeme, "Login");
    EXPECT_EQ(tokens[3].type, TokenType::EOL);
    EXPECT_EQ(tokens[4].type, TokenType::EndOfFile);
}

// Test 5: Scenario with Steps
TEST(LexerTest, ScenarioWithSteps)
{
    std::string input = "Scenario : Successful_login\n"
                        "Given a user exists\n"
                        "When the user logs in\n"
                        "Then login is successful\n";
    Lexer lexer(input);
    auto tokens = lexer.tokenize();

    // Check that key keywords are present in the token stream.
    bool foundScenario = false;
    bool foundGiven = false;
    bool foundWhen = false;
    bool foundThen = false;
    for (const auto& token : tokens)
    {
        if (token.lexeme == "Scenario")
            foundScenario = true;
        if (token.lexeme == "Given")
            foundGiven = true;
        if (token.lexeme == "When")
            foundWhen = true;
        if (token.lexeme == "Then")
            foundThen = true;
    }
    EXPECT_TRUE(foundScenario);
    EXPECT_TRUE(foundGiven);
    EXPECT_TRUE(foundWhen);
    EXPECT_TRUE(foundThen);
}

// Test 6: Placeholder Tokens
TEST(LexerTest, PlaceholderTokens)
{
    std::string input = "Given a user <username> exists\n";
    Lexer lexer(input);
    auto tokens = lexer.tokenize();

    // Expected token sequence:
    // "Given" (StringLiteral or StepKeyword), "a" (StringLiteral), "user" (StringLiteral),
    // "<" (LeftAngle), "username" (StringLiteral), ">" (RightAngle),
    // "exists" (StringLiteral), EOL, EndOfFile.
    for (auto& token : tokens)
    {
        std::cout << tokenAsString(token) << std::endl;
        std::cout << std::quoted(token.lexeme) << std::endl;
    }
    ASSERT_GE(tokens.size(), 9);
    // Check that tokens 3, 4, 5 correspond to the placeholder delimiters and content.
    EXPECT_EQ(tokens[3].type, TokenType::LeftAngle);
    EXPECT_EQ(tokens[4].type, TokenType::StringLiteral);
    EXPECT_EQ(tokens[4].lexeme, "username");
    EXPECT_EQ(tokens[5].type, TokenType::RightAngle);
}

// Test 7: Examples Table
TEST(LexerTest, ExamplesTable)
{
    std::string input = "Examples :\n"
                        "| username | password |\n"
                        "| john_doe | secret   |\n";
    Lexer lexer(input);
    auto tokens = lexer.tokenize();

    // Verify that the table markers and column names are present.
    bool foundPipe = false;
    bool foundUsername = false;
    bool foundPassword = false;
    for (const auto& token : tokens)
    {
        if (token.type == TokenType::Pipe)
        {
            foundPipe = true;
        }
        if (token.lexeme == "username")
        {
            foundUsername = true;
        }
        if (token.lexeme == "password")
        {
            foundPassword = true;
        }
    }
    EXPECT_TRUE(foundPipe);
    EXPECT_TRUE(foundUsername);
    EXPECT_TRUE(foundPassword);
}