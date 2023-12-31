#ifndef WSTRINGTOSTRING_HPP
#define WSTRINGTOSTRING_HPP

#include <string>
#include <windows.h>


// Inline function to convert a wide string (UTF-16) to a narrow string (UTF-8)
inline std::string WStringToString(const std::wstring& wstr)
{
    // If the input wstring is empty, return an empty narrow string
    if (wstr.empty())
        return std::string();

    // Calculate the number of bytes required to store the converted string in UTF-8 format:
    int sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);

    // Allocate a narrow string of the required size:
    std::string wsTS(sizeNeeded, 0);

    // Convert the wide string to a narrow string, storing the result in strTo
    WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &wsTS[0], sizeNeeded, NULL, NULL);

    // Return the converted narrow string
    return wsTS;
}


#endif 
