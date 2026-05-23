// Engine-internal: builds the execution chain for a target operation.
// Engine Requirement §3.1.
#pragma once

#include <chainapi/engine/ExecutionEngine.h>
#include <chainapi/engine/Operation.h>
#include <vector>

namespace chainapi::engine {

class DependencyResolver {
public:
    DependencyResolver();
    ~DependencyResolver();

    /// Returns the chain in topological order, terminating with `target`.
    /// Throws on cycle (caught by caller and surfaced as ErrorCode::Cycle).
    std::vector<OperationId> resolve(const Project& project,
                                      const OperationId& target) const;
};

}  // namespace chainapi::engine
