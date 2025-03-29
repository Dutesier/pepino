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

#include "Statement.h"
#include "Token.h"

#include <memory>
#include <string>
#include <vector>

namespace pep
{

class Parser
{
public:
    explicit Parser(const std::vector<Token>& tokens);
    std::unique_ptr<FeatureStatement> parseFeature();

private:
    std::unique_ptr<FeatureStatement> parseFeatureStatement();
    std::unique_ptr<BackgroundStatement> parseBackgroundStatement();
    std::unique_ptr<ScenarioStatement> parseScenarioStatement();
    std::unique_ptr<ScenarioOutlineStatement> parseScenarioOutlineStatement();
    std::unique_ptr<ExamplesStatement> parseExamplesStatement();
    std::unique_ptr<StepStatement> parseStepStatement();

    bool isAtEnd() const;
    const Token& peek() const;
    const Token& previous() const;
    const Token& advance();
    void advanceEmptyLines();
    std::string consumeLiteralUntilEOL();
    bool match(TokenType type);
    const Token& consume(TokenType type, const std::string& message);
    std::vector<std::string> parseTags();
    std::vector<std::string> parseTableRow();

    const std::vector<Token>& m_tokens;
    size_t m_current = 0;
};

} // namespace pep