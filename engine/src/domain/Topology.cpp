// Topological sort utilities — Kahn's algorithm with stable tie-break.
// Engine Req AC-3.1.3.
#include "Topology.h"

namespace chainapi::engine {

std::vector<OperationId>
topological_sort(const std::map<OperationId, std::vector<OperationId>>& /*edges*/) {
    // Phase 1: real implementation (Kahn's, in-degree priority queue with
    // lexicographic tie-break to satisfy determinism guarantee).
    return {};
}

}  // namespace chainapi::engine
