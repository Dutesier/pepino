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

#include "types/types.h"

#include <exception>
#include <functional>
#include <memory>
#include <string_view>
#include <unordered_map>

namespace pep
{

class HookRegistry
{

public:
    // Returns the singleton instance.
    static HookRegistry& getInstance()
    {
        static HookRegistry instance;
        return instance;
    }

    void registerAfterAll(std::function<void(const types::FeatureInfo&)>&& hook);
    void executeAfterAll(const types::FeatureInfo& feature);
    void registerBeforeAll(std::function<void(const types::FeatureInfo&)>&& hook);
    void executeBeforeAll(const types::FeatureInfo& feature);

    void registerBefore(std::function<void(const types::ScenarioInfo&)>&& hook);
    void executeBefore(const types::ScenarioInfo& scenario);
    void registerAfter(std::function<void(const types::ScenarioInfo&)>&& hook);
    void executeAfter(const types::ScenarioInfo& scenario);

    void registerBeforeStep(std::function<void(const types::StepInfo&)>&& hook);
    void executeBeforeStep(const types::StepInfo& step);
    void registerAfterStep(std::function<void(const types::StepInfo&)>&& hook);
    void executeAfterStep(const types::StepInfo& step);

    // Prevent copying and assignment.
    HookRegistry(const HookRegistry&) = delete;
    HookRegistry& operator=(const HookRegistry&) = delete;
    HookRegistry(HookRegistry&&) = delete;
    HookRegistry& operator=(HookRegistry&&) = delete;
    ~HookRegistry() = default;

    // Clears all registered hooks.
    void clearHooks();

private:
    HookRegistry() = default;

    std::function<void(const types::FeatureInfo&)> m_afterAllHook;
    std::function<void(const types::FeatureInfo&)> m_beforeAllHook;
    std::function<void(const types::ScenarioInfo&)> m_beforeHook;
    std::function<void(const types::ScenarioInfo&)> m_afterHook;
    std::function<void(const types::StepInfo&)> m_beforeStepHook;
    std::function<void(const types::StepInfo&)> m_afterStepHook;
};

} // namespace pep