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
#include "parsing/Statement.h"
#include <memory>

namespace pep
{

// Catch2TestRunner is a concrete implementation of ITestRunner using Catch2.
// It sets up a Catch::Session and passes the test filter as a command-line argument.
class Catch2TestRunner : public ITestRunner
{
public:
    int runTests(std::unique_ptr<FeatureStatement> feature) const override;
};

} // namespace pep