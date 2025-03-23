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
#pragma once

#include "ITestRunner.h"
#include "InputParser.h"

#include <memory>

namespace pep
{

// TestController orchestrates the process:
// It gets the input string, uses InputParser to extract the test name,
// and then calls ITestRunner to run the corresponding Catch2 test.
class TestController
{
public:
    TestController(std::unique_ptr<ITestRunner> runner);

    int executeTest(const std::string& input);

private:
    std::unique_ptr<ITestRunner> testRunner;
    InputParser inputParser;
};

} // namespace pep