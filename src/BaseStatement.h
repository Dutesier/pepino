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

#include "BaseExpression.h"

#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <type_traits>
#include <variant>
#include <vector>

namespace pep
{

// Forward declaration
class FeatureStatement;
class BackgroundStatement;
class ScenarioStatement;
class StepStatement;
class ScenarioOutlineStatement;
class ExamplesStatement;
class TagStatement;

class StatementVisitor
{
public:
    virtual ~StatementVisitor() = default;

    virtual void visit(const FeatureStatement& stmt) = 0;
    virtual void visit(const BackgroundStatement& stmt) = 0;
    virtual void visit(const ScenarioStatement& stmt) = 0;
    virtual void visit(const StepStatement& stmt) = 0;
    virtual void visit(const ScenarioOutlineStatement& stmt) = 0;
    virtual void visit(const ExamplesStatement& stmt) = 0;
    virtual void visit(const TagStatement& stmt) = 0;
};

class Statement
{
public:
    virtual ~Statement() = default;

    // Accept method for the Visitor pattern
    virtual void accept(StatementVisitor& visitor) const = 0;
};

class TagStatement : public Statement
{
public:
    std::string name;

    explicit TagStatement(std::string name)
        : name(std::move(name))
    {
    }

    void accept(StatementVisitor& visitor) const override { visitor.visit(*this); }
};

class StepStatement : public Statement
{
public:
    std::string keyword; // e.g. "Given", "When", "Then"
    std::vector<std::unique_ptr<Expression>> expressions;
    // Each Expression could be a LiteralExpression or something else

    StepStatement(std::string keyword, std::vector<std::unique_ptr<Expression>> expressions)
        : keyword(std::move(keyword))
        , expressions(std::move(expressions))
    {
    }

    void accept(StatementVisitor& visitor) const override { visitor.visit(*this); }
};

class BackgroundStatement : public Statement
{
public:
    std::vector<std::unique_ptr<StepStatement>> steps;

    explicit BackgroundStatement(std::vector<std::unique_ptr<StepStatement>> steps)
        : steps(std::move(steps))
    {
    }

    void accept(StatementVisitor& visitor) const override { visitor.visit(*this); }
};

class ScenarioStatement : public Statement
{
public:
    std::string name;
    std::vector<std::unique_ptr<TagStatement>> tags;
    std::vector<std::unique_ptr<StepStatement>> steps;

    ScenarioStatement(
        std::string name,
        std::vector<std::unique_ptr<TagStatement>> tags,
        std::vector<std::unique_ptr<StepStatement>> steps)
        : name(std::move(name))
        , tags(std::move(tags))
        , steps(std::move(steps))
    {
    }

    void accept(StatementVisitor& visitor) const override { visitor.visit(*this); }
};

class ExamplesStatement : public Statement
{
public:
    // A simple structure for the examples table:
    // "headers" might be ["x", "y", "result"]
    // "rows" might be [ ["42", "24", "66"], ["2", "3", "5"] ]
    std::vector<std::string> headers;
    std::vector<std::vector<std::string>> rows;

    ExamplesStatement(std::vector<std::string> headers, std::vector<std::vector<std::string>> rows)
        : headers(std::move(headers))
        , rows(std::move(rows))
    {
    }

    void accept(StatementVisitor& visitor) const override { visitor.visit(*this); }
};

class ScenarioOutlineStatement : public Statement
{
public:
    std::string name;
    std::vector<std::unique_ptr<TagStatement>> tags;
    std::vector<std::unique_ptr<StepStatement>> steps;
    // Usually, there's exactly one ExamplesStatement, but let's store as unique_ptr
    std::unique_ptr<ExamplesStatement> examples;

    ScenarioOutlineStatement(
        std::string name,
        std::vector<std::unique_ptr<TagStatement>> tags,
        std::vector<std::unique_ptr<StepStatement>> steps,
        std::unique_ptr<ExamplesStatement> examples)
        : name(std::move(name))
        , tags(std::move(tags))
        , steps(std::move(steps))
        , examples(std::move(examples))
    {
    }

    void accept(StatementVisitor& visitor) const override { visitor.visit(*this); }
};

class FeatureStatement : public Statement
{
public:
    std::string name;
    std::vector<std::unique_ptr<TagStatement>> tags;
    std::unique_ptr<BackgroundStatement> background; // optional
    // The children of a feature could be ScenarioStatements or ScenarioOutlineStatements
    std::vector<std::unique_ptr<Statement>> children;

    FeatureStatement(
        std::string name,
        std::vector<std::unique_ptr<TagStatement>> tags,
        std::unique_ptr<BackgroundStatement> background,
        std::vector<std::unique_ptr<Statement>> children)
        : name(std::move(name))
        , tags(std::move(tags))
        , background(std::move(background))
        , children(std::move(children))
    {
    }

    void accept(StatementVisitor& visitor) const override { visitor.visit(*this); }
};

} // namespace pep
