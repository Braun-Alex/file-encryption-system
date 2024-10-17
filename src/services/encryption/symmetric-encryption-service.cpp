#include "symmetric-encryption-service.h"

#include <utility>

SymmetricEncryptionService::SymmetricEncryptionService(
    std::shared_ptr<SymmetricEncryptionInterface> algorithm): encryptionAlgorithm(std::move(algorithm)) {}

std::string SymmetricEncryptionService::encryptData(const std::string& data, const std::string& passphrase) const {
    return encryptionAlgorithm->encrypt(data, passphrase);
}

std::string SymmetricEncryptionService::decryptData(const std::string& data, const std::string& passphrase) const {
    return encryptionAlgorithm->decrypt(data, passphrase);
}
