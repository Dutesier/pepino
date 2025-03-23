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

#include "TestController.h"

namespace pep
{

TestController::TestController(std::unique_ptr<ITestRunner> runner)
    : testRunner(std::move(runner))
{
}

int TestController::executeTest(const std::string& input)
{
    std::string testName = inputParser.parseInput(input);
    return testRunner->runTests(testName);
}

} // namespace pep