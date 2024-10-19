#pragma once

#include "Poco/Crypto/DigestEngine.h"

std::string deriveKey(const std::string& passphrase, const std::string& salt, int iterations);