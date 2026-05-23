// Engine-internal interface for OS-keychain secret storage.
// Concrete impl: KeychainSecretStore (QtKeychain-backed).
#pragma once

#include <optional>
#include <string>

namespace chainapi::engine {

class SecretStore {
public:
    virtual ~SecretStore() = default;

    /// Read a named secret. Returns nullopt if not present.
    virtual std::optional<std::string> read(const std::string& name) = 0;

    /// Write or overwrite a secret. Throws on backend failure.
    virtual void write(const std::string& name, const std::string& value) = 0;

    /// Remove a secret. No-op if missing.
    virtual void remove(const std::string& name) = 0;
};

}  // namespace chainapi::engine
