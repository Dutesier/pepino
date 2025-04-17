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

#include "context.h"
#include "pepino.h"
#include "steps/steps.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace testing;

class PepinoStepsTest : public testing::Test
{
};

class MyContext : public pep::Context<MyContext>
{
public:
    std::string name{};
    int number{};
    testing::MockFunction<void()> mockCb;
};

TEST_F(PepinoStepsTest, pepinoRunsSingleStep)
{
    auto& ctx = MyContext::getInstance();
    EXPECT_CALL(ctx.mockCb, Call()).Times(1);
    auto ret = pep::debug_runStep("tests/data/one_step.feature");
    EXPECT_EQ(ret, 0);
    Mock::VerifyAndClearExpectations(&ctx.mockCb); // Needed for reporting in the actual test
}

TEST_F(PepinoStepsTest, typeAssertion)
{
    pep::debug_runStep("tests/data/type_assertion.feature");
    EXPECT_EQ(MyContext::getInstance().name, "against");
    EXPECT_EQ(MyContext::getInstance().number, 42);
}

GIVEN_CTX(
    MyContext,
    "^a number (\\d+)$",
    [](MyContext& ctx, int x)
    {
        std::cout << "Received int: " << x << std::endl;
        ctx.mockCb.AsStdFunction()();
    });

GIVEN_CTX(
    MyContext,
    "^a name (\\w+)$",
    [](MyContext& ctx, std::string name)
    {
        std::cout << "Received name: " << name << std::endl;
        ctx.name = name;
    });

GIVEN_CTX(
    MyContext,
    "^a name (\\d+)$",
    [](MyContext& ctx, int name)
    {
        std::cout << "Received name w/ number: " << name << std::endl;
        ctx.number = name;
    });
