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

#include "HookRegistry.h"

// Helper macros for token concatenation.
#define HOOK_PASTE(x, y) x##y

#define HOOK_PASTE2(x, y) HOOK_PASTE(x, y)

#define UNIQUE_NAME(base) HOOK_PASTE2(base, __COUNTER__)

// This macro defines a hook with a parameter. It creates a unique function
// name, and then uses that name consistently in both the registration call and the function declaration.
#define REGISTER_HOOK_WITH_PARAM_IMPL(regFunc, paramType, uniqueName)                                                  \
    static void uniqueName(const paramType& info);                                                                     \
    static int TOKEN_PASTE2(uniqueName, _registrar) = []() -> int                                                      \
    {                                                                                                                  \
        pep::HookRegistry::getInstance().regFunc(uniqueName);                                                          \
        return 0;                                                                                                      \
    }();                                                                                                               \
    static void uniqueName(const paramType& info)

// This macro wraps REGISTER_HOOK_WITH_PARAM_IMPL to generate a unique name.
#define REGISTER_HOOK_WITH_PARAM(regFunc, paramType)                                                                   \
    REGISTER_HOOK_WITH_PARAM_IMPL(regFunc, paramType, UNIQUE_NAME(HookFunc_))

// Now define specialized macros that build on REGISTER_HOOK_WITH_PARAM:
// Users can write, for example:
//     BEFORE_ALL() {
//         // code using a const types::FeatureInfo& parameter
//     }
#define BEFORE_ALL() REGISTER_HOOK_WITH_PARAM(registerBeforeAll, pep::types::FeatureInfo)
#define AFTER_ALL() REGISTER_HOOK_WITH_PARAM(registerAfterAll, pep::types::FeatureInfo)
#define BEFORE() REGISTER_HOOK_WITH_PARAM(registerBefore, pep::types::ScenarioInfo)
#define AFTER() REGISTER_HOOK_WITH_PARAM(registerAfter, pep::types::ScenarioInfo)
#define BEFORE_STEP() REGISTER_HOOK_WITH_PARAM(registerBeforeStep, pep::types::StepInfo)
#define AFTER_STEP() REGISTER_HOOK_WITH_PARAM(registerAfterStep, pep::types::StepInfo)