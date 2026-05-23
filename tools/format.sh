#!/usr/bin/env bash
# Formats all C++ sources under engine/, cli/, desktop/, and ipc/.
# Honors .clang-format at the repo root.
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"

if ! command -v clang-format >/dev/null 2>&1; then
    echo "clang-format not found in PATH" >&2
    exit 1
fi

find "$ROOT/engine" "$ROOT/cli" "$ROOT/desktop" "$ROOT/ipc" \
    \( -name '*.h' -o -name '*.hpp' -o -name '*.cpp' -o -name '*.cc' \) \
    -not -path '*/build/*' \
    -print0 \
    | xargs -0 clang-format -i

echo "Formatted."
