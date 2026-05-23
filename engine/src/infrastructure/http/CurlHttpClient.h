#pragma once

#include "HttpClient.h"

namespace chainapi::engine {

class CurlHttpClient final : public HttpClient {
public:
    CurlHttpClient();
    ~CurlHttpClient() override;

    HttpResponse send(const HttpRequest& request) override;
};

}  // namespace chainapi::engine
