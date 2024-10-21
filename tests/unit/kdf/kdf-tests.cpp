#include <gtest/gtest.h>

#include "salt-generator.h"
#include "kdf.h"

TEST(KDFTest, KeyPassphraseUniqueness) {
    int saltLength = 16, kdfIterations = 1000;
    const std::string passphrase = "Key uniqueness for passphrases",
                      firstSalt = SaltGenerator::generateSalt(saltLength),
                      secondSalt = SaltGenerator::generateSalt(saltLength);

    const std::string firstDerivedKey = deriveKey(passphrase, firstSalt, kdfIterations),
                      secondDerivedKey = deriveKey(passphrase, secondSalt, kdfIterations);

    EXPECT_NE(firstDerivedKey, secondDerivedKey);
}

TEST(KDFTest, EqualKeys) {
    int kdfIterations = 1000;
    const std::string passphrase = "Key uniqueness for passphrases",
                      salt = SaltGenerator::generateSalt(16);

    const std::string firstDerivedKey = deriveKey(passphrase, salt, kdfIterations),
                      secondDerivedKey = deriveKey(passphrase, salt, kdfIterations);

    EXPECT_EQ(firstDerivedKey, secondDerivedKey);
}

TEST(KDFTest, KeyEqualsPassphrase) {
    const std::string passphrase = "Key equals passphrase",
          salt = SaltGenerator::generateSalt(16);

    const std::string theSamePassphrase = deriveKey(passphrase, salt, 0);

    EXPECT_EQ(passphrase, theSamePassphrase);
}

TEST(KDFTest, KeyIterationsUniqueness) {
    const std::string passphrase = "Key uniqueness for KDF iterations",
          salt = SaltGenerator::generateSalt(16);

    const std::string firstDerivedKey = deriveKey(passphrase, salt, 50),
                      secondDerivedKey = deriveKey(passphrase, salt, 1000);

    EXPECT_NE(firstDerivedKey, secondDerivedKey);
}