# AGENTS.md — ChainAPI

Project-level instructions for AI coding agents (Kiro, Codex, Claude Code, Cursor, etc.). Keep terse. The full product spec lives in `doc/`.

## Context

**ChainAPI** is a workflow-aware API testing tool. Treats an API as a graph of resources, actors, and dependencies; auto-resolves request chains. Three deliverables:

- **`engine/`** — `libchainapi-engine`, a pure C++ engine library. **No Qt UI dependency.**
- **`cli/`** — `chainapi` CLI binary. Links engine + `Qt6::Core` only.
- **`desktop/`** — Qt 6 desktop UI app. The *only* place Qt UI libraries are allowed.
- **`ipc/`** — Phase B scaffold (currently empty).

Read [`doc/ChainAPI - Project Layout.md`](doc/ChainAPI%20-%20Project%20Layout.md) before making structural changes.

## Stack Baseline

| Tool | Pinned to |
|---|---|
| CMake | **4.0** minimum |
| C++ standard | **C++23** |
| Qt | **6.8 LTS** minimum |
| Compiler | Apple Clang 16+, Clang 18+, GCC 14+, MSVC 19.40+ |
| Dependency manager | **vcpkg** (manifest mode, `master` baseline) |
| Test framework | GoogleTest |
| CI | GitHub Actions, three OS × Debug/Release matrix |

C++26 is not portable yet; do not use C++26-only features (reflection, contracts, `std::execution`) without feature-test gating. Use C++23 idioms (`std::expected`, `std::print`, ranges additions, `deducing this`) freely — they are stable on all supported compilers.

## Architectural Boundary (Hard Rule)

The engine layer must not directly or transitively depend on Qt UI or the QScintilla code editor. Enforced in three places:

1. **CMake link guard** — `cmake/ChainApiBoundaryGuards.cmake` fails configure if the engine, CLI, or engine tests link `Qt6::Widgets`, `Qt6::Gui`, `Qt6::Quick`, `Qt6::QuickWidgets`, or `QScintilla::QScintilla`.
2. **CI grep job** — `.github/workflows/boundary-check.yml` rejects any `#include <QWidget>` / `<QApplication>` / `<Qsci...>` under `engine/` or `cli/`.
3. **Public header surface** — `engine/include/chainapi/engine/*.h` uses pImpl + value types. No Qt UI types appear.

If you need to expose engine state to the desktop UI, do it via `engine/include/chainapi/engine/Events.h` callbacks or expand `PublicApi.h`. Do not add a `chainapi-ui-shared` target that imports both worlds.

## Build & Test Commands

```bash
# Configure & build (macOS Debug with ASan + UBSan)
cmake --preset macos-debug
cmake --build --preset macos-debug

# Run all tests
ctest --preset macos-debug --output-on-failure

# Engine tests only (fastest)
ctest --preset macos-debug --label-regex engine

# Run the CLI
./build/macos-debug/cli/chainapi --help

# Run the desktop app
./build/macos-debug/desktop/ChainAPI.app/Contents/MacOS/ChainAPI
```

Other presets: `macos-release`, `linux-debug`, `linux-release`, `windows-debug`, `windows-release`. Set `$VCPKG_ROOT` before configuring.

## Code Style

- **Formatter**: `clang-format` per `.clang-format`. Run `tools/format.sh` before committing.
- **Lint**: `clang-tidy` per `.clang-tidy`.
- **Warnings**: `-Wall -Wextra -Wpedantic -Wconversion`, applied via `chainapi_set_warnings()`. `-Werror` is gated behind CI, not local builds.
- **File size**: ≤ 800 lines; ≤ 50 lines per function. Split rather than scroll.
- **Headers**: forward-declare in `.h`, include in `.cpp`. One class per public header.
- **Error handling**: prefer `std::expected<T, E>` (C++23) over error-code globals or out-parameters. Reserve exceptions for genuinely unrecoverable conditions; never throw across ABI boundaries or in destructors.

## Naming Conventions

- Files: `PascalCase.{h,cpp}` (matches Qt convention used in this repo).
- Types: `PascalCase`. Functions and methods: `camelCase`. Constants: `kCamelCase` or `SCREAMING_SNAKE_CASE` for macros only.
- Namespace: `chainapi::engine` for engine types, `chainapi::cli`, `chainapi::desktop` for app code.
- CMake targets: `chainapi-<layer>` (`chainapi-engine`, `chainapi-cli`, `chainapi-desktop`). Aliases: `chainapi::engine`.

