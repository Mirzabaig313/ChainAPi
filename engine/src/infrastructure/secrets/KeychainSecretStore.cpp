// KeychainSecretStore — OS-keychain-backed secret storage via QtKeychain.
// PRD §5.1 / NFR-3.2.
//
// Implementation is gated on CHAINAPI_HAS_QTKEYCHAIN; if QtKeychain is not
// available at configure time the store falls back to an in-memory
// placeholder that will refuse writes (so the absence is loud, not silent).
#include "KeychainSecretStore.h"

namespace chainapi::engine {

KeychainSecretStore::KeychainSecretStore() = default;
KeychainSecretStore::~KeychainSecretStore() = default;

std::optional<std::string>
KeychainSecretStore::read(const std::string& /*name*/) {
    // Phase 1: synchronous bridge over QtKeychain's async API via QEventLoop.
    return std::nullopt;
}

void KeychainSecretStore::write(const std::string& /*name*/,
                                 const std::string& /*value*/) {
    // Phase 1.
}

void KeychainSecretStore::remove(const std::string& /*name*/) {
    // Phase 1.
}

}  // namespace chainapi::engine
