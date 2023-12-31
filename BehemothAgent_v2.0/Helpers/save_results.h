#pragma once  
#include <string>
#include <vector>


/// <summary>
/// This function saves a given list of file paths to a file.
/// It is used in conjunction with FileSearcher to search files writable
/// and save their paths to a text file.
/// </summary>
/// <param name="filePath"></param>
/// <param name="files"></param>
void SaveResultsToFile(const std::wstring& filePath, const std::vector<std::wstring>& files);
