#pragma once

#include <chainapi/engine/Operation.h>
#include <map>
#include <vector>

namespace chainapi::engine {

/// Stable topological sort. Tie-break is lexicographic over OperationId.value.
std::vector<OperationId>
topological_sort(const std::map<OperationId, std::vector<OperationId>>& edges);

}  // namespace chainapi::engine
