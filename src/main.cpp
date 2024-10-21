#include "app.h"
#include "kdf.h"
#include "aes-gcm.h"
#include "symmetric-encryption-service.h"
#include "pickers.h"
#include "passphrase-checker.h"

#include <fstream>

#define SALT_LENGTH 16
#define KDF_ITERATIONS 1000
#define ENCRYPTION_POSTFIX "_enc"
#define DECRYPTION_POSTFIX "_dec"
#define APPLYING_FAILED "_failed"
#define LOG_FILENAME "logs.txt"

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
        const std::shared_ptr<SymmetricEncryptionInterface> aesAlgorithm = std::make_shared<AESEncryptionGCM>();
        const SymmetricEncryptionService encryptionService(aesAlgorithm);

        size_t pathCount = filePaths->row_count();
        std::string logs;

        for (size_t i = 0; i < pathCount; ++i) {
            if (auto filePath = filePaths->row_data(i); filePath.has_value()) {
                std::string convertedFilePath(filePath.value());

                if (std::ifstream inputFile(convertedFilePath, std::ios::binary); !inputFile) {
                    logs += "File " + convertedFilePath + " could not be opened!\n";
                } else {
                    std::string fileContent((std::istreambuf_iterator(inputFile)), std::istreambuf_iterator<char>());
                    inputFile.close();

                    std::string result, POSTFIX;
                    try {
                        if (mode == AESMode::Encryption) {
                            const std::string salt = SaltGenerator::generateSalt(SALT_LENGTH);
                            const std::string derivedKey = deriveKey(std::string(passphrase), salt, KDF_ITERATIONS);
                            result = salt + encryptionService.encryptData(fileContent, derivedKey);
                            logs += "File " + convertedFilePath + " has been successfully encrypted\n";
                        } else if (mode == AESMode::Decryption) {
                            const std::string salt = fileContent.substr(0, SALT_LENGTH);
                            const std::string derivedKey = deriveKey(std::string(passphrase), salt, KDF_ITERATIONS);
                            result = encryptionService.decryptData(fileContent.substr(SALT_LENGTH), derivedKey);
                            logs += "File " + convertedFilePath + " has been successfully decrypted\n";
                        }
                    } catch (const Poco::Exception& exception) {
                        if (mode == AESMode::Encryption) {
                            result = "Encryption failed. " + exception.displayText();
                            logs += "File " + convertedFilePath + " encryption failed. " + exception.displayText() + "\n";
                        } else if (mode == AESMode::Decryption) {
                            result = "Decryption failed. " + exception.displayText();
                            logs += "File " + convertedFilePath + " decryption failed. " + exception.displayText() + "\n";
                        }
                        POSTFIX = APPLYING_FAILED;
                    } catch (const std::exception& error) {
                        if (mode == AESMode::Encryption) {
                            result = "Encryption failed. " + std::string(error.what());
                            logs += "File " + convertedFilePath + " encryption failed. " + std::string(error.what()) + "\n";
                        } else if (mode == AESMode::Decryption) {
                            result = "Decryption failed. " + std::string(error.what());
                            logs += "File " + convertedFilePath + " decryption failed. " + std::string(error.what())+ "\n";
                        }
                        POSTFIX = APPLYING_FAILED;
                    }

                    switch (mode) {
                        case AESMode::Encryption: POSTFIX += ENCRYPTION_POSTFIX; break;
                        case AESMode::Decryption: POSTFIX += DECRYPTION_POSTFIX; break;
                        default: POSTFIX = "_undefined";
                    }

                    std::string fileName(convertedFilePath.begin() + convertedFilePath.find_last_of('/'), convertedFilePath.end());
                    auto dotPosition = fileName.find_last_of('.');
                    std::ofstream outputFile(std::string(directory) + fileName.substr(0, dotPosition) + POSTFIX + fileName.substr(dotPosition),
                        std::ios::binary);

                    if (!outputFile) {
                        logs += "File " + directory + "/" + fileName + " could not be saved!\n";
                    } else {
                        outputFile.write(result.data(), result.size());
                        outputFile.close();
                    }
                }
            }
        }

        if (std::ofstream logFile(std::string(directory) + "/" + LOG_FILENAME, std::ios::binary); logFile) {
            logFile.write(logs.data(), logs.size());
            logFile.close();
        }
    });

    ui->run();
    return 0;
}