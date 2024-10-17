#pragma once

#include <string>

class SymmetricEncryptionInterface {
public:
    virtual std::string encrypt(const std::string& data, const std::string& passphrase) const = 0;
    virtual std::string decrypt(const std::string& data, const std::string& passphrase) const = 0;
    virtual ~SymmetricEncryptionInterface() = default;
};
