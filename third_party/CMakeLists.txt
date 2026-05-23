# third_party/CMakeLists.txt
#
# Reserved for vendored dependencies. Currently empty — all required
# libraries (yaml-cpp, libcurl, sqlite3, nlohmann/json, qtkeychain) are
# managed by vcpkg.json at the project root.
#
# QuickJS will be vendored here in Phase 1 (Project Layout Q3): it is
# small, single-file, slow-moving, and avoids dependency-pinning issues.
# Expected layout when added:
#
#   third_party/
#     quickjs/
#       CMakeLists.txt          ← exposes target chainapi-quickjs
#       quickjs.c
#       quickjs.h
#       ...
