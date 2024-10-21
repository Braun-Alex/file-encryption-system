#include "aes-gcm.h"

using namespace Poco;

std::string AESEncryptionGCM::encrypt(const std::string& data, const std::string& passphrase) const {
    const std::string initializationVector = SaltGenerator::generateSalt(INITIALIZATION_VECTOR_LENGTH);
    const Crypto::CipherKey key(GCM_AES_MODE, passphrase, initializationVector);

    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    EVP_EncryptInit_ex(ctx, EVP_aes_256_gcm(), nullptr, nullptr, nullptr);
    EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, INITIALIZATION_VECTOR_LENGTH, nullptr);
    EVP_EncryptInit_ex(ctx, nullptr, nullptr, (unsigned char*)key.getKey().data(), (unsigned char*)initializationVector.data());

    unsigned char ciphertext[data.size()];
    int len;
    EVP_EncryptUpdate(ctx, ciphertext, &len, (unsigned char*)data.data(), data.size());

    unsigned char tag[16];
    EVP_EncryptFinal_ex(ctx, nullptr, &len);
    EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_GET_TAG, 16, tag);

    EVP_CIPHER_CTX_free(ctx);

    const std::string encryptedData(reinterpret_cast<const char*>(ciphertext), data.size());
    const std::string authTag(reinterpret_cast<const char*>(tag), 16);

    return Base64Codec::encode(initializationVector + encryptedData + authTag);
}

std::string AESEncryptionGCM::decrypt(const std::string& data, const std::string& passphrase) const {
    const std::string decodedData = Base64Codec::decode(data);
    const std::string initializationVector(decodedData.begin(), decodedData.begin() + INITIALIZATION_VECTOR_LENGTH);
    const std::string authTag(decodedData.end() - 16, decodedData.end());
    const std::string encryptedData(decodedData.begin() + INITIALIZATION_VECTOR_LENGTH, decodedData.end() - 16);

    const Crypto::CipherKey key(GCM_AES_MODE, passphrase, initializationVector);

    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    EVP_DecryptInit_ex(ctx, EVP_aes_256_gcm(), nullptr, nullptr, nullptr);
    EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, INITIALIZATION_VECTOR_LENGTH, nullptr);
    EVP_DecryptInit_ex(ctx, nullptr, nullptr, (unsigned char*)key.getKey().data(), (unsigned char*)initializationVector.data());

    EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_TAG, 16, (unsigned char*)authTag.data());

    unsigned char plaintext[encryptedData.size()];
    int len;
    EVP_DecryptUpdate(ctx, plaintext, &len, (unsigned char*)encryptedData.data(), encryptedData.size());

    if (EVP_DecryptFinal_ex(ctx, nullptr, &len) <= 0) {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("Invalid authentication tag");
    }

    EVP_CIPHER_CTX_free(ctx);

    return {reinterpret_cast<const char*>(plaintext), encryptedData.size()};
}