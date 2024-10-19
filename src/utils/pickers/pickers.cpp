#include "pickers.h"

std::shared_ptr<slint::VectorModel<slint::SharedString>> pick_files() {
    const std::string pick_command = R"(zenity --file-selection --multiple --title="Choose files" --separator="|")";

    FILE* pipe = popen(pick_command.c_str(), "r");

    if (!pipe) {
        return {};
    }

    std::string filePaths;

    if (char buffer[1024]; fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        filePaths = buffer;
        filePaths.erase(filePaths.find_last_not_of(" \n\r\t") + 1);
    }

    pclose(pipe);

    if (!filePaths.empty()) {
        auto files = std::make_shared<slint::VectorModel<slint::SharedString>>();
        std::stringstream ss(filePaths);
        std::string file;
        while (std::getline(ss, file, '|')) {
            files->push_back(slint::SharedString(file));
        }

        return files;
    }

    return {};
}

slint::SharedString pick_directory() {
    const std::string pick_command = R"(zenity --file-selection --directory --title="Choose a directory")";

    FILE* pipe = popen(pick_command.c_str(), "r");

    if (!pipe) {
        return {};
    }

    std::string directoryPath;

    if (char buffer[1024]; fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        directoryPath = buffer;
        directoryPath.erase(directoryPath.find_last_not_of(" \n\r\t") + 1);
    }

    pclose(pipe);

    if (!directoryPath.empty()) {
        return {directoryPath};
    }

    return {};
}
