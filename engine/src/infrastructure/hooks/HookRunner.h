// Engine-internal interface for executing pre/post hooks in a sandboxed
// JS environment. Concrete impl: QuickJsHookRunner.
//
// Sandbox rules (PRD §5.10):
//   - No filesystem access
//   - No network beyond the operation's own request
//   - 1-second timeout per hook
//   - Read-only access to other actors' variables
#pragma once

#include <chainapi/engine/Operation.h>
#include <map>
#include <optional>
#include <string>

namespace chainapi::engine {

struct HookRequestView {
    HttpMethod method{};
    std::string url;
    std::map<std::string, std::string> headers;
    std::optional<std::string> body;
};

struct HookResponseView {
    int status{0};
    std::map<std::string, std::string> headers;
    std::string body;
};

struct HookContext {
    HookRequestView request;
    std::optional<HookResponseView> response;       ///< Only set for post_response.
    std::map<std::string, std::map<std::string, std::string>> variables;
    std::map<std::string, std::string> env;
};

struct HookOutcome {
    bool ok{true};
    std::string error_message;
    HookRequestView mutated_request;                 ///< Pre-request hooks may mutate.
    std::optional<HookResponseView> mutated_response;
};

class HookRunner {
public:
    virtual ~HookRunner() = default;

    virtual HookOutcome run_pre_request(const std::string& script,
                                        HookContext context) = 0;
    virtual HookOutcome run_post_response(const std::string& script,
                                          HookContext context) = 0;
};

}  // namespace chainapi::engine
