// YamlSchemaParser — yaml-cpp-backed schema parser.
//
// Phase 1 will:
//   - Parse chainapi.yaml + imports glob (PRD §5.3).
//   - Validate version (last 3 majors supported).
//   - Build implicit dependency edges from {{X.y}} references.
//   - Detect cycles and undefined references (Engine Req §3.1).
//   - Produce SchemaError with file:line on failure.
#include "YamlSchemaParser.h"

namespace chainapi::engine {

YamlSchemaParser::YamlSchemaParser() = default;
YamlSchemaParser::~YamlSchemaParser() = default;

SchemaParseResult
YamlSchemaParser::parse(const std::filesystem::path& /*root_yaml*/) {
    Project project;
    return project;
}

}  // namespace chainapi::engine
