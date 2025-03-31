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

#include <functional>
#include <iostream>
#include <regex>
#include <stdexcept>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

#include "TypeConverters.h"
#include "types/types.h"

namespace pep
{

template <typename T> struct function_traits;

template <typename ClassType, typename ReturnType, typename... Args>
struct function_traits<ReturnType (ClassType::*)(Args...) const>
{
    using args_tuple = std::tuple<Args...>;
};

class StepRegistry
{
private:
    struct StepDefinition
    {
        types::StepType type; // The type of step (Given, When, Then, etc.)
        std::regex pattern;
        std::string patternStr; // Store the original regex string.
        int specificity;        // A computed metric; higher means more specific.
        // The callback wrapped to accept a vector of capture groups.
        std::function<void(const std::vector<std::string>&)> func;
    };

    using StepDefinitionPtr = std::shared_ptr<StepDefinition>;

public:
    static StepRegistry& getInstance()
    {
        static StepRegistry instance;
        return instance;
    }

    // Registration: compute specificity and store the original pattern.
    template <typename Callback>
    void registerStep(types::StepType type, const std::string& patternStr, Callback callback)
    {
        std::regex pattern(patternStr);
        int spec = computeSpecificity(patternStr);
        auto wrapper = [callback](const std::vector<std::string>& args) { callWithArgs(callback, args); };

        auto stepDef = std::make_shared<StepDefinition>();
        stepDef->pattern = pattern;
        stepDef->patternStr = patternStr;
        stepDef->specificity = spec;
        stepDef->func = wrapper;
        stepDef->type = type;
        steps.push_back(stepDef);
    }

    // Execute a step: choose among all matching candidates the one with highest
    // specificity.
    void executeStep(const std::string& stepText) const
    {
        std::vector<StepDefinitionPtr> candidates;
        for (const auto& stepDef : steps)
        {
            std::smatch match;
            if (std::regex_match(stepText, match, stepDef->pattern))
            {
                candidates.push_back(stepDef);
            }
        }
        if (candidates.empty())
        {
            throw std::runtime_error("No matching step found for: (START)" + stepText + "(END)");
        }
        // Choose candidate with highest specificity.
        StepDefinitionPtr bestCandidate = candidates.front();
        for (const auto& candidate : candidates)
        {
            if (candidate->specificity > bestCandidate->specificity)
            {
                bestCandidate = candidate;
            }
        }
        // For debugging: print the chosen regex literal.
        std::cout << "Executing step with regex: " << bestCandidate->patternStr << std::endl;

        // Extract capture groups.
        std::smatch match;
        if (std::regex_match(stepText, match, bestCandidate->pattern))
        {
            std::vector<std::string> captures;
            for (size_t i = 1; i < match.size(); i++)
            {
                captures.push_back(match[i].str());
            }
            bestCandidate->func(captures);
        }
        else
        {
            throw std::runtime_error("Unexpected failure: candidate best match did not match");
        }
    }

private:
    std::vector<StepDefinitionPtr> steps;
    StepRegistry() = default;

    int computeSpecificity(const std::string& patternStr) const
    {
        int score = 0;
        size_t pos = 0;
        while (pos < patternStr.size())
        {
            // Check for digit tokens.
            if (patternStr.compare(pos, 2, "\\d") == 0)
            {
                score += 2; // moderate weight for digit placeholders
                pos += 2;
            }
            // Check for word tokens.
            else if (patternStr.compare(pos, 2, "\\w") == 0)
            {
                score += 1; // lower weight for generic word characters
                pos += 2;
            }
            // Check for anchors, which can improve specificity.
            else if (patternStr[pos] == '^' || patternStr[pos] == '$')
            {
                score += 3;
                pos += 1;
            }
            // Check for a character class.
            else if (patternStr[pos] == '[')
            {
                size_t endBracket = patternStr.find(']', pos);
                if (endBracket != std::string::npos)
                {
                    // Weight is proportional to the size of the character
                    // class.
                    int length = static_cast<int>(endBracket - pos - 1);
                    score += (length > 0 ? length : 1);
                    pos = endBracket + 1;
                }
                else
                {
                    pos++; // if no closing bracket, just advance
                }
            }
            // Count literal alphanumeric characters as highly specific.
            else if (std::isalnum(patternStr[pos]))
            {
                score += 4;
                pos++;
            }
            // Otherwise, skip meta characters or punctuation.
            else
            {
                pos++;
            }
        }
        return score;
    }

    // Helper to call the callback with converted arguments.
    template <typename Callback, typename... Args, std::size_t... I>
    static void callHelper(Callback callback, const std::vector<std::string>& args, std::index_sequence<I...>)
    {
        callback(convert<Args>(args[I])...);
    }

    // Extract the callback's argument types and invoke it.
    template <typename Callback> static void callWithArgs(Callback callback, const std::vector<std::string>& args)
    {
        using Functor = std::remove_reference_t<Callback>;
        using Traits = function_traits<decltype(&Functor::operator())>;
        constexpr std::size_t N = std::tuple_size<typename Traits::args_tuple>::value;
        if (args.size() != N)
            throw std::runtime_error("Argument count mismatch in step callback");
        callHelperImpl(callback, args, std::make_index_sequence<N>{});
    }

    // Unpack arguments and call the callback.
    template <typename Callback, std::size_t... I>
    static void callHelperImpl(Callback callback, const std::vector<std::string>& args, std::index_sequence<I...>)
    {
        using Functor = std::remove_reference_t<Callback>;
        using Traits = function_traits<decltype(&Functor::operator())>;
        callback(convert<typename std::tuple_element<I, typename Traits::args_tuple>::type>(args[I])...);
    }
};

} // namespace pep