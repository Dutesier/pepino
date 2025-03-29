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

#include "TestController.h"

#include "parsing/Lexer.h"
#include "parsing/Parser.h"
#include "parsing/Statement.h"

#include <fstream>

namespace pep
{

TestController::TestController(std::unique_ptr<ITestRunner> runner)
    : testRunner(std::move(runner))
{
}

int TestController::executeTest(const std::string& input)
{
    // Read the file content
    std::ifstream file(input);
    if (!file.is_open())
    {
        throw std::runtime_error("Could not open file: " + input);
    }
    std::string content((std::istreambuf_iterator<char>(file)),
                        std::istreambuf_iterator<char>());
    file.close();
    // Tokenize the content
    Lexer lexer(content);
    std::vector<Token> tokens = lexer.tokenize();
    // Parse the tokens into a feature statement
    Parser parser(tokens);
    std::unique_ptr<FeatureStatement> feature = parser.parseFeature();
    if (!feature)
    {
        throw std::runtime_error("Failed to parse feature from file: " + input);
    }

    return testRunner->runTests(std::move(feature));
}

} // namespace pep