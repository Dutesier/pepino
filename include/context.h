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

namespace pep
{

template <typename Derived> class Context
{
public:
    /// Returns the one and only instance of Derived.
    static Derived& getInstance()
    {
        static Derived instance;
        return instance;
    }

    // Disable copy & move — there is only one instance!
    Context(const Context&) = delete;
    Context& operator=(const Context&) = delete;
    Context(Context&&) = delete;
    Context& operator=(Context&&) = delete;

protected:
    // Construction only allowed by Derived
    Context() = default;
    ~Context() = default;
};

class DefaultContext : public Context<DefaultContext>
{
};

} // namespace pep