## Layer Rules (Clean Architecture)

```
domain/        ← pure business logic, no I/O. Allowed deps: stdlib + Qt6::Core (for QString only).
application/   ← use cases. Allowed deps: domain + infrastructure (privately).
infrastructure/← I/O adapters (HTTP, SQLite, secrets, schema). Concrete impls behind interfaces.
```

Public engine headers (`engine/include/chainapi/engine/`) contain only domain + application surfaces. Infrastructure types are private.

## Testing Expectations

- New code lands with tests. Domain-layer tests aim for 90%+ coverage.
- Use real dependencies via test fixtures (sqlite in-memory, mock SUT under `tests/fixtures/mock-sut/`). Never mock the database.
- Each test independent. No shared mutable state across tests.
- Tests live next to the code: `engine/tests/unit/`, `engine/tests/integration/`.

Add a corresponding test for every bug fix that reproduces the failure first.

## Git & PRs

- Conventional commits: `feat:`, `fix:`, `refactor:`, `docs:`, `test:`, `chore:`, `perf:`, `ci:`. Subject ≤ 70 chars.
- Branch from `main`. Push to a feature branch. Open a PR; never push directly to `main`.
- PR description must list what was tested. Failing CI blocks merge.
- Do not commit secrets, build outputs (`build/`), or vcpkg installed trees. `.gitignore` already covers these — verify your staging area before committing.

## Available Agents (Kiro / Compatible Harnesses)

When working on this codebase, prefer these specialised agents over generic flow:

- **`/cpp-reviewer`** — UB, ownership, concurrency, and modern C++ review. Run after non-trivial engine or CLI changes.
- **`/cpp-build-resolver`** — minimal-diff fixes for CMake / compile / link / sanitizer errors.
- **`/qt-reviewer`** — desktop-only. QObject lifetime, signal/slot correctness, QML/C++ bridge.
- **`/security-reviewer`** — input validation, deserialization, secret handling. Use when touching the import path, hook runner, or secret store.
- **`/vuln-validator`** — adversarial review of an existing security finding. Confirms or refutes; cannot file new findings.
- **`/reachability-tracer`** — given a confirmed bug, walks taint backward to decide if attacker-controlled input can reach it.
- **`/planner`** — break down complex changes before coding. Use for any multi-file feature.

Skills:

- **`/cpp-patterns`** — modern C++ idioms reference.
- **`/qt-patterns`** — Qt 6.8 idioms and CMake integration.
- **`/hunt`** — narrow-scope vulnerability hunting (one attack class × one component).

## Things to Avoid

- Adding a `find_package(Qt6 ...)` call for `Widgets`, `Gui`, or `Quick` outside `desktop/CMakeLists.txt`.
- `#include <QWidget>` (or any UI-namespaced Qt header) under `engine/` or `cli/`.
- `file(GLOB ...)` for source lists in any `CMakeLists.txt`.
- Bumping the public API surface without updating `engine/include/chainapi/engine/PublicApi.h`.
- Using `using namespace std;` or `using namespace chainapi;` at namespace or header scope.
- Catching exceptions just to log and rethrow without context. Use `std::expected` in new code.
- Adding agent-generated tests that don't actually exercise the change. Each test must fail without the code under test.

## Things Worth Doing Proactively

- Use `std::expected<T, ChainApiError>` for new error paths in the engine application layer.
- Use `std::print` / `std::println` instead of `iostream` chains for new code.
- Use C++23 ranges additions (`views::zip`, `views::enumerate`, `ranges::fold_left`, `ranges::to`) where they replace explicit loops.
- When adding a new infrastructure adapter, define the pure interface in `engine/src/infrastructure/<area>/<Name>.h` first, then the concrete `.cpp`.
- Add the matching `chainapi_forbid_dependencies(...)` call when introducing a new engine sub-target.

## License & Scope

Apache 2.0. The OSS license covers engine, CLI, schema, and desktop app. The AI importer (`prompts/import/`) and the planned team-workspace cloud sync are designated for closed paid components — do not commit closed-source-only code under the OSS tree without first updating `LICENSE` and PRD §15.
