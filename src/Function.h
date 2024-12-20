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

#include "BaseStatement.h"
#include "Environment.h"
#include "Object.h"

namespace pep
{

class Function : public Callable
{
public:
    Function(const FunctionStatement& declaration, std::shared_ptr<Environment> closure);

private:
    const FunctionStatement& declaration;
    std::shared_ptr<Environment> closure;
};

} // namespace pep