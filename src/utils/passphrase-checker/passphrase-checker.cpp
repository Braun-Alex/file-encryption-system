#include "passphrase-checker.h"

bool isPassphraseStrong(const std::string& password) {
    const Poco::RegularExpression regex("^(?=.*[a-z])(?=.*[A-Z])(?=.*\\d)(?=.*[^a-zA-Z\\d]).{13,}$");
    return regex.match(password);
}