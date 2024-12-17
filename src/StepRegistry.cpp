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
 ******************************************************************************/

#include "StepRegistry.h"
#include <algorithm>
#include <limits>
#include <unordered_map>

namespace pep
{
namespace
{
bool regexMatches(const std::string& gtest, const std::string& featureFile)
{
    // Iterate through the string s1 while a { is not found, if a { is found, then we parse the regex contained within
    // the {} and check if the string s2 matches the regex at that position If the regex matches, we continue iterating
    // through s1 until we find the next {, if we find a { we repeat the process

    size_t j = 0;
    // std::unordered_map<std::string, Step::Arguments> arguments;
    for (size_t i = 0; i < gtest.size() && j < featureFile.size(); ++i)
    {
        if (gtest[i] == '{')
        {
            // The regex should be something like {something:w}
            // Store the something in a string
            std::string regex;
            ++i;
            while (gtest[i] != ':')
            {
                if (gtest[i] == '}')
                {
                    // If no : is found this is not a valid regex
                    return false;
                }
                regex.push_back(gtest[i]);
                ++i;
            }
            // Advance the iterator to after :
            ++i;
            // There should now be a character indicating the type of the regex
            // char type = gtest[i];
            // switch (type)
            // {
            // case 'w':
            //     // The regex is a word
            //     // TODO : Use the already parsed feature file to add the word here
            //     arguments[regex] = "";
            //     break;
            // case 'd':
            //     // The regex is a digit
            //     // TODO : Use the already parsed feature file to add the digit here
            //     arguments[regex] = 42.0;
            //     break;
            // case 'b':
            //     // The regex is a boolean
            //     // TODO : Use the already parsed feature file to add the boolean here
            //     arguments[regex] = false;
            //     break;
            // default:
            //     return false;
            // }

            // Advance the regex identifier
            ++i;
            // String should now have a }
            if (gtest[i] != '}')
            {
                // If no } is found this is not a valid regex
                return false;
            }
        }
        else if (gtest[i] == featureFile[j])
        {
            // If the characters match, continue iterating through both strings
            ++j;
        }
        else
        {
            // If the characters do not match, return false
            return false;
        }
    }
    return true;
}

} // namespace

// The registered step can be something like:
// {me:w} sends struct with data={data:d}, name={name:w} and age={age:d}s
// The step is registered literally, but when resolving we must see if the passed string matche the regex of the
// registered step

void StepRegistry::registerStep(const std::string& name, const std::function<void(std::vector<std::string>)>& func)
{
    m_steps[name] = func;
}

std::function<void(std::vector<std::string>)> StepRegistry::resolveStep(const std::string& query) const
{
    // Iterate through all the registered steps
    for (const auto& step : m_steps)
    {
        // If the query matches the regex of the step, return the function
        if (regexMatches(query, step.first))
        {
            return step.second;
        }
    }
    return {};
}

} // namespace pep