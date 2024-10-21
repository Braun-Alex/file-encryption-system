#include <gtest/gtest.h>

#include "base64.h"

TEST(Base64Codec, EncodeAndDecode) {
    const std::string data = "Encoding and decoding test";
    const std::string encodedData = Base64Codec::encode(data);

    EXPECT_EQ(Base64Codec::decode(encodedData), data);
}