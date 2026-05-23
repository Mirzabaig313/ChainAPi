#include "Bootstrapper.h"

namespace chainapi::desktop {

Bootstrapper::Bootstrapper() {
    // Phase 1/2: construct concrete dependencies (CurlHttpClient,
    // YamlSchemaParser, SqliteHistoryStore, KeychainSecretStore,
    // QuickJsHookRunner) and inject them into ExecutionEngine.
    //
    // Note: those concrete types live in engine/src/ and are not part of
    // the public engine surface, so the Bootstrapper depends on factory
    // helpers exposed from the engine library. Those factories will be
    // added when the concrete implementations land.
    //
    // engine::ExecutionEngine::Dependencies deps {
    //     engine::make_curl_http_client(),
    //     engine::make_yaml_schema_parser(),
    //     engine::make_sqlite_history_store(),
    //     engine::make_keychain_secret_store(),
    //     engine::make_quickjs_hook_runner(),
    // };
    // engine_ = std::make_unique<engine::ExecutionEngine>(std::move(deps));
}

Bootstrapper::~Bootstrapper() = default;

engine::ExecutionEngine& Bootstrapper::engine() noexcept {
    return *engine_;
}

}  // namespace chainapi::desktop
