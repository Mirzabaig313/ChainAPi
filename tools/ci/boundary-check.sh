#!/usr/bin/env bash
# Header-include leak check for the engine and CLI layers.
# Implements Project Layout §4.3.
#
# Fails fast if an engine or CLI source file includes any Qt UI header
# (Widgets, Gui, Quick) or QScintilla. CMake-time link checks catch
# direct linkage; this catches `#include` regressions even when the file
# does not yet link.
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
violations=0

check_dir() {
    local dir="$1"
    local label="$2"
    if [[ ! -d "$dir" ]]; then
        return 0
    fi

    local hits
    hits=$(grep -rEln '^[[:space:]]*#[[:space:]]*include[[:space:]]*[<"](Q(Widget|Window|Application|Quick|Gui)|QtWidgets|QtGui|QtQuick|Qsci)' \
        "$dir" || true)

    if [[ -n "$hits" ]]; then
        echo "::error::$label includes forbidden Qt UI / QScintilla headers:"
        echo "$hits" | sed 's/^/  /'
        violations=1
    fi
}

check_dir "$ROOT/engine/src"     "engine/src"
check_dir "$ROOT/engine/include" "engine/include"
check_dir "$ROOT/cli/src"        "cli/src"

if [[ "$violations" -ne 0 ]]; then
    echo
    echo "Engine and CLI must not include Qt UI headers (PRD §8.6)." >&2
    exit 1
fi

echo "Boundary check passed."
