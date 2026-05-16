#!/bin/bash
# Build PuttyAlt from source (Linux/MinGW cross-compile)
set -e

BUILD_DIR="${1:-build}"

echo "=== PuttyAlt Build ==="
echo "Build directory: $BUILD_DIR"

cmake -B "$BUILD_DIR" -DCMAKE_BUILD_TYPE=Release
cmake --build "$BUILD_DIR" --config Release -j$(nproc)

echo "=== Build complete ==="
echo "Binaries in: $BUILD_DIR"
