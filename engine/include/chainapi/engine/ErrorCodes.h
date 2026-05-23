// Error taxonomy from Engine Requirement §5. Codes are stable; UI/CLI
// render them; QA asserts on them.
#pragma once

#include <string_view>

namespace chainapi::engine {

enum class ErrorCode {
    // Schema layer
    SchemaInvalid,
    YamlParse,
    Cycle,
    RefUndefined,
    SchemaVersion,

    // Variable resolution
    VarUnresolved,
    IndexedRefOutOfRange,

    // Network
    NetworkTimeout,
    NetworkDns,
    NetworkTls,

    // HTTP
    Http5xx,
    Http4xx,
    StatusMismatch,

    // Auth
    SessionRefreshFailed,

    // Hooks
    HookFailure,
    HookTimeout,

    // Extraction
    ExtractionFailed,
    ResponseParse,

    // Run
    Cancelled,
};

enum class ErrorClass {
    Schema,
    Resolution,
    Network,
    Http,
    Auth,
    Hook,
    Extraction,
    Run,
};

/// Human-readable, stable code string (e.g. "E_CYCLE"). Safe to use in logs
/// and asserted on by integration tests.
std::string_view to_code_string(ErrorCode code) noexcept;

/// Whether a step that fails with this code should be retried per the
/// per-operation RetryPolicy.
bool is_retryable(ErrorCode code) noexcept;

ErrorClass classify(ErrorCode code) noexcept;

}  // namespace chainapi::engine
