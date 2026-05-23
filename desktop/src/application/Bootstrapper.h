#pragma once

#include <chainapi/engine/PublicApi.h>

#include <memory>

namespace chainapi::desktop {

/// Constructs an ExecutionEngine with concrete infrastructure
/// implementations and exposes it to the rest of the desktop. Centralising
/// the wiring here keeps view models free of dependency-injection logic
/// and keeps the future Phase B "swap engine for IPC client" change
/// scoped to a single file (PRD §8.6 / Project Layout §6).
class Bootstrapper {
public:
    Bootstrapper();
    ~Bootstrapper();

    engine::ExecutionEngine& engine() noexcept;

private:
    std::unique_ptr<engine::ExecutionEngine> engine_;
};

}  // namespace chainapi::desktop
