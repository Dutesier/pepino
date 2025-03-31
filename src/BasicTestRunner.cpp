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

#include "BasicTestRunner.h"

#include "Logger.h"
#include "hooks/HookRegistry.h"
#include "parsing/Statement.h"
#include "parsing/Token.h"
#include "steps/StepRegistry.h"

#include <iostream>
#include <memory>
#include <numeric>
#include <vector>

namespace pep
{

namespace
{
// Helper function to get the step type from the first word of the step text.
types::StepType getStepType(const std::string& stepText)
{
    auto firstSpace = stepText.find(' ');
    const std::string type = (firstSpace == std::string::npos) ? stepText : stepText.substr(0, firstSpace);
    if (type == "Given")
    {
        return types::StepType::Given;
    }
    else if (type == "When")
    {
        return types::StepType::When;
    }
    else if (type == "Then")
    {
        return types::StepType::Then;
    }
    else if (type == "And")
    {
        return types::StepType::And;
    }
    else if (type == "But")
    {
        return types::StepType::But;
    }
    else
    {
        throw std::runtime_error("Unknown step type: " + type);
    }
}
} // namespace

int BasicTestRunner::runTests(std::unique_ptr<FeatureStatement> feature) const
{
    if (!feature)
    {
        std::cerr << "No feature to run." << std::endl;
        return 1; // failure (no feature)
    }
    auto myFeature = std::move(feature);
    try
    {
        runFeature(*myFeature);
        return 0; // success
    }
    catch (const TestFailedException& e)
    {
        std::cerr << "Test failed: " << e.what() << std::endl;
        return 42; // failure (test failed)
    }
    catch (const std::exception& e)
    {
        std::cerr << "Caught exception: " << e.what() << std::endl;
        return 2; // failure (exception caught)
    }
}

void BasicTestRunner::runFeature(const FeatureStatement& feature) const
{
    types::FeatureInfo featureInfo{ feature.name, feature.tags };
    HookRegistry::getInstance().executeBeforeAll(featureInfo);
    // Run each scenario. If a background exists (i.e. feature.background is
    // non-null), pass it by const reference; otherwise, run the scenario
    // without it.
    for (const auto& scenario : feature.scenarios)
    {
        types::ScenarioInfo scenarioInfo{ scenario->name, scenario->tags };
        if (feature.background)
        {
            HookRegistry::getInstance().executeBefore(scenarioInfo);
            runScenario(*scenario, *feature.background);
            HookRegistry::getInstance().executeAfter(scenarioInfo);
        }
        else
        {
            HookRegistry::getInstance().executeBefore(scenarioInfo);
            runScenario(*scenario);
            HookRegistry::getInstance().executeAfter(scenarioInfo);
        }
    }
    // Run each scenario outline similarly.
    for (const auto& scenarioOutline : feature.scenarioOutlines)
    {
        types::ScenarioInfo scenarioInfo{ scenarioOutline->name, scenarioOutline->tags };
        if (feature.background)
        {
            HookRegistry::getInstance().executeBefore(scenarioInfo);
            runScenarioOutline(*scenarioOutline, *feature.background);
            HookRegistry::getInstance().executeAfter(scenarioInfo);
        }
        else
        {
            HookRegistry::getInstance().executeBefore(scenarioInfo);
            runScenarioOutline(*scenarioOutline);
            HookRegistry::getInstance().executeAfter(scenarioInfo);
        }
    }

    HookRegistry::getInstance().executeAfterAll(featureInfo);
}

// Run a scenario without a background.
void BasicTestRunner::runScenario(const ScenarioStatement& scenario) const
{
    std::cout << "Running Scenario: " << scenario.name << std::endl;
    for (const auto& step : scenario.steps)
    {
        runStep(*step);
    }
}

// Run a scenario with a background.
void BasicTestRunner::runScenario(const ScenarioStatement& scenario, const BackgroundStatement& background) const
{
    std::cout << "Running Scenario: " << scenario.name << " with Background" << std::endl;
    // First run all background steps.
    for (const auto& step : background.steps)
    {
        runStep(*step);
    }
    // Then run the scenario's own steps.
    runScenario(scenario);
}

// Run a scenario outline without a background.
void BasicTestRunner::runScenarioOutline(const ScenarioOutlineStatement& scenarioOutline) const
{
    std::cout << "Running Scenario Outline: " << scenarioOutline.name << std::endl;
    if (!scenarioOutline.examples)
    {
        throw TestFailedException("Scenario Outline has no Examples");
        return;
    }
    const auto& headers = scenarioOutline.examples->headers;
    for (const auto& row : scenarioOutline.examples->rows)
    {
        if (row.size() != headers.size())
        {
            std::cerr << "Warning: In Scenario Outline '" << scenarioOutline.name
                      << "', header count and row size do not match." << std::endl;
            continue;
        }
        std::unordered_map<std::string, std::string> mapping;
        for (size_t i = 0; i < headers.size(); ++i)
        {
            mapping[headers[i]] = row[i];
        }
        std::cout << "Running Scenario Outline iteration with mapping: ";
        for (const auto& kv : mapping)
        {
            std::cout << "<" << kv.first << ">=" << kv.second << " ";
        }
        std::cout << std::endl;
        for (const auto& step : scenarioOutline.steps)
        {
            std::string substituted = substitutePlaceholders(step->text, mapping);
            runStep(getStepType(step->keyword), substituted);
        }
    }
}

// Run a scenario outline with a background.
void BasicTestRunner::runScenarioOutline(
    const ScenarioOutlineStatement& scenarioOutline,
    const BackgroundStatement& background) const
{
    // First run all background steps.
    for (const auto& step : background.steps)
    {
        runStep(*step);
    }

    // Then run the scenario outline steps.
    runScenarioOutline(scenarioOutline);
}

// Run a single step from a StepStatement.
void BasicTestRunner::runStep(const StepStatement& step) const
{
    // Iterate through the step text and throw on placeholders.
    for (const auto& token : step.text)
    {
        if (token.type == TokenType::Placeholder)
        {
            throw TestFailedException("Step contains unbound placeholder: " + token.lexeme);
        }
    }
    std::string literal = std::accumulate(
        std::next(step.text.begin()),
        step.text.end(),
        step.text.front().lexeme,
        [](const std::string& a, const Token& b) { return a + " " + b.lexeme; });

    types::StepInfo stepType{ getStepType(step.keyword), literal };
    HookRegistry::getInstance().executeBeforeStep(stepType);
    StepRegistry::getInstance().executeStep(literal);
    HookRegistry::getInstance().executeAfterStep(stepType);
}

// Run a single step given substituted step text.
void BasicTestRunner::runStep(const types::StepType& type, const std::string& substitutedStepText) const
{
    std::cout << "Running substituted step: " << substitutedStepText << std::endl;

    types::StepInfo stepInfo{ type, substitutedStepText };

    HookRegistry::getInstance().executeBeforeStep(stepInfo);
    StepRegistry::getInstance().executeStep(substitutedStepText);
    HookRegistry::getInstance().executeAfterStep(stepInfo);
}

// Substitute placeholders in the given text.
// For each mapping pair, replace occurrences of <header> with the corresponding
// value.
std::string BasicTestRunner::substitutePlaceholders(
    const std::vector<Token>& text,
    const std::unordered_map<std::string, std::string>& mapping) const
{
    std::string literal = std::accumulate(
        std::next(text.begin()),
        text.end(),
        text.front().lexeme,
        [&mapping](const std::string& a, const Token& b)
        {
            if (b.type == TokenType::Placeholder)
            {
                auto it = mapping.find(b.lexeme);
                if (it != mapping.end())
                {
                    return a + " " + it->second;
                }
                else
                {
                    throw TestFailedException("Unbound placeholder: " + b.lexeme);
                }
            }
            return a + " " + b.lexeme;
        });
    return literal;
}

} // namespace pep