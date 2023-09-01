#include <Windows.h>
#include <iostream>
#include "sendrequest.h"
#include "check_perms.h"

#pragma comment(lib, "iphlpapi.lib")


int main() {
    std::wstring path = L"D:\\file_2.txt";
    DWORD accessRights = GENERIC_WRITE;
    bool result = checkPerms(path, accessRights);
    std::wcout << (result ? L"Access Allowed" : L"Access Denied") << std::endl;

    return 0;
}
