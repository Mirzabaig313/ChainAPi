#pragma once

#include "HookRunner.h"

namespace chainapi::engine {

class QuickJsHookRunner final : public HookRunner {
public:
    QuickJsHookRunner();
    ~QuickJsHookRunner() override;

    HookOutcome run_pre_request(const std::string& script,
                                 HookContext context) override;
    HookOutcome run_post_response(const std::string& script,
                                   HookContext context) override;
};

}  // namespace chainapi::engine
