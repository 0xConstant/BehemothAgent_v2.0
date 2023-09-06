#ifndef LINE_COUNT_HPP
#define LINE_COUNT_HPP

#include <string>
#include <fstream>

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
