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

#include "Catch2TestRunner.h"

#include <catch2/catch_session.hpp>

namespace pep
{

int Catch2TestRunner::runTests(const std::string& testName) const
{
    // Create a Catch2 session
    Catch::Session session;

    // Prepare arguments to filter tests by name.
    // The first argument is the executable name (dummy here) and the second is the test filter.
    std::vector<const char*> args = { "executable", testName.c_str() };

    // Run only tests whose names match the provided filter.
    return session.run(static_cast<int>(args.size()), args.data());
}

} // namespace pep