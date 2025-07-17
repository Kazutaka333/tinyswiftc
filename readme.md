# TinySwiftC

TinySwiftC is a minimal, educational Swift-like compiler written in C++ using LLVM. This project demonstrates the core concepts of compiler construction, including lexing, parsing, AST generation, semantic analysis, and LLVM IR code generation. It is designed as a side project to showcase my skills and passion for compiler engineering.

## Features

- **Custom Lexer and Parser:** Tokenizes and parses a Swift-inspired language syntax.
- **AST Construction:** Builds an abstract syntax tree for further analysis and code generation.
- **Semantic Checks:** Performs basic type and return value checks.
- **LLVM IR Generation:** Translates the AST into LLVM IR for optimization and code emission.
- **Object File and Executable Generation:** Uses LLVM and Clang to produce native executables.
- **Debug Logging:** Optional debug output for tracing compilation steps (`--debug` flag).
- **Automated Testing:** Includes a test script for validating compiler output.

## Example

Input (`example.swift`):

```swift
func main() -> Int {
    return 42
}
```

Compile and run:

```sh
make
./build/tinyswiftc example.swift
clang build/output.o -o build/output
./build/output
echo $?   # Should print 42
```

## Getting Started

### Prerequisites

- C++17 compiler (tested with Clang and GCC)
- [LLVM](https://llvm.org/) (install via Homebrew: `brew install llvm` on macOS)
- CMake and Make
- Bash (for running `test.sh`)

### Build Instructions

```sh
make
```

### Run Tests

```sh
./test.sh
```

To enable debug logging:

```sh
./test.sh --debug
```

## Project Structure

```
tinyswiftc/
├── AST/                # AST node definitions
├── Lexer/              # Lexer implementation
├── Parser/             # Parser implementation
├── Util/               # Utility code (debug logging, etc.)
├── main.cpp            # Compiler entry point
├── Makefile
├── test.sh             # Test runner
├── example.swift       # Example input
└── readme.md
```

## Why I Built This

I created TinySwiftC to deepen my understanding of compiler internals and LLVM, and to demonstrate my ability to design and implement a non-trivial systems project from scratch. Every component, from the lexer to code generation, is written by me and reflects my approach to clean, maintainable, and well-documented code.

## What I Learned

- Practical use of LLVM APIs for code generation and optimization
- Design patterns for ASTs and recursive descent parsing
- Debugging and testing compiler frontends and backends
- Build automation and cross-platform development

## Contact

If you’d like to discuss compilers, LLVM, or have opportunities for a passionate compiler engineer, please reach out via [Linkedin](https://ca.linkedin.com/in/kazhomma/en) or email (hommakazutaka at gmail.com)