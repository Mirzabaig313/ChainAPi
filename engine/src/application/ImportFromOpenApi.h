#pragma once

#include <chainapi/engine/ExecutionEngine.h>
#include <filesystem>
#include <optional>
#include <string>

namespace chainapi::engine {

class ImportFromOpenApi {
public:
    struct Result {
        std::optional<Project> project;
        std::string warnings;
    };

    Result run(const std::filesystem::path& spec) const;
};

}  // namespace chainapi::engine
