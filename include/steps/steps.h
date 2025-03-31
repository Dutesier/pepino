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
#include "types/types.h"

namespace pep
{
#define TOKEN_PASTE(x, y) x##y
#define TOKEN_PASTE2(x, y) TOKEN_PASTE(x, y)

#define TOKEN_PASTE(x, y) x##y
#define TOKEN_PASTE2(x, y) TOKEN_PASTE(x, y)

#ifndef STEP
#define STEP(type, pattern, callback)                                                                                  \
    namespace                                                                                                          \
    {                                                                                                                  \
    const bool TOKEN_PASTE2(step_registration_dummy_, __COUNTER__) = []()                                              \
    {                                                                                                                  \
        pep::StepRegistry::getInstance().registerStep(type, pattern, callback);                                        \
        return true;                                                                                                   \
    }();                                                                                                               \
    }
#endif

#define GIVEN(pattern, callback) STEP(pep::types::StepType::Given, pattern, callback)
#define WHEN(pattern, callback) STEP(pep::types::StepType::When, pattern, callback)
#define THEN(pattern, callback) STEP(pep::types::StepType::Then, pattern, callback)

} // namespace pep