
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

### 2. Define a Context

```cpp
// MyContext.h
#pragma once
#include "pepino/ContextRegistry.h"

struct MyContext : pep::BaseContext {
  std::string username;
  int attempts = 0;
};

template <>
inline std::unique_ptr<pep::BaseContext> pep::ContextRegistry::makeContext() {
  return std::make_unique<MyContext>();
}
```

### 3. Register Step Handlers

```cpp
#include "pepino/StepMacros.h"

GIVEN(R"(^a user "([^"]+)" with password "([^"]+)"$)", [](MyContext& ctx, const auto& caps) {
  ctx.username = caps[0];
});

WHEN(R"(^they login with "([^"]+)" and "([^"]+)"$)", [](MyContext& ctx, const auto& caps) {
  ctx.attempts++;
});

THEN(R"(^login should be successful$)", [](MyContext& ctx, const auto&) {
  if (ctx.username != "alice") throw std::runtime_error("Login failed");
});
```

### 4. Add Optional Hooks

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

### 5. Run a Feature

```cpp
#include "pepino/Lexer.h"
#include "pepino/Parser.h"
#include "pepino/TestRunner.h"

int main() {
  std::string featureText = /* load from .feature file */;
  auto tokens  = pep::Lexer(featureText).tokenize();
  auto feature = pep::Parser(tokens).parseFeature();
  pep::TestRunner().runFeature(*feature);
}
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

## ✅ Example

```cpp
GIVEN(R"(^something happened$)", [](MyContext& ctx, auto&) {
  ctx.attempts++;
});

THEN(R"(^the count should be (\d+)$)", [](MyContext& ctx, const auto& caps) {
  int expected = std::stoi(caps[0]);
  if (ctx.attempts != expected)
    throw std::runtime_error("Count mismatch");
});
```

---

## 🛠 Contributing

- Write scenarios, features, and parsers.
- Extend the AST or the `TestRunner` for new Gherkin capabilities.
- Submit issues and PRs to improve performance or correctness.

---

## 🧾 License

MIT License © 2025 Duarte Reis & contributors.
