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

#include "Token.h"

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace pep
{
// Base class for all statements.
class Statement
{
public:
    virtual ~Statement() = default;
};

class StepStatement : public Statement
{
public:
    std::string keyword;                                    // e.g., "Given", "When", "Then"
    std::vector<Token> text;                                // The step text; may contain placeholders like "<var>"
    std::unordered_map<std::string, std::string> variables; // Placeholder values
};

class BackgroundStatement : public Statement
{
public:
    std::vector<std::unique_ptr<StepStatement>> steps;
};

class ScenarioStatement : public Statement
{
public:
    std::vector<std::string> tags;
    std::string name;
    std::vector<std::unique_ptr<StepStatement>> steps;
};

class ExamplesStatement : public Statement
{
public:
    std::vector<std::string> headers;
    std::vector<std::vector<std::string>> rows;
};

class ScenarioOutlineStatement : public Statement
{
public:
    std::vector<std::string> tags;
    std::string name;
    std::vector<std::unique_ptr<StepStatement>> steps;
    std::unique_ptr<ExamplesStatement> examples;
};

class FeatureStatement : public Statement
{
public:
    std::vector<std::string> tags;
    std::string name;
    std::unique_ptr<BackgroundStatement> background;
    std::vector<std::unique_ptr<ScenarioStatement>> scenarios;
    std::vector<std::unique_ptr<ScenarioOutlineStatement>> scenarioOutlines;
};

} // namespace pep
