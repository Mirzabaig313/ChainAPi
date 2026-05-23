// Actor — an identity with its own auth flow (PRD §4.1, §5.5).
#pragma once

#include <chainapi/engine/Operation.h>
#include <chrono>
#include <map>
#include <optional>
#include <string>
#include <vector>

namespace chainapi::engine {

/// Auth strategy for an actor. PRD §5.5 supports `simple` (single request)
/// and `chain` (multi-step, e.g. OTP).
enum class AuthStrategy {
    Simple,
    Chain
};

/// One step in an actor's auth chain.
struct AuthStep {
    std::string id;                                ///< Stable id within the actor.
    HttpMethod method{HttpMethod::Post};
    std::string path_template;
    std::map<std::string, std::string> headers;
    std::optional<std::string> body_template;
    std::optional<int> expect_status;
    std::vector<Extraction> extractions;
};

/// Optional refresh block. Engine spec §3.3.2 / AC-3.3.3.
struct SessionRefresh {
    HttpMethod method{HttpMethod::Post};
    std::string path_template;
    std::map<std::string, std::string> headers;
    std::optional<std::string> body_template;
    std::vector<Extraction> extractions;
};

/// Headers (and other request artifacts) injected into every operation
/// performed as this actor. Mirrors PRD §5.5 `inject:` block.
struct ActorInjection {
    std::map<std::string, std::string> headers;
};

struct Actor {
    ActorId id;
    std::string description;

    AuthStrategy strategy{AuthStrategy::Simple};
    std::vector<AuthStep> auth_steps;              ///< For `simple`, exactly one step.

    std::chrono::seconds session_ttl{15 * 60};
    std::optional<SessionRefresh> refresh;

    ActorInjection inject;
};

}  // namespace chainapi::engine
