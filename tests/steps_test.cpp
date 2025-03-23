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

#include "pepino.h"
#include "steps/steps.h"

#include <gtest/gtest.h>

class PepinoStepsTest : public testing::Test
{
};

TEST_F(PepinoStepsTest, pepinoRunsSingleStep)
{
    auto ret = pep::debug_runStep("a number 42");
    EXPECT_EQ(ret, 0);
}

TEST_F(PepinoStepsTest, typeAssertion)
{
    auto ret = pep::debug_runStep("a name 42");
    auto ret2 = pep::debug_runStep("a name against");
    EXPECT_EQ(ret, 0);
    EXPECT_EQ(ret2, 0);
}

STEP("^a number (\\d+)$", [](int x) { std::cout << "Received int: " << x << std::endl; });

STEP("^a name (\\w+)$", [](std::string name) { std::cout << "Received name: " << name << std::endl; });

STEP("^a name (\\d+)$", [](int name) { std::cout << "Received name w/ number: " << name << std::endl; });