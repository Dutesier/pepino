
# Pepino

**Pepino** is a Gherkin feature file interpreter and test generator for C++ using the Google Test (gtest) framework. Inspired by the "Crafting Interpreters" book by Robert Nystrom, Pepino is a project that parses `.feature` files written in Gherkin syntax and generates corresponding C++ test cases using gtest. This allows developers to write behavior-driven tests for their C++ codebase in a highly readable, structured format.

## Features

- **Parse Gherkin Feature Files**: Reads `.feature` files written in Gherkin syntax.
- **Generate gtest-Compatible Code**: Produces C++ code compatible with the Google Test framework.
- **Supports Gherkin Syntax**: Handles core Gherkin constructs such as `Feature`, `Scenario`, `Given`, `When`, `Then`, `And`, and `Scenario Outline` with `Examples`.
- **Example-Driven Tests**: Supports parameterized tests via `Scenario Outline` and `Examples` tables for varied test cases.
- **Reusable Test Scenarios**: Use `Background` sections for shared setup across multiple scenarios.

## Table of Contents

- [Getting Started](#getting-started)
- [Installation](#installation)
- [Usage](#usage)
- [Supported Syntax](#supported-syntax)
- [Example](#example)
- [Contributing](#contributing)
- [License](#license)

## Getting Started

Pepino translates behavior specifications into executable C++ tests, making it easier to manage BDD-style test scenarios. The main workflow involves creating Gherkin feature files, running Pepino to generate C++ test files, and then compiling and running the tests with gtest.

### Prerequisites

- **C++20 or newer** for generated code compatibility.
- **Google Test (gtest)** library installed for test compilation and execution.
- **CMake (optional)** if using CMake to build the generated tests.

## Installation

Clone the repository:

```bash
git clone https://github.com/dutesier/pepino.git
cd pepino
```

Then build the project:

```bash
mkdir build
cd build
cmake ..
make
```

## Usage

1. **Write Gherkin Feature Files**: Create `.feature` files that describe the desired behavior in Gherkin syntax.
2. **Run Pepino**: Use Pepino to parse the feature files and generate corresponding C++ test files.

```bash
./pepino path/to/your.feature
```

This will generate a `.cpp` file containing the gtest-compatible C++ code.

3. **Compile and Run Tests**: Use gtest to compile and run the generated tests.

```bash
g++ generated_test.cpp -lgtest -lgtest_main -pthread -o test
./test
```

## Supported Syntax

Pepino supports the following Gherkin syntax constructs:

- `Feature`: Top-level description of the feature under test.
- `Scenario`: Describes a specific test case within the feature.
- `Given`: Specifies preconditions.
- `When`: Defines actions performed.
- `Then`: Outlines expected outcomes.
- `And`: Adds additional steps to `Given`, `When`, or `Then`.
- `Scenario Outline` with `Examples`: Parameterized scenarios for running the same test logic with different inputs.
- `Background`: Provides shared setup for all scenarios in a feature.

For detailed BNF of the supported Gherkin syntax, refer to `BNF.md`.

## Example

### Sample `.feature` file

```gherkin
Feature: Login functionality

  Background:
    Given a user exists with username "user" and password "pass"

  Scenario: Successful login
    Given the user is on the login page
    When the user enters valid credentials
    Then they should be redirected to the dashboard

  Scenario Outline: Unsuccessful login attempts
    Given the user is on the login page
    When the user enters <username> and <password>
    Then they should see an error message

    Examples:
      | username | password |
      | user     | wrongpass |
      | unknown  | pass      |
```

### Generated C++ Test File (example output)

```cpp
#include <gtest/gtest.h>

TEST(LoginFunctionality, SuccessfulLogin) {
    // Given a user exists with username "user" and password "pass"
    // the user is on the login page
    // When the user enters valid credentials
    // Then they should be redirected to the dashboard
}

TEST(LoginFunctionality, UnsuccessfulLoginAttempts) {
    // Scenario outline for multiple examples
    struct Example { std::string username; std::string password; };
    std::vector<Example> examples = {
        {"user", "wrongpass"},
        {"unknown", "pass"}
    };

    for (const auto& example : examples) {
        // Given the user is on the login page
        // When the user enters example.username and example.password
        // Then they should see an error message
    }
}
```

## Contributing

Contributions are welcome! If you'd like to contribute, please fork the repository and submit a pull request with your changes.

1. Fork the Project
2. Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3. Commit your Changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the Branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

## License

Distributed under the MIT License. See `LICENSE` for more information.

---

Pepino is a tool for developers who want to bring the benefits of BDD into their C++ projects using Google Test. Happy testing!
