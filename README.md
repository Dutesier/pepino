
# 🥒 Pepino

**Pepino** is a modern C++20 library for delayed step execution, behavior-driven testing, and structured feature interpretation—powered by Gherkin-style `.feature` files and a rich macro-based runtime.

At its core, Pepino is a **delayed invocation engine**: it parses Gherkin steps, binds them to regular-expression based callbacks, injects a shared runtime context, and runs scenarios in the correct sequence—automatically.

---

## ✨ Features

- ✅ Plaintext `.feature` file support (Gherkin syntax)
- ✅ Step registration with `GIVEN`, `WHEN`, `THEN` macros
- ✅ Shared mutable `Context` between steps
- ✅ Type-erased, safe, and customizable step dispatch
- ✅ Automatic scenario/background/examples resolution
- ✅ Hooks for `BEFORE_ALL`, `AFTER_STEP`, and more
- ✅ Lightweight and dependency-free (pure C++20 headers)

---

## 🧠 What is Gherkin?

**Gherkin** is a human-readable DSL for describing behavior. For example:

```gherkin
Feature: User login

  Scenario: Successful login
    Given a user exists with username "alice" and password "secret"
    When the user logs in
    Then the login should succeed
```

Pepino reads `.feature` files like these and matches each step to a registered C++ callback using regular expressions.

---

## 🚀 Quickstart

### 1. Create a `.feature` File

```gherkin
Feature: Login

  Scenario: Valid login
    Given a user "alice" with password "secret"
    When they login with "alice" and "secret"
    Then login should be successful
```

### 2. Register Step Handlers

```cpp
#include "pepino/steps/steps.h"

GIVEN("^a user exists with username \"(\\w+)\" and password \"(\\w+)\"$",
      [](pep::DefaultContext&, std::string username, std::string password) {
          std::cout << "Created user: " << username << "/" << password << std::endl;
      });

WHEN("the user enters valid credentials", [](pep::DefaultContext&) {
    std::cout << "Entered valid credentials\n";
});

THEN("they should be redirected to the dashboard", [](pep::DefaultContext&) {
    std::cout << "Redirected to dashboard\n";
});
```

### 3. Add Optional Hooks

```cpp
#include "pepino/HookMacros.h"

BEFORE_ALL() {
  std::cout << "Running all scenarios...
";
}

AFTER_STEP() {
  std::cout << "Step finished.
";
}
```

### 4. Run a Feature

```cpp
#include "pepino/Lexer.h"
#include "pepino/Parser.h"
#include "pepino/TestRunner.h"

int main() {
  return pep::run("pathToFeatureFile.feature");
}
```

### Alternatively, you can setup your own context
For stateful steps and validation, define a custom context class.

```cpp
class MyContext : public pep::DefaultContext<MyContext> {
public:
    std::string name{};
    int number{};
    testing::MockFunction<void()> mockCb;
};

// Step callbacks use MyContext directly:
GIVEN_CTX(MyContext, "^a number (\\d+)$", [](MyContext& ctx, int x) {
    std::cout << "Got number: " << x << std::endl;
    ctx.number = x;
    ctx.mockCb.AsStdFunction()(); // for test verification
});

GIVEN_CTX(MyContext, "^a name (\\w+)$", [](MyContext& ctx, std::string name) {
    ctx.name = std::move(name);
    std::cout << "Got name: " << ctx.name << std::endl;
});

THEN_CTX(MyContext, "^the name should be (\\w+)$", [](MyContext& ctx, std::string expected) {
    if (ctx.name != expected) {
        throw std::runtime_error("Name mismatch!");
    }
});
```

---

## 🧩 Architecture Overview

Pepino is organized around:

- **Lexer / Parser** — Converts `.feature` files into an AST
- **StepRegistry** — Binds regex patterns to callbacks
- **Context** — Shared state across steps, type-erased
- **TestRunner** — Executes scenarios in Gherkin order
- **HookRegistry** — Manages lifecycle callbacks like `BEFORE_ALL`

All of this is **delayed execution**: steps are not run when declared—they’re invoked later by the `TestRunner`.

---

## 📦 Integration

- **Header-only**: Just add `include/pepino/` to your project.
- **Source files**: Add `src/*.cpp` to your build system.
- **No runtime dependencies**: Zero 3rd-party libs required.
- **C++20**: Requires a C++20-compatible compiler.

---


## 🛠 Contributing

- Write scenarios, features, and parsers.
- Extend the AST or the `TestRunner` for new Gherkin capabilities.
- Submit issues and PRs to improve performance or correctness.

---

## 🧾 License

MIT License © 2025 Duarte Reis & contributors.
