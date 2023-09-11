#ifndef FILE_SEARCH_H
#define FILE_SEARCH_H

#include <vector>
#include <string>
#include <unordered_set>


/// <summary>
/// This function is used to recursively search for files. 
/// It takes a path for a directory, a list of file types and names for forbidden folders.
/// Forbidden folders are ignored and they usually include directories like "C"\Windows",
/// file types are used to identify specific files like documents, PDF archives and databases.
/// The function performs a recursive file search and enumerates all writable files.
/// </summary>
/// <param name="dir"></param>
/// <param name="fileTypes"></param>
/// <param name="forbiddenDirs"></param>
/// <returns></returns>
std::vector<std::wstring> FileSearcher(const std::wstring& dir, const std::vector<std::wstring>& fileTypes, const std::unordered_set<std::wstring>& forbiddenDirs);

#endif 
