#include <gtest/gtest.h>
#include <gmock/gmock-more-matchers.h>

#include "kdf.h"
#include "aes-gcm.h"
#include "symmetric-encryption-service.h"

class SymmetricCipherTest: public testing::TestWithParam<std::pair<std::string, std::string>> {
protected:
    void SetUp() override {
        const auto [DATA, PASSPHRASE] = GetParam();
        data = DATA;
        const std::string salt = SaltGenerator::generateSalt(saltLength);
        derivedKey = deriveKey(PASSPHRASE, salt, kdfIterations);
    }

    const int saltLength = 16, kdfIterations = 1000;
    std::string data, derivedKey;
    SymmetricEncryptionService encryptionService{std::make_shared<AESEncryptionGCM>()};
};

TEST_P(SymmetricCipherTest, CipherTextUniqueness) {
    const std::string firstCipherText = encryptionService.encryptData(data, derivedKey);
    const std::string secondCipherText = encryptionService.encryptData(data, derivedKey);
    EXPECT_NE(firstCipherText, secondCipherText);
}

TEST_P(SymmetricCipherTest, EncryptionAndDecryption) {
    const std::string cipherText = encryptionService.encryptData(data, derivedKey);
    const std::string decryptedData = encryptionService.decryptData(cipherText, derivedKey);
    EXPECT_EQ(data, decryptedData);
}

TEST_P(SymmetricCipherTest, IncorrectPassphraseDecryption) {
    const std::string cipherText = encryptionService.encryptData(data, derivedKey);
    EXPECT_ANY_THROW(encryptionService.decryptData(cipherText, SaltGenerator::generateSalt(saltLength)));
}

TEST_P(SymmetricCipherTest, CorruptedCipherTextDecryption) {
    const std::string cipherText = encryptionService.encryptData(data, derivedKey);
    EXPECT_ANY_THROW(encryptionService.decryptData(SaltGenerator::generateSalt(saltLength), derivedKey));
}

INSTANTIATE_TEST_SUITE_P(Default, SymmetricCipherTest, testing::Values(
    std::make_pair("", ""),
    std::make_pair("The quick brown fox jumped over the dog", ""),
    std::make_pair("", "NerwDCx{<m%F5c?la{Rvm6L%_=2Q}>8(M^_@2hyl%;o_fu{c%1"),
    std::make_pair("The quick brown fox jumped over the dog", "NerwDCx{<m%F5c?la{Rvm6L%_=2Q}>8(M^_@2hyl%;o_fu{c%9"),
    std::make_pair("The quick brown dog jumped over the fox", "NerwDCx{<m%F5c?la{Rvm6L%_=2Q}>8(M^_@2hyl%;o_fu{c%3")
));