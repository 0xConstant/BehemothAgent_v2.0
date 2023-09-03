#ifndef FILE_SEARCH_H
#define FILE_SEARCH_H

#include <vector>
#include <string>
#include <unordered_set>
#include <windows.h>

// Declare the function in the header file
std::vector<std::wstring> FileSearcher(const std::wstring& dir, const std::vector<std::wstring>& fileTypes, const std::unordered_set<std::wstring>& forbiddenDirs);

#endif // FILE_SEARCHER_H
