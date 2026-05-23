// Engine-internal HTTP client interface. Concrete implementation:
// CurlHttpClient. The interface is process-local; no Qt types leak.
#pragma once

#include <chainapi/engine/Operation.h>
#include <chrono>
#include <map>
#include <optional>
#include <string>
#include <vector>

namespace chainapi::engine {

struct HttpRequest {
    HttpMethod method{HttpMethod::Get};
    std::string url;
    std::map<std::string, std::string> headers;
    std::optional<std::string> body;
    std::chrono::milliseconds timeout{30'000};
};

struct HttpResponse {
    int status{0};
    std::vector<std::pair<std::string, std::string>> headers;
    std::string body;
    std::chrono::milliseconds elapsed{};
};

class HttpClient {
public:
    virtual ~HttpClient() = default;

    /// Synchronous; cancellation is via the engine-level cancel() that
    /// aborts the underlying transport (libcurl multi-handle).
    /// Engine Req §3.8.
    virtual HttpResponse send(const HttpRequest& request) = 0;
};

}  // namespace chainapi::engine
