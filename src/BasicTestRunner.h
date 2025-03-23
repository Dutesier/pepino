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

namespace pep
{
// BasicTestRunner implements ITestRunner by querying the StepRegistry singleton.
// It looks for a callback associated with the provided test name (or step name)
// and invokes it if found.
class BasicTestRunner : public ITestRunner
{
public:
    int runTests(const std::string& testName) const override;
};

} // namespace pep