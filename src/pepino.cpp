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
#include "BasicTestRunner.h"
#include "TestController.h"

namespace pep
{

int debug_runStep(const std::string& pattern)
{
    TestController interpreter(std::make_unique<BasicTestRunner>());
    return interpreter.executeTest(pattern);
}

int run(const std::string& filepath)
{
    TestController interpreter(std::make_unique<BasicTestRunner>());
    return interpreter.executeTest(filepath);
}

} // namespace pep