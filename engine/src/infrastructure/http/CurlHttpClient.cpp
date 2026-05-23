// CurlHttpClient — libcurl-backed implementation of HttpClient.
// Phase 1 will implement: connection pooling, per-request timeouts,
// abort-via-multi-handle, TLS verification toggle, proxy config.
#include "CurlHttpClient.h"

namespace chainapi::engine {

CurlHttpClient::CurlHttpClient() = default;
CurlHttpClient::~CurlHttpClient() = default;

HttpResponse CurlHttpClient::send(const HttpRequest& /*request*/) {
    HttpResponse response;
    response.status = 0;
    return response;
}

}  // namespace chainapi::engine
