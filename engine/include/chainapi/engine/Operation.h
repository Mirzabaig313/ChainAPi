// Operation — a single endpoint action on a resource (PRD §4.2).
#pragma once

#include <chrono>
#include <map>
#include <optional>
#include <string>
#include <vector>

namespace chainapi::engine {

/// Stable identifier for an operation: "<resource>.<op_name>", e.g. "order.create".
struct OperationId {
    std::string value;

    bool operator==(const OperationId&) const = default;
    auto operator<=>(const OperationId&) const = default;
};

/// Stable identifier for a resource, e.g. "order".
struct ResourceId {
    std::string value;

    bool operator==(const ResourceId&) const = default;
    auto operator<=>(const ResourceId&) const = default;
};

/// Stable identifier for an actor, e.g. "vendor".
struct ActorId {
    std::string value;

    bool operator==(const ActorId&) const = default;
    auto operator<=>(const ActorId&) const = default;
};

enum class HttpMethod {
    Get, Post, Put, Patch, Delete, Head, Options
};

/// Where to pull a value out of the response.
struct Extraction {
    std::string variable_name;       ///< Stored as <resource>.<variable_name>
    std::string source_path;         ///< JSONPath / XPath / header / regex
    enum class Source { JsonPath, XPath, Header, StatusCode, Regex, Cookie };
    Source source{Source::JsonPath};
};

/// Per-operation retry policy. Engine spec §3.5.
struct RetryPolicy {
    int max_attempts{3};
    std::chrono::milliseconds base_backoff{500};
    std::chrono::milliseconds max_backoff{30'000};
};

/// One declared operation. Mirrors PRD §5.6 schema.
struct Operation {
    OperationId id;
    ResourceId  resource;
    ActorId     actor;

    HttpMethod  method{HttpMethod::Get};
    std::string path_template;                      ///< e.g. /api/v1/orders/{{order.order_id}}
    std::map<std::string, std::string> headers;
    std::map<std::string, std::string> query_params;
    std::optional<std::string> body_template;       ///< raw template, may contain {{X.y}}

    std::optional<int> expect_status;
    std::vector<Extraction> extractions;

    /// Explicit dependencies declared by the user (PRD §5.7 / §4.3).
    std::vector<OperationId> explicit_dependencies;

    /// Optional inline JS hook scripts (engine spec §3.12).
    std::optional<std::string> pre_request_script;
    std::optional<std::string> post_response_script;

    RetryPolicy retry;
    std::optional<std::chrono::milliseconds> timeout;
    bool force{false};                              ///< Per-op force re-run flag.
};

}  // namespace chainapi::engine
