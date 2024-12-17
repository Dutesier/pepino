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

#pragma once

#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

namespace pep
{

/**
 * @class IStepRegistry
 * @brief A registry for storing and resolving steps.
 *
 * The StepRegistry class provides a mechanism to register and resolve steps.
 * Steps are stored as std::function objects that take a vector of strings as parameters.
 */
class IStepRegistry
{
public:
    /**
     * @brief Registers a step with a given name.
     *
     * @param name The name of the step.
     * @param func The function to be associated with the step name.
     */
    virtual void registerStep(const std::string& name, const std::function<void(std::vector<std::string>)>& func) = 0;

    /**
     * @brief Resolves a step by its name.
     *
     * @param name The name of the step to resolve.
     * @return The function associated with the step name.
     */
    virtual std::function<void(std::vector<std::string>)> resolveStep(const std::string& name) const = 0;
};

} // namespace pep