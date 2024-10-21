#include "base64.h"

std::string Base64Codec::encode(const std::string& data) {
    std::stringstream encodedStream;
    Poco::Base64Encoder encoder(encodedStream);
    encoder.write(reinterpret_cast<const char*>(data.data()), static_cast<int>(data.size()));
    encoder.close();
    return encodedStream.str();
}

std::string Base64Codec::decode(const std::string& data) {
    std::stringstream encodedStream(data);
    Poco::Base64Decoder decoder(encodedStream);
    std::vector<unsigned char> decodedData;

    int character = decoder.get();
    while (character != -1) {
        decodedData.push_back(static_cast<unsigned char>(character));
        character = decoder.get();
    }

    return {decodedData.begin(), decodedData.end()};
}