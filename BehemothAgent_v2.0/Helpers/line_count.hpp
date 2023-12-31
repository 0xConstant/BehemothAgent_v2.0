#ifndef LINE_COUNT_HPP
#define LINE_COUNT_HPP

#include <string>
#include <fstream>


/// <summary>
/// This function is used to get a count of the number of non-empty lines in a file.
/// It's used for getting a count of writable files.
/// </summary>
/// <param name="filePath"></param>
/// <returns></returns>
inline std::string countNonEmptyLines(const std::wstring& filePath) {
    std::wifstream file(filePath);

    size_t lineCount = 0;
    std::wstring line;

    while (std::getline(file, line)) {
        if (!line.empty() && line.find_first_not_of(L' ') != std::wstring::npos) {
            ++lineCount;
        }
    }

    return std::to_string(lineCount);
}

#endif
