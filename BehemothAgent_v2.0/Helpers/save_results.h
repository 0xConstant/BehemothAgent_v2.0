#pragma once  // ensures that the header is included only once during compilation
#include <string>
#include <vector>

// Function declaration
void SaveResultsToFile(const std::wstring& filePath, const std::vector<std::wstring>& files);
