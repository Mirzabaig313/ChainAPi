// Engine-internal interface for parsing chainapi.yaml (and sub-files) into
// a validated Project. Concrete impl: YamlSchemaParser (yaml-cpp).
//
// Cycle detection, undefined-reference detection, and version validation
// happen here. Errors produce an explanation pointing to file:line.
#pragma once

#include <chainapi/engine/ErrorCodes.h>
#include <chainapi/engine/ExecutionEngine.h>
#include <filesystem>
#include <string>
#include <variant>

namespace chainapi::engine {

struct SchemaError {
    ErrorCode code{};
    std::string message;
    std::filesystem::path file;
    int line{0};
};

using SchemaParseResult = std::variant<Project, SchemaError>;

class SchemaParser {
public:
    virtual ~SchemaParser() = default;

    virtual SchemaParseResult parse(const std::filesystem::path& root_yaml) = 0;
};

}  // namespace chainapi::engine
