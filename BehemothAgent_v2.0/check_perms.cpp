#include <AccCtrl.h>
#include <Sddl.h>
#include <Aclapi.h>
#include <windows.h>
#include "check_perms.h"


bool CheckPerms(const std::wstring& path, DWORD accessRights)
{
    if (path.empty()) return false;

    HANDLE hFile = CreateFile(path.c_str(), accessRights, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        DWORD error = GetLastError();
        if (error == ERROR_ACCESS_DENIED)
        {
            CloseHandle(hFile);
            return false;
        }
    }

    CloseHandle(hFile);
    return true;
}
