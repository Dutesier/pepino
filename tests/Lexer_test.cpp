/******************************************************************************
 * Project:  Pepino
 * Brief:    A C++ Pepino Interpreter.
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

#include "../src/Lexer.h"

#include <gtest/gtest.h>
#include <string_view>
#include <vector>

class TestLexer : public testing::Test
{
public:
    void SetUp() override {}

    void TearDown() override {}

protected:
};

TEST_F(TestLexer, tokenizeHelloWorld)
{
    using enum pep::TokenType;
    std::string_view HelloWorld = "Feature: Hello, World!\n"
                                  "  Scenario: Hello, World!\n"
                                  "    Given I have a string\n"
                                  "    When I print the string\n"
                                  "    Then I should see the string\n"
                                  "    And I should see that 42==42\n"
                                  "!-- Unless its commented of course\n";
    std::vector<pep::Token> expectedTokens{ pep::Token{ Feature, std::monostate{}, "", 1 },
                                            pep::Token{ Identifier, std::monostate{}, "Hello", 1 },
                                            pep::Token{ Identifier, std::monostate{}, "World", 1 },
                                            pep::Token{ Scenario, std::monostate{}, "", 2 },
                                            pep::Token{ Identifier, std::monostate{}, "Hello", 2 },
                                            pep::Token{ Identifier, std::monostate{}, "World", 2 },
                                            pep::Token{ Given, std::monostate{}, "", 3 },
                                            pep::Token{ Identifier, "I", "", 3 },
                                            pep::Token{ Identifier, "have", "", 3 },
                                            pep::Token{ Identifier, "a", "", 3 },
                                            pep::Token{ Identifier, "string", "", 3 },
                                            pep::Token{ When, std::monostate{}, "", 4 },
                                            pep::Token{ Identifier, "I", "", 4 },
                                            pep::Token{ Identifier, "print", "", 4 },
                                            pep::Token{ Identifier, "the", "", 4 },
                                            pep::Token{ Identifier, "string", "", 4 },
                                            pep::Token{ Then, std::monostate{}, "", 5 },
                                            pep::Token{ Identifier, "I", "", 5 },
                                            pep::Token{ Identifier, "should", "", 5 },
                                            pep::Token{ Identifier, "see", "", 5 },
                                            pep::Token{ Identifier, "the", "", 5 },
                                            pep::Token{ Identifier, "string", "", 5 },
                                            pep::Token{ And, std::monostate{}, "", 6 },
                                            pep::Token{ Identifier, "I", "", 6 },
                                            pep::Token{ Identifier, "should", "", 6 },
                                            pep::Token{ Identifier, "see", "", 6 },
                                            pep::Token{ Identifier, "that", "", 6 },
                                            pep::Token{ Number, 42.0, "", 6 },
                                            pep::Token{ Identifier, "==", "", 6 },
                                            pep::Token{ Number, 42.0, "", 6 },
                                            pep::Token{ Eof, std::monostate{}, "", 6 }

    };
    pep::Lexer lex(HelloWorld);

    auto output = lex.tokenize();
    // Print the output
    for (const auto& token : output)
    {
        std::cout << token.print() << std::endl;
    }

    ASSERT_EQ(expectedTokens.size(), output.size());
    for (std::size_t i = 0; i < output.size(); ++i)
    {
        EXPECT_EQ(output[i].type, expectedTokens[i].type);
    }
}

TEST_F(TestLexer, tokenizeFeature)
{
    using enum pep::TokenType;
    std::string_view source = "Feature: Sample Feature\n";
    std::vector<pep::Token> expectedTokens{ pep::Token{ Feature, std::monostate{}, "", 1 },
                                            pep::Token{ Identifier, std::monostate{}, "Sample", 1 },
                                            pep::Token{
                                                Identifier,
                                                std::monostate{},
                                                "Feature",
                                                1 }, // Intentional, keywords should be at the beginning of the line.
                                            pep::Token{ Eof, std::monostate{}, "", 1 } };
    pep::Lexer lex(source);

    auto output = lex.tokenize();
    ASSERT_EQ(expectedTokens.size(), output.size());
    for (std::size_t i = 0; i < output.size(); ++i)
    {
        EXPECT_EQ(output[i].type, expectedTokens[i].type);
    }
}

TEST_F(TestLexer, tokenizeScenario)
{
    using enum pep::TokenType;
    std::string_view source = "Scenario: Sample Scenario\n";
    std::vector<pep::Token> expectedTokens{ pep::Token{ Scenario, std::monostate{}, "", 1 },
                                            pep::Token{ Identifier, std::monostate{}, "Sample", 1 },
                                            pep::Token{ Identifier, std::monostate{}, "Scenario", 1 },
                                            pep::Token{ Eof, std::monostate{}, "", 1 } };
    pep::Lexer lex(source);

    auto output = lex.tokenize();
    ASSERT_EQ(expectedTokens.size(), output.size());
    for (std::size_t i = 0; i < output.size(); ++i)
    {
        EXPECT_EQ(output[i].type, expectedTokens[i].type);
    }
}

TEST_F(TestLexer, tokenizeGivenWhenThen)
{
    using enum pep::TokenType;
    std::string_view source = "Given I have a string\nWhen I print the string\nThen I should see the string\n";
    std::vector<pep::Token> expectedTokens{
        pep::Token{ Given, std::monostate{}, "", 1 }, pep::Token{ Identifier, "I", "", 1 },
        pep::Token{ Identifier, "have", "", 1 },      pep::Token{ Identifier, "a", "", 1 },
        pep::Token{ Identifier, "string", "", 1 },    pep::Token{ When, std::monostate{}, "", 2 },
        pep::Token{ Identifier, "I", "", 2 },         pep::Token{ Identifier, "print", "", 2 },
        pep::Token{ Identifier, "the", "", 2 },       pep::Token{ Identifier, "string", "", 2 },
        pep::Token{ Then, std::monostate{}, "", 3 },  pep::Token{ Identifier, "I", "", 3 },
        pep::Token{ Identifier, "should", "", 3 },    pep::Token{ Identifier, "see", "", 3 },
        pep::Token{ Identifier, "the", "", 3 },       pep::Token{ Identifier, "string", "", 3 },
        pep::Token{ Eof, std::monostate{}, "", 3 }
    };
    pep::Lexer lex(source);

    auto output = lex.tokenize();
    ASSERT_EQ(expectedTokens.size(), output.size());
    for (std::size_t i = 0; i < output.size(); ++i)
    {
        EXPECT_EQ(output[i].type, expectedTokens[i].type);
    }
}

TEST_F(TestLexer, tokenizeNumbers)
{
    using enum pep::TokenType;
    std::string_view source = "42 3.14\n";
    std::vector<pep::Token> expectedTokens{ pep::Token{ Number, 42.0, "", 1 },
                                            pep::Token{ Number, 3.14, "", 1 },
                                            pep::Token{ Eof, std::monostate{}, "", 1 } };
    pep::Lexer lex(source);

    auto output = lex.tokenize();
    ASSERT_EQ(expectedTokens.size(), output.size());
    for (std::size_t i = 0; i < output.size(); ++i)
    {
        EXPECT_EQ(output[i].type, expectedTokens[i].type);
    }
}

TEST_F(TestLexer, tokenizeComments)
{
    using enum pep::TokenType;
    std::string_view source = "!-- This is a comment\n";
    std::vector<pep::Token> expectedTokens{ pep::Token{ Eof, std::monostate{}, "", 1 } };
    pep::Lexer lex(source);

    auto output = lex.tokenize();
    ASSERT_EQ(expectedTokens.size(), output.size());
    for (std::size_t i = 0; i < output.size(); ++i)
    {
        EXPECT_EQ(output[i].type, expectedTokens[i].type);
    }
}