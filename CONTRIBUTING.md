# Contributing to PuttyAlt

Thanks for your interest in contributing!

## Development Setup

```bash
git clone https://github.com/chillymasterio/puttyalt.git
cd puttyalt
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```

## Code Style

- C99 with POSIX extensions
- 4-space indentation, no tabs
- Braces on same line for control flow, next line for functions
- All public API prefixed with module name (e.g., `tabs_close()`)
- Include guards: `PUTTYALT_MODULENAME_H`

## Commit Messages

- Imperative mood ("Add feature" not "Added feature")
- First line: 50-72 characters
- Reference issues where applicable

## Pull Requests

1. Fork and create a feature branch
2. Write clean, tested code
3. Update CHANGELOG.md if adding features or fixing bugs
4. Ensure CI passes
5. Submit PR with clear description

## Module Structure

Each feature is a self-contained module:
- `puttyalt_foo.h` — public header with types and API
- `puttyalt_foo.c` — implementation
- Register in `CMakeLists.txt`
- Add feature flag in `puttyalt.h`
