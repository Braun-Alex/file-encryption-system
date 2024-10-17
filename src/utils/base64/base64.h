#pragma once

#include "codec-interface.h"

#include "Poco/Base64Encoder.h"
#include "Poco/Base64Decoder.h"
#include <sstream>
#include <vector>

class Base64Codec final: public CodecInterface {
public:
    std::string encode(const std::string& data) const override;
    std::string decode(const std::string& data) const override;
};
