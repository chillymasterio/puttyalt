# Contributing to PuttyAlt

## Building from Source

```bash
git clone https://github.com/chillymasterio/puttyalt.git
cd puttyalt

# Linux native build
mkdir build && cd build
cmake .. && make -j$(nproc)

# Windows cross-compile (MinGW)
mkdir build-win && cd build-win
cmake .. -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchain-mingw.cmake
make -j$(nproc)
```

## Code Style

- C99 standard
- 4-space indentation
- `snake_case` for functions and variables
- `UPPER_CASE` for macros and constants
- Prefix all public symbols with module name (e.g., `term_init`, `sel_start`)
- Guard all headers with `#ifndef PUTTYALT_MODULE_H`

## Module Structure

Each module consists of:
- `puttyalt_module.h` — Public API, types, constants
- `puttyalt_module.c` — Implementation
- `#ifdef _WIN32` for platform-specific code, stubs for Unix

## Pull Requests

1. Fork the repo
2. Create a feature branch
3. Write clean, tested code
4. Submit a PR with a clear description
