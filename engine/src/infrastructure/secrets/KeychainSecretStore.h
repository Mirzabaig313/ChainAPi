#pragma once

#include "SecretStore.h"

namespace chainapi::engine {

class KeychainSecretStore final : public SecretStore {
public:
    KeychainSecretStore();
    ~KeychainSecretStore() override;

    std::optional<std::string> read(const std::string& name) override;
    void write(const std::string& name, const std::string& value) override;
    void remove(const std::string& name) override;
};

}  // namespace chainapi::engine
