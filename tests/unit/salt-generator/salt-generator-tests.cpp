#include <gtest/gtest.h>

#include "salt-generator.h"

TEST(SaltTest, SaltLength) {
    const std::string emptySalt = SaltGenerator::generateSalt(0);
    EXPECT_EQ(emptySalt, "");

    int shortSaltLength = 16;
    const std::string shortSalt = SaltGenerator::generateSalt(shortSaltLength);
    EXPECT_EQ(shortSalt.size(), shortSaltLength);

    int longSaltLength = 256;
    const std::string longSalt = SaltGenerator::generateSalt(longSaltLength);
    EXPECT_EQ(longSalt.size(), longSaltLength);
}

TEST(SaltTest, SaltUniqueness) {
    int saltLength = 16;
    const std::string firstSalt = SaltGenerator::generateSalt(saltLength);
    const std::string secondSalt = SaltGenerator::generateSalt(saltLength);

    EXPECT_NE(firstSalt, secondSalt);
}