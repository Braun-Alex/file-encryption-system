#include <gtest/gtest.h>

#include "passphrase-checker.h"

TEST(PassphraseChecker, WeakPassphrase) {
    const std::vector<std::string> weakPassphrases = {
        "",
        "123",
        "123456789",
        "ABCDEFGKL",
        "It is my passphrase",
        "Hi123456789",
        "21.10.2024",
        "(Short)123",
        "3_5_9_15_39_359"
    };

    for (const auto& weakPassphrase: weakPassphrases) {
        EXPECT_FALSE(isPassphraseStrong(weakPassphrase));
    }
}

TEST(PassphraseChecker, StrongPassphrase) {
    const std::vector<std::string> strongPassphrases = {
        "ohb#ApCa?wdCD4R",
        ":Tp&<)22aUG1WB_$xwqgzB:5|VvUv5",
        "NerwDCx{<m%F5c?la{Rvm6L%_=2Q}>8(M^_@2hyl%;o_fu{c%7"
    };

    for (const auto& strongPassphrase: strongPassphrases) {
        EXPECT_TRUE(isPassphraseStrong(strongPassphrase));
    }
}