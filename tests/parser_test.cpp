
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

#include "../src/parsing/Parser.h"

#include <gtest/gtest.h>
#include <iostream>
#include <vector>

using namespace pep;

// ------------------------------
// Minimal Feature: No children
// ------------------------------
TEST(ParserTest, ParseMinimalFeature)
{
    // Tokens:
    //   Feature : MyFeature
    //   EndOfFile
    std::vector<Token> tokens = { Token{ TokenType::Feature, "Feature", 1 },
                                  Token{ TokenType::Colon, ":", 1 },
                                  Token{ TokenType::StringLiteral, "MyFeature", 1 },
                                  Token{ TokenType::EndOfFile, "", 1 } };

    Parser parser(tokens);
    std::unique_ptr<FeatureStatement> feature = parser.parseFeature();
    ASSERT_NE(feature, nullptr);
    EXPECT_EQ(feature->name, "MyFeature");
    EXPECT_TRUE(feature->tags.empty());
    EXPECT_TRUE(feature->scenarioOutlines.empty());
    EXPECT_TRUE(feature->scenarios.empty());
    EXPECT_EQ(feature->background, nullptr);
}

// ----------------------------------------------------
// Feature with Background and a single step in it
// ----------------------------------------------------
TEST(ParserTest, ParseFeatureWithBackground)
{
    // Tokens:
    //   Feature : FeatureWithBackground
    //   Background :
    //     Given a user exists
    //   EndOfFile
    std::vector<Token> tokens = { Token{ TokenType::Feature, "Feature", 1 },
                                  Token{ TokenType::Colon, ":", 1 },
                                  Token{ TokenType::StringLiteral, "FeatureWithBackground", 1 },
                                  // Background section
                                  Token{ TokenType::Background, "Background", 2 },
                                  Token{ TokenType::Colon, ":", 2 },
                                  // One step in background:
                                  Token{ TokenType::Given, "Given", 2 },
                                  Token{ TokenType::StringLiteral, "a user exists", 2 },
                                  Token{ TokenType::EndOfFile, "", 2 } };

    Parser parser(tokens);
    std::unique_ptr<FeatureStatement> feature = parser.parseFeature();
    ASSERT_NE(feature, nullptr);
    EXPECT_EQ(feature->name, "FeatureWithBackground");
    ASSERT_NE(feature->background, nullptr);
    EXPECT_EQ(feature->background->steps.size(), 1);
    auto& step = feature->background->steps[0];
    EXPECT_EQ(step->keyword, "Given");
    std::vector<Token> expectedText = { Token{ TokenType::StringLiteral, "a user exists", 2 } };
    EXPECT_EQ(step->text, expectedText);
}

// ----------------------------------------------------
// Feature with a Scenario that has tags and steps
// ----------------------------------------------------
TEST(ParserTest, ParseFeatureWithScenario)
{
    // Tokens:
    //   @feature
    //   Feature : FeatureWithScenario
    //   @scenario
    //   Scenario : SuccessfulScenario
    //     Given user is logged in
    //   EndOfFile
    std::vector<Token> tokens = { // Feature-level tag:
                                  Token{ TokenType::Tag, "@feature", 1 },
                                  Token{ TokenType::Feature, "Feature", 1 },
                                  Token{ TokenType::Colon, ":", 1 },
                                  Token{ TokenType::StringLiteral, "FeatureWithScenario", 1 },
                                  // Scenario-level (preceded by a tag):
                                  Token{ TokenType::Tag, "@scenario", 2 },
                                  Token{ TokenType::Scenario, "Scenario", 2 },
                                  Token{ TokenType::Colon, ":", 2 },
                                  Token{ TokenType::StringLiteral, "SuccessfulScenario", 2 },
                                  // A step in the scenario:
                                  Token{ TokenType::Given, "Given", 3 },
                                  Token{ TokenType::StringLiteral, "user is logged in", 3 },
                                  Token{ TokenType::EndOfFile, "", 3 }
    };

    Parser parser(tokens);
    std::unique_ptr<FeatureStatement> feature = parser.parseFeature();
    ASSERT_NE(feature, nullptr);
    // Feature tags.
    ASSERT_EQ(feature->tags.size(), 1);
    EXPECT_EQ(feature->tags[0], "@feature");

    // Scenario parsing.
    ASSERT_EQ(feature->scenarios.size(), 1);
    auto& scenario = feature->scenarios[0];
    ASSERT_EQ(scenario->tags.size(), 1);
    EXPECT_EQ(scenario->tags[0], "@scenario");
    EXPECT_EQ(scenario->name, "SuccessfulScenario");
    ASSERT_EQ(scenario->steps.size(), 1);
    auto& step = scenario->steps[0];
    EXPECT_EQ(step->keyword, "Given");
    std::vector<Token> expectedText = { Token{ TokenType::StringLiteral, "user is logged in", 3 } };
    EXPECT_EQ(step->text, expectedText);
}

