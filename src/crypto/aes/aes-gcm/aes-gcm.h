#pragma once

#include "salt-generator.h"
#include "base64.h"
#include "symmetric-encryption.h"

#include <openssl/evp.h>
#include "Poco/Crypto/Cipher.h"
#include "Poco/Crypto/CipherKey.h"
#include "Poco/Crypto/CipherFactory.h"

class AESEncryptionGCM final: public SymmetricEncryptionInterface {
public:
    std::string encrypt(const std::string& data, const std::string& passphrase) const override;
    std::string decrypt(const std::string& data, const std::string& passphrase) const override;

private:
    const int INITIALIZATION_VECTOR_LENGTH = 16;
    const std::string GCM_AES_MODE = "aes-256-gcm";
};