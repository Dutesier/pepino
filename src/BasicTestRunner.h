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

#include "ITestRunner.h"
#include "parsing/Statement.h"

#include <exception>
#include <vector>

namespace pep
{
// BasicTestRunner implements ITestRunner by querying the StepRegistry
// singleton. It looks for a callback associated with the provided test name (or
// step name) and invokes it if found.
class BasicTestRunner : public ITestRunner
{
public:
    int runTests(std::unique_ptr<FeatureStatement> feature) const override;

private:
    // Runs the entire feature (background, scenarios, scenario outlines)
    void runFeature(const FeatureStatement& feature) const;
    // Run a single scenario (with an optional background)
    void runScenario(const ScenarioStatement& scenario) const;
    void runScenario(const ScenarioStatement& scenario,
                     const BackgroundStatement& background) const;

    // Run a single scenario outline (with an optional background)
    void runScenarioOutline(
        const ScenarioOutlineStatement& scenarioOutline) const;
    void runScenarioOutline(const ScenarioOutlineStatement& scenarioOutline,
                            const BackgroundStatement& background) const;

    // Run a single step from a step statement
    void runStep(const StepStatement& step) const;

    // Run a step given a substituted step text (for scenario outlines)
    void runStep(const std::string& substitutedStepText) const;

    // Helper: Substitute placeholders in a step text using the provided
    // mapping.
    std::string substitutePlaceholders(
        const std::vector<Token>& text,
        const std::unordered_map<std::string, std::string>& mapping) const;

public:
    // Custom exception for test failures
    class TestFailedException : public std::exception
    {
    public:
        explicit TestFailedException(const std::string& message)
            : m_message(message)
        {
        }
        const char* what() const noexcept override
        {
            return m_message.c_str();
        }

    private:
        std::string m_message;
    };
};

} // namespace pep