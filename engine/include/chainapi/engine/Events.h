// Observability events emitted by the engine (Engine Requirement §10).
// Consumed by the desktop timeline UI and the CLI's renderers.
#pragma once

#include <chainapi/engine/ErrorCodes.h>
#include <chainapi/engine/Operation.h>
#include <chrono>
#include <cstdint>
#include <optional>
#include <string>
#include <variant>
#include <vector>

namespace chainapi::engine {

struct RunId {
    std::uint64_t value{0};
    auto operator<=>(const RunId&) const = default;
};

using TimePoint = std::chrono::system_clock::time_point;

enum class SkipReason { SessionValid, ExtractionCached };

struct RunStarted {
    RunId run_id;
    OperationId target;
    std::size_t chain_size{};
    std::string env_name;
    TimePoint at;
};

struct StepStarted {
    RunId run_id;
    std::size_t step_index{};
    OperationId op;
    int attempt{1};
    TimePoint at;
};

struct StepSkipped {
    RunId run_id;
    std::size_t step_index{};
    OperationId op;
    SkipReason reason{};
    TimePoint at;
};

struct RequestPrepared {
    RunId run_id;
    std::size_t step_index{};
    HttpMethod method{};
    std::string url;                    ///< Fully-resolved (after substitution).
    std::vector<std::pair<std::string, std::string>> masked_headers;
    std::size_t body_size{};
    TimePoint at;
};

struct ResponseReceived {
    RunId run_id;
    std::size_t step_index{};
    int status{};
    std::vector<std::pair<std::string, std::string>> headers;
    std::size_t body_size{};
    std::chrono::milliseconds elapsed{};
    TimePoint at;
};

struct ExtractionApplied {
    RunId run_id;
    std::size_t step_index{};
    ResourceId resource;
    std::vector<std::string> variable_names;   ///< Names only; values masked if from auth.
    TimePoint at;
};

struct StepFailed {
    RunId run_id;
    std::size_t step_index{};
    OperationId op;
    ErrorCode code{};
    ErrorClass cls{};
    int attempt{1};
    std::string detail;
    TimePoint at;
};

struct StepCancelled {
    RunId run_id;
    std::size_t step_index{};
    OperationId op;
    TimePoint at;
};

struct SessionRefreshed {
    RunId run_id;
    ActorId actor;
    enum class Trigger { Expiry, Unauthorized };
    Trigger trigger{};
    TimePoint at;
};

enum class RunOutcome { Succeeded, Failed, Cancelled };

struct RunEnded {
    RunId run_id;
    RunOutcome outcome{};
    std::chrono::milliseconds elapsed{};
    TimePoint at;
};

using RunEvent = std::variant<
    RunStarted,
    StepStarted,
    StepSkipped,
    RequestPrepared,
    ResponseReceived,
    ExtractionApplied,
    StepFailed,
    StepCancelled,
    SessionRefreshed,
    RunEnded
>;

}  // namespace chainapi::engine
