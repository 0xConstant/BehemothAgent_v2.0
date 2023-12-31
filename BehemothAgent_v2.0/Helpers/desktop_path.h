#pragma once

#include <filesystem>
#include <string>


/// <summary>
/// This function is used to the retrieve the full Desktop path for the current user.
/// </summary>
/// <returns></returns>
std::filesystem::path getDesktopPath() {
    char* userProfile = nullptr;
    size_t userProfileSize = 0;
    _dupenv_s(&userProfile, &userProfileSize, "USERPROFILE");
    if (userProfile == nullptr) {
        free(userProfile); 
        return std::filesystem::path();
    }
    std::filesystem::path desktopPath = std::string(userProfile) + "\\Desktop";
    free(userProfile);
    return desktopPath;
}

