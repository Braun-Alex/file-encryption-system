#pragma once

#include <string>
#include <random>

class SaltGenerator final {
public:
    static std::string generateSalt(int length);

private:
    inline static std::string CHARSET = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
};