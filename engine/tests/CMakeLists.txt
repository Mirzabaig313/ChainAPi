find_package(GTest CONFIG REQUIRED)
include(GoogleTest)

# Smoke test: confirms libchainapi-engine builds, links, and the public
# error-code mapping is stable. Real engine integration tests land in
# Phase 1 per Engine Requirement §8.
add_executable(chainapi-engine-tests
    unit/SmokeTest.cpp
)
target_link_libraries(chainapi-engine-tests
    PRIVATE
        chainapi::engine
        GTest::gtest
        GTest::gtest_main
)
chainapi_set_warnings(chainapi-engine-tests)
chainapi_apply_sanitizers(chainapi-engine-tests)

gtest_discover_tests(chainapi-engine-tests
    PROPERTIES LABELS "engine;unit"
)

# The engine tests must NOT pull in Qt UI either.
chainapi_forbid_dependencies(chainapi-engine-tests
    Qt6::Widgets
    Qt6::Gui
    Qt6::Quick
    Qt6::QuickWidgets
)
