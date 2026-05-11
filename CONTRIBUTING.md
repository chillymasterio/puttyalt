# Contributing to PuttyAlt

## Development Setup

```bash
git clone https://github.com/chillymasterio/puttyalt.git
cd puttyalt
mkdir build && cd build
cmake .. -DCMAKE_C_FLAGS="-Wall -Wextra -Wpedantic"
make -j$(nproc)
```

## Code Style

- C99 standard, 4-space indent
- `puttyalt_` prefix for all module files
- Header guards: `#ifndef PUTTYALT_MODULENAME_H`
- Cross-platform: `#ifdef _WIN32` for Windows-specific code
- Unix stubs for all Windows APIs

## Pull Request Guidelines

1. One feature/fix per PR
2. Update `CHANGELOG.md` with your changes
3. Ensure `cmake .. && make` builds clean on Linux
4. Add module to `CMakeLists.txt` if new files
5. Add feature flag to `puttyalt.h` if new feature

## Module Structure

Each module consists of:
- `puttyalt_module.h` — public API and data types
- `puttyalt_module.c` — implementation with platform guards

## Testing

```bash
./build/puttyalt --version   # verify build
./build/puttyalt --help      # check CLI
```
