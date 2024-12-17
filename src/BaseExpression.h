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

#include "Object.h"

namespace pep
{

class LiteralExpression;
class PlaceholderExpression;
class ExpressionVisitor
{
public:
    virtual ~ExpressionVisitor() = default;

    virtual Object visit(const LiteralExpression& expr) = 0;
    virtual Object visit(const PlaceholderExpression& expr) = 0;
};

class Expression
{
public:
    virtual ~Expression() = default;
    // Accept method for the Visitor pattern
    virtual Object accept(ExpressionVisitor& visitor) const = 0;
};

class LiteralExpression : public Expression
{
public:
    LiteralExpression(Object object)
        : object(std::move(object))
    {
    }

    Object accept(ExpressionVisitor& visitor) const override { return visitor.visit(*this); }

    Object object;
};

// Concrete Expression: A placeholder (e.g. `{x:int}`) embedded in step text
class PlaceholderExpression : public Expression
{
public:
    std::string name; // e.g. "x"
    std::string type; // e.g. "int"

    PlaceholderExpression(std::string name, std::string type)
        : name(std::move(name))
        , type(std::move(type))
    {
    }

    Object accept(ExpressionVisitor& visitor) const override { return visitor.visit(*this); }
};

} // namespace pep
