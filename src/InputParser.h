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

#include <string>

namespace pep
{

// InputParser is responsible for parsing the raw user input.
// Here, it simply returns the input as the test name, but in a more complex scenario
// it could validate or extract more complex filtering criteria.
class InputParser
{
public:
    std::string parseInput(const std::string& input) const;
};

} // namespace pep