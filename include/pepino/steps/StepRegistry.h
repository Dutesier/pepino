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

#include <exception>
#include <functional>
#include <iostream>
#include <regex>
#include <stdexcept>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

#include "TypeConverters.h"
#include "pepino/context.h"
#include "pepino/types/types.h"

namespace pep
{

template <typename T> struct function_traits;

// Extract operator() signature for lambdas/functors
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
        types::StepType type;
        std::regex pattern;
        std::string patternStr;
        int specificity;
        std::function<void(const std::vector<std::string>&)> func;
    };
    using StepDefinitionPtr = std::shared_ptr<StepDefinition>;

public:
    static StepRegistry& getInstance()
    {
        static StepRegistry instance;
        return instance;
    }

    /// Register a step whose callback takes (DerivedContext&, Args...).
    /// The wrapper will fetch DerivedContext::getInstance() internally.
    template <typename DerivedContext, typename Callback>
    void registerStep(types::StepType type, const std::string& patternStr, Callback callback)
    {
        std::regex pattern(patternStr);
        int spec = computeSpecificity(patternStr);

        // Wrap user callback: grab the singleton and dispatch
        auto wrapper = [callback](const std::vector<std::string>& args)
        {
            auto& ctx = DerivedContext::getInstance();
            callWithArgs(callback, ctx, args);
        };

        auto stepDef = std::make_shared<StepDefinition>();
        stepDef->type = type;
        stepDef->pattern = std::move(pattern);
        stepDef->patternStr = patternStr;
        stepDef->specificity = spec;
        stepDef->func = std::move(wrapper);

        steps.push_back(std::move(stepDef));
    }

    /// Match `stepText` against all registered patterns, pick the most
    /// specific, extract captures, and invoke its wrapper.
    void executeStep(const std::string& stepText) const
    {
        std::vector<StepDefinitionPtr> candidates;
        for (const auto& sd : steps)
        {
            std::smatch m;
            if (std::regex_match(stepText, m, sd->pattern))
                candidates.push_back(sd);
        }
        if (candidates.empty())
        {
            throw std::runtime_error("No matching step found for: (START)" + stepText + "(END)");
        }

        // Choose highest specificity
        auto best = candidates.front();
        for (auto& cand : candidates)
        {
            if (cand->specificity > best->specificity)
                best = cand;
        }

        std::cout << "Executing step with regex: " << best->patternStr << std::endl;

        std::smatch match;
        if (!std::regex_match(stepText, match, best->pattern))
        {
            throw std::runtime_error("No matching step found for: (START)" + stepText + "(END)");
        }

        std::vector<std::string> captures;
        for (size_t i = 1; i < match.size(); ++i)
            captures.push_back(match[i].str());

        best->func(captures);
    }

    class UnimplementedStepException : public std::exception
    {
    public:
        explicit UnimplementedStepException(const std::string& message)
            : m_message("Unimplemented step: " + message)
        {
        }
        const char* what() const noexcept override { return m_message.c_str(); }

    private:
        std::string m_message;
    };

    class FailedStepException : public std::exception
    {
    public:
        explicit FailedStepException(const std::string& message)
            : m_prefix("Step failed: " + message)
        {
        }

        const char* what() const noexcept override { return m_prefix.c_str(); }

    private:
        std::string m_prefix = "Step failed: ";
    };

private:
    std::vector<StepDefinitionPtr> steps;
    StepRegistry() = default;

    // Heuristic: count literals and tokens to rank specificity
    int computeSpecificity(const std::string& pat) const
    {
        int score = 0;
        size_t pos = 0;
        while (pos < pat.size())
        {
            if (pat.compare(pos, 2, "\\d") == 0)
            {
                score += 2;
                pos += 2;
            }
            else if (pat.compare(pos, 2, "\\w") == 0)
            {
                score += 1;
                pos += 2;
            }
            else if (pat[pos] == '^' || pat[pos] == '$')
            {
                score += 3;
                pos += 1;
            }
            else if (pat[pos] == '[')
            {
                auto end = pat.find(']', pos);
                if (end != std::string::npos)
                {
                    int len = static_cast<int>(end - pos - 1);
                    score += (len > 0 ? len : 1);
                    pos = end + 1;
                }
                else
                {
                    pos++;
                }
            }
            else if (std::isalnum(static_cast<unsigned char>(pat[pos])))
            {
                score += 4;
                pos++;
            }
            else
            {
                pos++;
            }
        }
        return score;
    }

    // Unpack and convert args, dropping the first tuple element (the context)
    template <typename Callback, typename DerivedContext>
    static void callWithArgs(Callback callback, DerivedContext& ctx, const std::vector<std::string>& args)
    {
        using Functor = std::remove_reference_t<Callback>;
        using Traits = function_traits<decltype(&Functor::operator())>;
        using Tuple = typename Traits::args_tuple;
        constexpr size_t N = std::tuple_size<Tuple>::value;
        if (args.size() != N - 1)
        {
            throw std::runtime_error("Argument count mismatch in step callback");
        }
        callHelperImpl<Callback, DerivedContext>(callback, ctx, args, std::make_index_sequence<N - 1>{});
    }

    // IndexSequence to shift each capture into the correct argument slot
    template <typename Callback, typename DerivedContext, size_t... I>
    static void callHelperImpl(
        Callback callback,
        DerivedContext& ctx,
        const std::vector<std::string>& args,
        std::index_sequence<I...>)
    {
        using Functor = std::remove_reference_t<Callback>;
        using Traits = function_traits<decltype(&Functor::operator())>;
        using Tuple = typename Traits::args_tuple;

        callback(ctx, convert<typename std::tuple_element<I + 1, Tuple>::type>(args[I])...);
    }
};

} // namespace pep
