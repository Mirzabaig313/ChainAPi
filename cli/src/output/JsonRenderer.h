#pragma once

#include <chainapi/engine/PublicApi.h>

#include <ostream>

namespace chainapi::cli {

/// Machine-readable JSON renderer. PRD FR-13.3.
class JsonRenderer {
public:
    explicit JsonRenderer(std::ostream& out);
    void render(const engine::RunResult& result);

private:
    std::ostream& out_;
};

}  // namespace chainapi::cli
