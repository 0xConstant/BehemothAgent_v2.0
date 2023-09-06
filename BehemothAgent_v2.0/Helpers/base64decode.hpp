#ifndef BASE64DECODE_HPP
#define BASE64DECODE_HPP

#include <windows.h>
#include <wincrypt.h>
#include <string>
#include <vector>

#pragma comment(lib, "crypt32.lib")


inline std::wstring s2ws(const std::string& str) {
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.size(), NULL, 0);
    std::wstring wstr(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.size(), &wstr[0], size_needed);
    return wstr;
}


std::string DecodeBase64(const std::string& base64String) {
    std::wstring wideBase64 = s2ws(base64String);

    DWORD decodedSize = 0;

    // Determine the required buffer size (without actually decoding).
    if (!CryptStringToBinaryW(wideBase64.c_str(), 0, CRYPT_STRING_BASE64, NULL, &decodedSize, NULL, NULL)) {
        return "";
    }

    std::vector<BYTE> decodedData(decodedSize);
    if (!CryptStringToBinaryW(wideBase64.c_str(), 0, CRYPT_STRING_BASE64, decodedData.data(), &decodedSize, NULL, NULL)) {
        return "";
    }

    return std::string(decodedData.begin(), decodedData.end());
}

#endif
