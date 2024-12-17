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

    Object visit(const StepDescriptionExpression& expr) override
    {
        std::cout << "StepDescriptionExpression" << std::endl;
        // Print all tokens
        for (const auto& token : expr.tokens)
        {
            std::cout << token.print() << std::endl;
        }
        return {};
    }

    Object visit(const ExampleExpression& expr) override
    {
        std::cout << "ExampleExpression" << std::endl;
        // Print all tokens
        for (const auto& token : expr.header)
        {
            std::cout << token.print() << std::endl;
        }

        for (const auto& row : expr.data)
        {
            for (const auto& token : row)
            {
                std::cout << token.print() << std::endl;
            }
        }
        return {};
    }
};

} // namespace pep