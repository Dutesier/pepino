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
#include "pepino/types/types.h"

namespace pep
{
#define TOKEN_PASTE(x, y) x##y
#define TOKEN_PASTE2(x, y) TOKEN_PASTE(x, y)

#define STEP_CTX(ctxType, stepType, pattern, callback)                                                                 \
    namespace                                                                                                          \
    {                                                                                                                  \
    const bool TOKEN_PASTE2(_step_reg_, __COUNTER__) = []()                                                            \
    {                                                                                                                  \
        pep::StepRegistry::getInstance().registerStep<ctxType>(stepType, pattern, callback);                           \
        return true;                                                                                                   \
    }();                                                                                                               \
    }

/// — the “new” form, when you want to explicitly say which Context to use:
#define GIVEN_CTX(ctx, pat, cb) STEP_CTX(ctx, pep::types::StepType::Given, pat, cb)
#define WHEN_CTX(ctx, pat, cb) STEP_CTX(ctx, pep::types::StepType::When, pat, cb)
#define THEN_CTX(ctx, pat, cb) STEP_CTX(ctx, pep::types::StepType::Then, pat, cb)
#define AND_CTX(ctx, pat, cb) STEP_CTX(ctx, pep::types::StepType::And, pat, cb)
#define BUT_CTX(ctx, pat, cb) STEP_CTX(ctx, pep::types::StepType::But, pat, cb)

/// — backwards‐compatible “no‐context” macros all just bind to DefaultContext:
#define GIVEN(pat, cb) GIVEN_CTX(pep::DefaultContext, pat, cb)
#define WHEN(pat, cb) WHEN_CTX(pep::DefaultContext, pat, cb)
#define THEN(pat, cb) THEN_CTX(pep::DefaultContext, pat, cb)
#define AND(pat, cb) AND_CTX(pep::DefaultContext, pat, cb)
#define BUT(pat, cb) BUT_CTX(pep::DefaultContext, pat, cb)

} // namespace pep
