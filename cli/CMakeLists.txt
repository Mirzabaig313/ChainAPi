add_executable(chainapi-cli
    src/main.cpp
    src/commands/RunCommand.cpp
    src/commands/LintCommand.cpp
    src/commands/ImportCommand.cpp
    src/output/TextRenderer.cpp
    src/output/JsonRenderer.cpp
    src/output/JUnitRenderer.cpp
)

target_link_libraries(chainapi-cli
    PRIVATE
        chainapi::engine
        Qt6::Core
)

set_target_properties(chainapi-cli PROPERTIES OUTPUT_NAME "chainapi")

chainapi_set_warnings(chainapi-cli)
chainapi_apply_sanitizers(chainapi-cli)

# CLI must not pull in Qt UI libraries even transitively.
chainapi_forbid_dependencies(chainapi-cli
    Qt6::Widgets
    Qt6::Gui
    Qt6::Quick
    Qt6::QuickWidgets
)
