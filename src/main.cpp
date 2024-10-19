#include "app.h"
#include "aes-gcm.h"
#include "symmetric-encryption-service.h"
#include "pickers.h"
#include "passphrase-checker.h"

#include <fstream>

#define ENCRYPTION_POSTFIX "_enc"
#define DECRYPTION_POSTFIX "_dec"

int main(int argc, char** argv) {
    auto ui = App::create();

    ui->on_pick_files([&]{
        ui->set_files(pick_files());
    });

    ui->on_pick_directory([&]{
        ui->set_directory(pick_directory());
    });

    ui->on_check_passphrase([&](const slint::SharedString& passphrase){
        ui->set_strong_passphrase(isPassphraseStrong(std::string(passphrase)));
    });

    ui->on_apply_cipher([&](const std::shared_ptr<slint::Model<slint::SharedString>>& filePaths,
        const slint::SharedString& passphrase, const slint::SharedString& directory, AESMode mode) {
        std::shared_ptr<SymmetricEncryptionInterface> aesAlgorithm = std::make_shared<AESEncryptionGCM>();
        SymmetricEncryptionService encryptionService(aesAlgorithm);

        size_t pathCount = filePaths->row_count();

        for (size_t i = 0; i < pathCount; ++i) {
            if (auto filePath = filePaths->row_data(i); filePath.has_value()) {
                std::string convertedFilePath(filePath.value());

                std::ifstream inputFile(convertedFilePath, std::ios::binary);
                if (!inputFile) {
                    ui->set_applying_is_successfull(false);
                }
                std::string fileContent((std::istreambuf_iterator(inputFile)), std::istreambuf_iterator<char>());
                inputFile.close();

                std::string result;
                try {
                    if (mode == AESMode::Encryption) {
                        result = encryptionService.encryptData(fileContent, std::string(passphrase));
                    } else if (mode == AESMode::Decryption) {
                        result = encryptionService.decryptData(fileContent, std::string(passphrase));
                    }
                } catch (const Poco::Exception&) {
                    ui->set_applying_is_successfull(false);
                } catch (const std::runtime_error&) {
                    ui->set_applying_is_successfull(false);
                }

                std::string POSTFIX;

                switch (mode) {
                    case AESMode::Encryption: POSTFIX = ENCRYPTION_POSTFIX; break;
                    case AESMode::Decryption: POSTFIX = DECRYPTION_POSTFIX; break;
                    default: POSTFIX = "_undefined";
                }

                std::string fileName(convertedFilePath.begin() + convertedFilePath.find_last_of('/'), convertedFilePath.end());
                auto dotPosition = fileName.find_last_of('.');
                std::ofstream outputFile(std::string(directory) + fileName.substr(0, dotPosition) + POSTFIX + fileName.substr(dotPosition),
                    std::ios::binary);

                if (!outputFile) {
                    ui->set_applying_is_successfull(false);
                }
                outputFile.write(result.data(), result.size());
                outputFile.close();
            }
        }

        ui->set_applying_is_successfull(true);
    });

    ui->run();
    return 0;
}