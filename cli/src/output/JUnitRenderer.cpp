#include "JUnitRenderer.h"

namespace chainapi::cli {

JUnitRenderer::JUnitRenderer(std::ostream& out) : out_(out) {}

void JUnitRenderer::render(const engine::RunResult& /*result*/) {
    // Phase 1: generate JUnit-compatible XML for CI consumers.
    out_ << "<testsuites/>\n";
}

}  // namespace chainapi::cli
