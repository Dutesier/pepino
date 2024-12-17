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

#include "IStepRegistry.h"
#include "Logger.h"

#include <functional>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

namespace pep
{

class StepRegistry : public IStepRegistry
{
public:
    /**
     * @brief Registers a step with a given name.
     *
     * @param name The name of the step.
     * @param func The function to be associated with the step name.
     */
    void registerStep(const std::string& name, const std::function<void(std::vector<std::string>)>& func) override;

    /**
     * @brief Resolves a step by its name.
     *
     * @param name The name of the step to resolve.
     * @return The function associated with the step name.
     */
    std::function<void(std::vector<std::string>)> resolveStep(const std::string& name) const override;

    /**
     * @brief Returns the instance of the StepRegistry.
     *
     * @return The instance of the StepRegistry.
     */
    static StepRegistry& instance()
    {
        static StepRegistry instance;
        return instance;
    }

    void debug_runAll()
    {
        for (const auto& [name, func] : m_steps)
        {
            Logger::debug("Running step: " + name);
            func({});
        }
    }

private:
    class Step
    {
    public:
        Step(std::string name, std::function<void(std::vector<std::string>)> func)
            : m_name(std::move(name))
            , m_func(std::move(func))
        {
        }

        int arity;
        std::string m_name;
        std::function<void(std::vector<std::string>)> m_func;

        using Arguments = std::variant<std::string, double, bool>;

    private:
        std::unordered_map<std::string, Arguments> m_arguments;
    };

    std::unordered_map<std::string, std::function<void(std::vector<std::string>)>> m_steps;
};

} // namespace pep