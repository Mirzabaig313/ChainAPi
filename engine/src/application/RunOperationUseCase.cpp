// RunOperationUseCase — high-level coordinator that the UI/CLI invokes.
// Phase 1 implementation pending; this file is a skeleton anchor for the
// engine-application library.
#include "RunOperationUseCase.h"

namespace chainapi::engine {

RunOperationUseCase::RunOperationUseCase(ExecutionEngine& engine)
    : engine_(engine) {}

RunResult RunOperationUseCase::execute(const Project& project,
                                        const OperationId& target,
                                        RunContext& ctx,
                                        const RunOptions& options) {
    return engine_.run(project, target, ctx, options);
}

}  // namespace chainapi::engine