// ----------------------------------------------------
// Feature with a Scenario Outline and Examples table
// ----------------------------------------------------
TEST(ParserTest, ParseFeatureWithScenarioOutline)
{
    // Tokens:
    //   Feature : FeatureWithOutline
    //   ScenarioOutline : OutlineScenario
    //     Given user <username> attempts login
    //   Examples :
    //     | username |
    //     | user1    |
    //     | user2    |
    //   EndOfFile
    std::vector<Token> tokens = { Token{ TokenType::Feature, "Feature", 1 },
                                  Token{ TokenType::Colon, ":", 1 },
                                  Token{ TokenType::StringLiteral, "FeatureWithOutline", 1 },
                                  // Scenario Outline section:
                                  Token{ TokenType::ScenarioOutline, "ScenarioOutline", 2 },
                                  Token{ TokenType::Colon, ":", 2 },
                                  Token{ TokenType::StringLiteral, "OutlineScenario", 2 },
                                  // Step in scenario outline:
                                  Token{ TokenType::Given, "Given", 3 },
                                  Token{ TokenType::StringLiteral, "user ", 3 },
                                  Token{ TokenType::LeftAngle, "<", 3 },
                                  Token{ TokenType::StringLiteral, "username", 3 },
                                  Token{ TokenType::RightAngle, ">", 3 },
                                  Token{ TokenType::StringLiteral, " attempts login", 3 },
                                  // Examples section:
                                  Token{ TokenType::Examples, "Examples", 4 },
                                  Token{ TokenType::Colon, ":", 4 },
                                  // For simplicity, assume header is a single token:
                                  Token{ TokenType::Pipe, "|", 5 },
                                  Token{ TokenType::StringLiteral, "username", 5 },
                                  Token{ TokenType::Pipe, "|", 5 },
                                  Token{ TokenType::EOL, "", 5 },
                                  // Example rows:
                                  Token{ TokenType::Pipe, "|", 6 },
                                  Token{ TokenType::StringLiteral, "user1", 6 },
                                  Token{ TokenType::Pipe, "|", 6 },
                                  Token{ TokenType::EOL, "", 6 },
                                  Token{ TokenType::Pipe, "|", 7 },
                                  Token{ TokenType::StringLiteral, "user2", 7 },
                                  Token{ TokenType::Pipe, "|", 7 },
                                  // End of file:
                                  Token{ TokenType::EndOfFile, "", 8 } };

    Parser parser(tokens);
    std::unique_ptr<FeatureStatement> feature = parser.parseFeature();
    ASSERT_NE(feature, nullptr);
    ASSERT_EQ(feature->scenarioOutlines.size(), 1);
    auto& outline = feature->scenarioOutlines[0];
    EXPECT_EQ(outline->name, "OutlineScenario");
    ASSERT_EQ(outline->steps.size(), 1);
    auto& step = outline->steps[0];
    EXPECT_EQ(step->keyword, "Given");

    std::vector<Token> expectedText = { Token{ TokenType::StringLiteral, "user ", 3 },
                                        Token{ TokenType::Placeholder, "username", 3 },
                                        Token{ TokenType::StringLiteral, " attempts login", 3 } };

    EXPECT_EQ(step->text, expectedText);
    for (auto& token : step->text)
    {
        std::cout << tokenAsString(token) << std::endl;
    }
    ASSERT_NE(outline->examples, nullptr);
    ASSERT_EQ(outline->examples->headers.size(), 1);
    EXPECT_EQ(outline->examples->headers[0], "username");
}

// ----------------------------------------------------
// Error Case: Missing colon after Feature keyword
// ----------------------------------------------------
TEST(ParserTest, MissingColonAfterFeatureThrows)
{
    // Tokens: Missing colon between "Feature" and feature name.
    std::vector<Token> tokens = { Token{ TokenType::Feature, "Feature", 1 },
                                  // Colon is missing here:
                                  Token{ TokenType::StringLiteral, "MyFeature", 1 },
                                  Token{ TokenType::EndOfFile, "", 1 } };

    Parser parser(tokens);
    EXPECT_THROW({ auto feature = parser.parseFeature(); }, std::runtime_error);
}

// ----------------------------------------------------
// Error Case: Invalid keyword at start (not "Feature")
// ----------------------------------------------------
TEST(ParserTest, InvalidKeywordThrows)
{
    // Tokens: First token is not "Feature"
    std::vector<Token> tokens = { Token{ TokenType::StringLiteral, "NotAFeature", 1 },
                                  Token{ TokenType::Colon, ":", 1 },
                                  Token{ TokenType::StringLiteral, "Whatever", 1 },
                                  Token{ TokenType::EndOfFile, "", 1 } };

    Parser parser(tokens);
    EXPECT_THROW({ auto feature = parser.parseFeature(); }, std::runtime_error);
}