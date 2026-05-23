// Public façade for libchainapi-engine. Project Layout §3.
//
// pImpl + value types only — no Qt UI types appear here, no infra-library
// types leak to embedders. Phase B (post-MVP) extracting the engine to a
// separate process or a Rust rewrite is a build-system change rather than a
// rewrite because of this surface.
#pragma once

#include <chainapi/engine/Actor.h>
#include <chainapi/engine/ErrorCodes.h>
#include <chainapi/engine/Events.h>
#include <chainapi/engine/Operation.h>
#include <chainapi/engine/Resource.h>
#include <chainapi/engine/RunContext.h>

#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace chainapi::engine {

// Forward-declared infrastructure interfaces. Concrete implementations
// live in `engine/src/infrastructure/` and are not part of the public ABI.
class HttpClient;
class SchemaParser;
class HistoryStore;
class SecretStore;
class HookRunner;

/// Result of a run.
struct RunResult {
    RunId run_id;
    RunOutcome outcome{RunOutcome::Succeeded};
    std::vector<StepResult> steps;

    [[nodiscard]] bool succeeded() const noexcept {
        return outcome == RunOutcome::Succeeded;
    }
};

/// Loaded, validated project. The schema parser produces this; the engine
/// consumes it. Cycles, undefined references, and unsupported versions
/// are caught at parse time and surfaced as `ErrorCode::*` values.
struct Project {
    std::string name;
    std::string default_environment;
    std::map<ActorId, Actor> actors;
    std::map<ResourceId, Resource> resources;
    std::map<std::string, std::map<std::string, std::string>> environments;
};

/// Per-run options.
struct RunOptions {
    bool dry_run{false};
    bool reset_extractions{false};        ///< "Reset Cache" — Engine Req AC-3.4.2
    bool reset_sessions{false};           ///< "Send Cleanly"  — AC-3.4.3
    std::string environment;              ///< Empty → use project default.
};

class ExecutionEngine {
public:
    /// Dependencies are constructor-injected. Tests substitute fakes;
    /// production wiring lives in `Bootstrapper.cpp` (desktop) or
    /// `main.cpp` (cli).
    struct Dependencies {
        std::unique_ptr<HttpClient>   http;
        std::unique_ptr<SchemaParser> schema;
        std::unique_ptr<HistoryStore> history;
        std::unique_ptr<SecretStore>  secrets;
        std::unique_ptr<HookRunner>   hooks;
    };

    explicit ExecutionEngine(Dependencies deps);
    ~ExecutionEngine();

    ExecutionEngine(const ExecutionEngine&) = delete;
    ExecutionEngine& operator=(const ExecutionEngine&) = delete;
    ExecutionEngine(ExecutionEngine&&) noexcept;
    ExecutionEngine& operator=(ExecutionEngine&&) noexcept;

    /// Execute a single operation, auto-resolving its dependency chain.
    /// `ctx` is mutated with session and extraction state for reuse on
    /// subsequent runs in the same project session.
    RunResult run(const Project& project,
                  const OperationId& target,
                  RunContext& ctx,
                  const RunOptions& options = {});

    /// Cancel an in-flight run. Engine Req §3.8.
    void cancel(RunId run);

    /// Subscribe to streaming run events. Engine Req §10.
    using EventCallback = std::function<void(const RunEvent&)>;
    void subscribe(EventCallback callback);

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

}  // namespace chainapi::engine
