#include "convert_to_wstring.h"
#include <windows.h> 


std::wstring ConvertToWString(const char* str) {
    if (!str) return L"";

    int size = MultiByteToWideChar(CP_UTF8, 0, str, -1, nullptr, 0);
    std::wstring wide_str(size, 0);
    MultiByteToWideChar(CP_UTF8, 0, str, -1, &wide_str[0], size);

    return wide_str;
}
