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

#include <stdexcept>
#include <string>

namespace pep
{

// Primary template declaration (no definition)
template <typename T> T convert(const std::string& str);

// Specialization for int
template <> inline int convert<int>(const std::string& str)
{
    return std::stoi(str);
}

// Specialization for double
template <> inline double convert<double>(const std::string& str)
{
    return std::stod(str);
}

// Specialization for std::string (the identity function)
template <> inline std::string convert<std::string>(const std::string& str)
{
    return str;
}

} // namespace pep