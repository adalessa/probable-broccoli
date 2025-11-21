# AGENTS.md

## Build, Lint, and Test Commands
- Build: `platformio run`
- Upload: `platformio run --target upload`
- Test: `platformio test` (if tests exist)
- Run single test: `platformio test --filter <test_name>`
- Lint: Use `clang-tidy` with `.clang-tidy` config

## Code Style Guidelines
- Language: C++17 (`-std=c++17`)
- Header guards: Use `#pragma once`
- Imports: Local headers with quotes, external with angle brackets
- Class names: PascalCase (e.g., `App`, `LcdApi`)
- Member variables: Lowercase, no underscores, pointers with `*`
- Functions: PascalCase for classes, camelCase for variables
- Indentation: 2 spaces, braces on same line
- Short statements may omit braces (see `.clang-tidy`)
- Error handling: Use conditional checks, avoid exceptions
- Types: Prefer references for non-owning, pointers for optional
- Use `.clang-tidy` and `.clangd` for linting and completion
- No Cursor or Copilot rules present

> Agents should follow these conventions for all code contributions.
