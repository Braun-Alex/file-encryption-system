#pragma once
#include <string>

class CodecInterface {
public:
    virtual std::string encode(const std::string& data) const = 0;
    virtual std::string decode(const std::string& data) const = 0;
    virtual ~CodecInterface() = default;
};