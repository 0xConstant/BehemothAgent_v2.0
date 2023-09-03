#include <Windows.h>
#include <iostream>
#include "file_search.h"


int main() {
    std::vector<std::wstring> fileTypes = { L".txt", L".jpg" };
    std::unordered_set<std::wstring> forbiddenDirs = { L"System Volume Information", L"Windows", L"Program Files"};

    std::vector<std::wstring> files = FileSearcher(L"C:\\", fileTypes, forbiddenDirs);

    for (const auto& file : files)
    {
        std::wcout << file << std::endl;
    }

    return 0;
}
