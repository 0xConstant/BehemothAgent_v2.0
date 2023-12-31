#include "save_results.h"
#include <fstream> 


void SaveResultsToFile(const std::wstring& filePath, const std::vector<std::wstring>& files) {
    std::wofstream outFile(filePath, std::ios_base::app);
    if (outFile.is_open()) {
        for (const auto& file : files) {
            outFile << file << std::endl;
        }
    }
}
