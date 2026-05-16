# Contributing to PuttyAlt

Thank you for your interest in contributing to PuttyAlt! This document provides guidelines for contributing.

## How to Contribute

### Reporting Bugs

- Use the [Bug Report](https://github.com/chillymasterio/puttyalt/issues/new?template=bug_report.yml) template
- Include your OS version, PuttyAlt version, and steps to reproduce
- Attach logs if applicable (Help > Event Log)

### Suggesting Features

- Use the [Feature Request](https://github.com/chillymasterio/puttyalt/issues/new?template=feature_request.yml) template
- Check existing issues and the [Roadmap](README.md#roadmap) first

### Submitting Code

1. **Fork** the repository
2. **Create a branch** from `main`:
   ```bash
   git checkout -b feature/short-description
   ```
3. **Write your code** following the style guidelines below
4. **Test** your changes — at minimum, ensure PuttyAlt builds and connects via SSH
5. **Commit** with clear messages:
   ```
   Add tabbed interface with Ctrl+T shortcut
   
   Implements tab control in the main window using Win32 Tab Common Control.
   Each tab runs an independent terminal session.
   ```
6. **Push** and open a **Pull Request**

## Code Style

PuttyAlt v0.2.0 follows PuTTY's existing coding style:

- **Language:** C99
- **Indentation:** 4 spaces (no tabs)
- **Braces:** Opening brace on the same line
- **Naming:** `snake_case` for functions and variables
- **Comments:** C-style `/* */` preferred for multi-line
- **Line length:** Keep reasonable, ~80 chars preferred

### Platform Abstraction

- Platform-independent code goes in the root or `utils/`
- Windows-specific code goes in `windows/`
- Unix-specific code goes in `unix/`
- Use the abstractions in `putty.h`, `storage.h`, `network.h`

## Building

```bash
cmake -B build
cmake --build build --config Release
```

## Security

- Never commit credentials or private keys
- Follow secure coding practices — avoid buffer overflows, use `smemclr()` for sensitive data
- Security-critical changes require extra review

## License

By contributing, you agree that your contributions will be licensed under the MIT License.
