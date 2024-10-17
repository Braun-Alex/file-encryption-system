#pragma once

#include "symmetric-encryption.h"

#include <memory>

class SymmetricEncryptionService {
public:
    explicit SymmetricEncryptionService(std::shared_ptr<SymmetricEncryptionInterface> algorithm);
    std::string encryptData(const std::string& data, const std::string& passphrase) const;
    std::string decryptData(const std::string& data, const std::string& passphrase) const;

private:
    std::shared_ptr<SymmetricEncryptionInterface> encryptionAlgorithm;
};