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

#include "parsing/Statement.h"
#include <memory>

namespace pep
{

// ITestRunner declares a simple interface for running tests by name.
// This abstraction allows you to change the underlying test framework without
// affecting the rest of the code (Dependency Inversion Principle).
class ITestRunner
{
public:
    virtual ~ITestRunner() = default;
    virtual int runTests(std::unique_ptr<FeatureStatement> feature) const = 0;
};

} // namespace pep
