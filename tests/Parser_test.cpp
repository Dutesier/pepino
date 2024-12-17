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

#include "../src/AstPrinter.hpp"
#include "../src/Parser.h"

#include <experimental/source_location>
#include <gtest/gtest.h>
#include <string_view>
#include <vector>

using namespace pep;

class TestParser : public testing::Test
{
public:
    void SetUp() override {}

    void TearDown() override {}

protected:
};

TEST_F(TestParser, parseTokenExpression)
{
    using enum TokenType;
    // std::vector<Token> tokens{ { Number, 42.42, "", 1 }, { Plus }, { Number, 42.42, "", 1 }, { Semicolon }, { Eof }
    // };

    // Parser parser(tokens);
    // auto expressionPtrOpt = parser.parse();
    // ASSERT_TRUE(!expressionPtrOpt.empty());

    // auto& expressionPtr = *(expressionPtrOpt.value());
    // AstPrinter printer;
    // printer.print(expressionPtr);
    // FAIL();
}
TEST_F(TestParser, parseExampleExpressionSingleRow)
{
    using enum TokenType;
    std::vector<Token> tokens{ { Pipe, "", "", 1 },
                               { Identifier, "header1", "", 1 },
                               { Pipe, "", "", 1 },
                               { Identifier, "header2", "", 1 },
                               { Pipe, "", "", 1 },
                               { Pipe, "", "", 2 },
                               { Number, 42.0, "", 2 },
                               { Pipe, "", "", 2 },
                               { Identifier, "value", "", 2 },
                               { Pipe, "", "", 2 },
                               { Eof, "", "", 3 } };

    Parser parser(tokens);
    auto expressionPtr = parser.parse();
    ASSERT_TRUE(expressionPtr != nullptr);

    auto exampleExpr = dynamic_cast<ExampleExpression*>(expressionPtr.get());
    ASSERT_TRUE(exampleExpr != nullptr);

    ASSERT_EQ(exampleExpr->header.size(), 2);
    EXPECT_EQ(exampleExpr->header[0].type, Identifier);
    EXPECT_EQ(exampleExpr->header[0].location, "header1");
    EXPECT_EQ(exampleExpr->header[1].type, Identifier);
    EXPECT_EQ(exampleExpr->header[1].location, "header2");

    ASSERT_EQ(exampleExpr->data.size(), 1);
    ASSERT_EQ(exampleExpr->data[0].size(), 2);
    EXPECT_EQ(exampleExpr->data[0][0].type, Number);
    EXPECT_EQ(std::get<double>(exampleExpr->data[0][0].literal), 42.0);
    EXPECT_EQ(exampleExpr->data[0][1].type, Identifier);
    EXPECT_EQ(exampleExpr->data[0][1].location, "value");
}

TEST_F(TestParser, parseExampleExpressionMultipleRows)
{
    using enum TokenType;
    std::vector<Token> tokens{ { Pipe, "", "", 1 },
                               { Identifier, "header1", "", 1 },
                               { Pipe, "", "", 1 },
                               { Identifier, "header2", "", 1 },
                               { Pipe, "", "", 1 },
                               { Pipe, "", "", 2 },
                               { Number, 42.0, "", 2 },
                               { Pipe, "", "", 2 },
                               { Identifier, "value1", "", 2 },
                               { Pipe, "", "", 2 },
                               { Pipe, "", "", 3 },
                               { Number, 43.0, "", 3 },
                               { Pipe, "", "", 3 },
                               { Identifier, "value2", "", 3 },
                               { Pipe, "", "", 3 },
                               { Eof, "", "", 4 } };

    Parser parser(tokens);
    auto expressionPtr = parser.parse();
    ASSERT_TRUE(expressionPtr != nullptr);

    auto exampleExpr = dynamic_cast<ExampleExpression*>(expressionPtr.get());
    ASSERT_TRUE(exampleExpr != nullptr);

    ASSERT_EQ(exampleExpr->header.size(), 2);
    EXPECT_EQ(exampleExpr->header[0].type, Identifier);
    EXPECT_EQ(exampleExpr->header[0].location, "header1");
    EXPECT_EQ(exampleExpr->header[1].type, Identifier);
    EXPECT_EQ(exampleExpr->header[1].location, "header2");

    ASSERT_EQ(exampleExpr->data.size(), 2);
    ASSERT_EQ(exampleExpr->data[0].size(), 2);
    EXPECT_EQ(exampleExpr->data[0][0].type, Number);
    EXPECT_EQ(std::get<double>(exampleExpr->data[0][0].literal), 42.0);
    EXPECT_EQ(exampleExpr->data[0][1].type, Identifier);
    EXPECT_EQ(exampleExpr->data[0][1].location, "value1");

    ASSERT_EQ(exampleExpr->data[1].size(), 2);
    EXPECT_EQ(exampleExpr->data[1][0].type, Number);
    EXPECT_EQ(std::get<double>(exampleExpr->data[1][0].literal), 43.0);
    EXPECT_EQ(exampleExpr->data[1][1].type, Identifier);
    EXPECT_EQ(exampleExpr->data[1][1].location, "value2");
}

TEST_F(TestParser, parseExampleExpressionEmpty)
{
    using enum TokenType;
    std::vector<Token> tokens{ { Pipe, "", "", 1 }, { Pipe, "", "", 1 }, { Eof, "", "", 2 } };

    Parser parser(tokens);
    EXPECT_THROW(parser.parse(), Parser::ParserException);
}