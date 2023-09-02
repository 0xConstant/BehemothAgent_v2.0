#include <Windows.h>
#include <iostream>
#include "sendrequest.h"
#include "check_perms.h"
#include "disk_id.h"


#pragma comment(lib, "iphlpapi.lib")


int main() {
    std::map<std::string, std::vector<std::wstring>> drives = Disk_ID();

    std::wcout << L"Logical Drives:" << std::endl;
    for (const auto& drive : drives["logical"])
    {
        std::wcout << drive << std::endl;
    }

    std::wcout << L"Network Drives:" << std::endl;
    for (const auto& drive : drives["network"])
    {
        std::wcout << drive << std::endl;
    }

    std::wcout << L"Other Drives:" << std::endl;
    for (const auto& drive : drives["others"])
    {
        std::wcout << drive << std::endl;
    }

    return 0;
}
