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

#include "BasicTestRunner.h"

#include "steps/StepRegistry.h"

#include <iostream>

namespace pep
{

int BasicTestRunner::runTests(const std::string& testName) const
{
    try
    {
        StepRegistry::getInstance().executeStep(testName);
        return 0; // success
    }
    catch (const std::exception& e)
    {
        std::cerr << "Caught exception: " << e.what() << std::endl;
        return 1; // failure (exception caught)
    }
}

} // namespace pep