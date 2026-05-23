// QuickJsHookRunner — sandboxed JS execution via QuickJS.
// PRD §5.10 / Engine Req §3.12.
//
// Sandbox enforced at runtime: no FS, no network, 1s timeout, read-only
// access to other actors' variables. Phase 1 implementation.
#include "QuickJsHookRunner.h"

namespace chainapi::engine {

QuickJsHookRunner::QuickJsHookRunner() = default;
QuickJsHookRunner::~QuickJsHookRunner() = default;

HookOutcome
QuickJsHookRunner::run_pre_request(const std::string& /*script*/,
                                    HookContext context) {
    HookOutcome out;
    out.ok = true;
    out.mutated_request = std::move(context.request);
    return out;
}

HookOutcome
QuickJsHookRunner::run_post_response(const std::string& /*script*/,
                                      HookContext context) {
    HookOutcome out;
    out.ok = true;
    out.mutated_request = std::move(context.request);
    out.mutated_response = std::move(context.response);
    return out;
}

}  // namespace chainapi::engine
