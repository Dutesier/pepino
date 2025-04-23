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

#include "hooks/HookRegistry.h"

#include "Logger.h"

namespace pep
{

void HookRegistry::registerAfterAll(
    std::function<void(const types::FeatureInfo&)>&& hook)
{
    if (!hook)
    {
        throw std::invalid_argument("Hook cannot be null");
    }

    if (m_afterAllHook)
    {
        throw std::runtime_error("AfterAll hook already registered");
    }

    m_afterAllHook = std::move(hook);
}

void HookRegistry::executeAfterAll(const types::FeatureInfo& feature)
{
    Logger::info("After all.");
    if (m_afterAllHook)
    {
        m_afterAllHook(feature);
    }
}

void HookRegistry::registerBeforeAll(
    std::function<void(const types::FeatureInfo&)>&& hook)
{
    if (!hook)
    {
        throw std::invalid_argument("Hook cannot be null");
    }

    if (m_beforeAllHook)
    {
        throw std::runtime_error("BeforeAll hook already registered");
    }

    m_beforeAllHook = std::move(hook);
}

void HookRegistry::executeBeforeAll(const types::FeatureInfo& feature)
{
    Logger::info("Before all.");
    if (m_beforeAllHook)
    {
        m_beforeAllHook(feature);
    }
}

void HookRegistry::registerBefore(
    std::function<void(const types::ScenarioInfo&)>&& hook)
{
    if (!hook)
    {
        throw std::invalid_argument("Hook cannot be null");
    }

    if (m_beforeHook)
    {
        throw std::runtime_error("Before hook already registered");
    }

    m_beforeHook = std::move(hook);
}

void HookRegistry::executeBefore(const types::ScenarioInfo& scenario)
{
    Logger::info("Before scenario: " + scenario.name);
    if (m_beforeHook)
    {
        m_beforeHook(scenario);
    }
}

void HookRegistry::registerAfter(
    std::function<void(const types::ScenarioInfo&)>&& hook)
{
    if (!hook)
    {
        throw std::invalid_argument("Hook cannot be null");
    }

    if (m_afterHook)
    {
        throw std::runtime_error("After hook already registered");
    }

    m_afterHook = std::move(hook);
}

void HookRegistry::executeAfter(const types::ScenarioInfo& scenario)
{
    Logger::info("After scenario: " + scenario.name);
    if (m_afterHook)
    {
        m_afterHook(scenario);
    }
}

void HookRegistry::registerBeforeStep(
    std::function<void(const types::StepInfo&)>&& hook)
{
    if (!hook)
    {
        throw std::invalid_argument("Hook cannot be null");
    }

    if (m_beforeStepHook)
    {
        throw std::runtime_error("BeforeStep hook already registered");
    }

    m_beforeStepHook = std::move(hook);
}

void HookRegistry::executeBeforeStep(const types::StepInfo& step)
{
    Logger::info("Before step: " + step.name);
    if (m_beforeStepHook)
    {
        m_beforeStepHook(step);
    }
}

void HookRegistry::registerAfterStep(
    std::function<void(const types::StepInfo&)>&& hook)
{
    if (!hook)
    {
        throw std::invalid_argument("Hook cannot be null");
    }

    if (m_afterStepHook)
    {
        throw std::runtime_error("AfterStep hook already registered");
    }

    m_afterStepHook = std::move(hook);
}

void HookRegistry::executeAfterStep(const types::StepInfo& step)
{
    Logger::info("After step: " + step.name);
    if (m_afterStepHook)
    {
        m_afterStepHook(step);
    }
}

void HookRegistry::clearHooks()
{
    m_afterAllHook = nullptr;
    m_beforeAllHook = nullptr;
    m_beforeHook = nullptr;
    m_afterHook = nullptr;
    m_beforeStepHook = nullptr;
    m_afterStepHook = nullptr;
}

} // namespace pep
