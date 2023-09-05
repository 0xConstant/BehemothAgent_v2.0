#pragma once

#include <filesystem>
#include <string>

std::filesystem::path getDesktopPath() {
    char* userProfile = nullptr;
    size_t userProfileSize = 0;
    _dupenv_s(&userProfile, &userProfileSize, "USERPROFILE");
    if (userProfile == nullptr) {
        free(userProfile);  // Always free resources when done
        return std::filesystem::path(); // Return an empty path
    }
    std::filesystem::path desktopPath = std::string(userProfile) + "\\Desktop";
    free(userProfile);
    return desktopPath;
}

