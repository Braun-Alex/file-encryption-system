#pragma once

#include "Poco/Base64Encoder.h"
#include "Poco/Base64Decoder.h"

#include <sstream>
#include <vector>

class Base64Codec {
public:
    static std::string encode(const std::string& data);
    static std::string decode(const std::string& data);
};
