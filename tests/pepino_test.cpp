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

class PepinoTest : public testing::Test
{
};

// @fake_backend
// Feature: Login functionality

//   Background:
//     Given a user exists with username "user" and password "pass"

//   Scenario: Successful login
//     Given the user is on the login page
//     When the user enters valid credentials
//     Then they should be redirected to the dashboard

//   Scenario Outline: Unsuccessful login attempts
//     Given the user is on the login page
//     When the user enters <username> and <password>
//     Then they should see an error message

//     Examples:
//       | username | password |
//       | user     | wrongpass |
//       | unknown  | pass      |

GIVEN("^a user exists with username \"(\\w+)\" and password "
      "\"(\\w+)\"$",
      [](std::string username, std::string password) {
          std::cout << "User exists with username: " << username
                    << " and password: " << password << std::endl;
      });
GIVEN("the user is on the login page",
      []() { std::cout << "User is on the login page." << std::endl; });
WHEN("the user enters valid credentials",
     []() { std::cout << "User enters valid credentials." << std::endl; });
THEN("they should be redirected to the dashboard", []() {
    std::cout << "User is redirected to the dashboard." << std::endl;
});
WHEN("the user enters (\\w+) and (\\w+)",
     [](std::string username, std::string password) {
         std::cout << "User enters username: " << username
                   << " and password: " << password << std::endl;
     });
THEN("they should see an error message",
     []() { std::cout << "User sees an error message." << std::endl; });

TEST_F(PepinoTest, pepinoRuns)
{
    auto ret = pep::run("tests/data/normal_pepino.feature");
    EXPECT_EQ(ret, 0);
}