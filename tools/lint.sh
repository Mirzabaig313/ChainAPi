#!/usr/bin/env bash
# Runs clang-tidy across the repo. Expects a build directory with
# compile_commands.json (e.g. build/macos-debug). Override via $BUILD_DIR.
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BUILD_DIR="${BUILD_DIR:-$ROOT/build/macos-debug}"

if [[ ! -f "$BUILD_DIR/compile_commands.json" ]]; then
    echo "compile_commands.json not found in $BUILD_DIR" >&2
    echo "Configure the project first: cmake --preset macos-debug" >&2
    exit 1
fi

if ! command -v clang-tidy >/dev/null 2>&1; then
    echo "clang-tidy not found in PATH" >&2
    exit 1
fi

find "$ROOT/engine" "$ROOT/cli" "$ROOT/desktop" \
    \( -name '*.cpp' -o -name '*.cc' \) \
    -not -path '*/build/*' \
    -print0 \
    | xargs -0 clang-tidy -p "$BUILD_DIR"

echo "Lint clean."
