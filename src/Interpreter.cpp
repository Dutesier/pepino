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

#include "Interpreter.h"

#include "AstPrinter.hpp" // Debugging
#include "BaseExpression.h"
#include "BaseStatement.h"
#include "Environment.h"
#include "Logger.h"
#include "Object.h"
#include "Token.h"

#include <assert.h>
#include <chrono>
#include <format>
#include <fstream>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <typeinfo>
#include <unistd.h>
#include <variant>
#include <vector>

namespace pep
{

namespace
{

// // Follows Pepino (Ruby)s convention
// bool isTruthy(const Object& value)
// {
//     if (std::holds_alternative<bool>(value))
//     {
//         return std::get<bool>(value);
//     }
//     if (std::holds_alternative<NullLiteral>(value))
//     {
//         return false;
//     }
//     return true;
// }

// bool isEqual(const Object& a, const Object& b)
// {
//     if (std::holds_alternative<bool>(a) && std::holds_alternative<bool>(b))
//     {
//         return std::get<bool>(a) == std::get<bool>(b);
//     }
//     if (std::holds_alternative<std::string>(a) && std::holds_alternative<std::string>(b))
//     {
//         return std::get<std::string>(a) == std::get<std::string>(b);
//     }

//     if (std::holds_alternative<double>(a) && std::holds_alternative<double>(b))
//     {
//         return std::get<double>(a) == std::get<double>(b);
//     }

//     if (std::holds_alternative<NullLiteral>(a) || std::holds_alternative<NullLiteral>(b))
//     {
//         if (std::holds_alternative<NullLiteral>(a) && std::holds_alternative<NullLiteral>(b))
//         {
//             return true;
//         }
//         return false;
//     }
//     return false;
// }

template <typename T> void assertBothAreType(const Token& tok, const Object& a, const Object& b)
{
    if (std::holds_alternative<T>(a) && std::holds_alternative<T>(b))
    {
        return;
    }

    T debug;
    throw Interpreter::InterpreterException{
        tok, "variables do not hold the same type " + std::string{ typeid(debug).name() }
    };
}

} // namespace

Interpreter::Interpreter()
{
    defineNativeFunctions();
}

Interpreter::Interpreter(std::filesystem::path path)
    : m_path(std::move(path))
{
    if (m_path->empty())
    {
        throw std::domain_error("Invalid file path.");
    }
    defineNativeFunctions();
}

void Interpreter::defineNativeFunctions()
{
    Callable clock{ .call = [](Interpreter&, const std::vector<Object>&) -> Object {
                       return static_cast<double>(
                           std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));
                   },
                    .name = "clock" };

    Callable sleep{ .call = [](Interpreter&, const std::vector<Object>& args) -> Object
                    {
                        if (args.empty())
                        {
                            throw InterpreterException{ Token{},
                                                        "Sleep requires one argument" }; // This should never be called
                        }

                        if (!std::holds_alternative<double>(args[0]))
                        {
                            throw InterpreterException{ Token{}, "Sleep requires a double as argument" };
                        }
                        ::sleep(std::get<double>(args[0]));
                        return args[0];
                    },
                    .arity = 1,
                    .name = "sleep" };

    // m_globalEnvironment->define(clock.name, clock);
    // m_globalEnvironment->define(sleep.name, sleep);
}

int Interpreter::run()
{
    if (m_path)
    {
        return interpretFile();
    }
    return interpretStdin();
}

int Interpreter::interpretFile()
{
    assert(m_path.has_value());
    std::ifstream file(m_path.value());
    if (!file.is_open())
    {
        m_logger.error(std::format("[interpretFile]: Failed to open file at {}.", m_path.value().string()));
        return EXIT_FAILURE;
    }

    std::string lineBuffer, wholeFile;
    while (getline(file, lineBuffer))
    {
        wholeFile += lineBuffer + '\n';
    }

    file.close();
    return interpret(wholeFile);
}

int Interpreter::interpretStdin()
{
    auto exitCode = EXIT_FAILURE;
    std::string lineBuffer{};
    Logger::terminal("");
    while (getline(std::cin, lineBuffer))
    {
        exitCode = interpret(lineBuffer);
        Logger::terminal("");
    }

    return exitCode;
}

int Interpreter::interpret(const std::string& content)
{
    if (content.empty())
    {
        m_logger.info("[interpret]: No content to interpret.");
        return EXIT_FAILURE;
    }

    m_logger.debug(std::format("[interpret]: Content: {}", content));
    std::string_view content_view{ content };
    m_lexer = std::make_unique<Lexer>(std::move(content_view));
    m_parser = std::make_unique<Parser>(m_lexer->tokenize());
    auto statements = m_parser->parse();

    m_stepRegistry.debug_runAll();
    // if (statements.empty())
    // {
    //     return EXIT_FAILURE;
    // }

    // AstPrinter printer;

    // m_env = m_globalEnvironment;
    // for (const auto& stmt : statements)
    // {
    //     try
    //     {
    //         stmt->accept(*this);
    //     }
    //     catch (InterpreterException& e)
    //     {
    //         Logger::error("Runtime error: " + std::string{ e.what() });
    //         return EXIT_FAILURE;
    //     }
    //     catch (Environment::EnvironmentException& e)
    //     {
    //         Logger::error("Runtime error: " + std::string{ e.what() });
    //         return EXIT_FAILURE;
    //     }
    // }

    return EXIT_SUCCESS;
}

Object Interpreter::visit(const LiteralExpression& expr)
{
    (void)expr;
    return {};
}

Object Interpreter::visit(const PlaceholderExpression& expr)
{
    (void)expr;
    return {};
}

void visit(const FeatureStatement& stmt)
{
    (void)stmt;
    return;
}

void visit(const BackgroundStatement& stmt)
{
    (void)stmt;
    return;
}

void visit(const ScenarioStatement& stmt)
{
    (void)stmt;
    return;
}

void visit(const StepStatement& stmt)
{
    (void)stmt;
    return;
}

void visit(const ScenarioOutlineStatement& stmt)
{
    (void)stmt;
    return;
}

void visit(const ExamplesStatement& stmt)
{
    (void)stmt;
    return;
}

void visit(const TagStatement& stmt)
{
    (void)stmt;
    return;
}

void Interpreter::logError(unsigned int line, std::string_view location, std::string_view message)
{
    m_logger.error(std::format("[line {}] {}: {}", line, location, message));
}

Object Interpreter::evaluate(const Expression& expr)
{
    return expr.accept(*this);
}

} // namespace pep
