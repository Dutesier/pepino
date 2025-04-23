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
#include <vector>

namespace pep::types
{

enum class StepType
{
    Given,
    When,
    Then,
    And,
    But
};

struct StepInfo
{
    StepType type;
    std::string name;
};

struct FeatureInfo
{
    std::string name;
    std::vector<std::string> tags;
};

struct ScenarioInfo
{
    std::string name;
    std::vector<std::string> tags;
};

} // namespace pep::types
