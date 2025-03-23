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

#include "StepRegistry.h"

namespace pep
{
#define TOKEN_PASTE(x, y) x##y
#define TOKEN_PASTE2(x, y) TOKEN_PASTE(x, y)

#ifndef STEP
#define STEP(pattern, callback)                                                                                        \
    namespace                                                                                                          \
    {                                                                                                                  \
    const bool TOKEN_PASTE2(step_registration_dummy_, __COUNTER__) = []()                                              \
    {                                                                                                                  \
        pep::StepRegistry::getInstance().registerStep(pattern, callback);                                              \
        return true;                                                                                                   \
    }();                                                                                                               \
    }
#endif

} // namespace pep