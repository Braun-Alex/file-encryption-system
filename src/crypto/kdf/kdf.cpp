#include "kdf.h"

std::string deriveKey(const std::string& passphrase, const std::string& salt, int iterations) {
    if (iterations <= 0) {
        return passphrase;
    }

    Poco::Crypto::DigestEngine engine("SHA256");
    for (int i = 0; i < iterations; ++i) {
        engine.update(passphrase + salt);
    }
    return Poco::Crypto::DigestEngine::digestToHex(engine.digest());
}