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
#include "Object.h"

#include <iostream>

namespace pep
{

class AstPrinter : public ExpressionVisitor
{
public:
    void print(const Expression& expr)
    {
        expr.accept(*this);
        std::cout << std::endl;
    }

    Object visit(const LiteralExpression& /*expr*/) override { return NullLiteral{}; }
    Object visit(const PlaceholderExpression& /*expr*/) override { return NullLiteral{}; }
};

} // namespace pep