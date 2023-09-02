#include "disk_id.h"


std::map<std::string, std::vector<std::wstring>> Disk_ID()
{
    std::map<std::string, std::vector<std::wstring>> dict;
    std::vector<std::wstring> ldisks;
    std::vector<std::wstring> ndisks;
    std::vector<std::wstring> odisks;

    DWORD dwSize = MAX_PATH;
    wchar_t szLogicalDrives[MAX_PATH] = { 0 };
    DWORD dwResult = GetLogicalDriveStrings(dwSize, szLogicalDrives);

    if (dwResult > 0 && dwResult <= MAX_PATH)
    {
        wchar_t* szSingleDrive = szLogicalDrives;
        while (*szSingleDrive)
        {
            UINT driveType = GetDriveType(szSingleDrive);

            if (driveType == DRIVE_FIXED && CheckPerms(szSingleDrive, GENERIC_READ))
            {
                ldisks.push_back(szSingleDrive);
            }
            else if (driveType == DRIVE_REMOTE && CheckPerms(szSingleDrive, GENERIC_READ))
            {
                ndisks.push_back(szSingleDrive);
            }
            else if (driveType != DRIVE_FIXED && driveType != DRIVE_REMOTE && CheckPerms(szSingleDrive, GENERIC_READ))
            {
                odisks.push_back(szSingleDrive);
            }

            // Get the next drive
            szSingleDrive += wcslen(szSingleDrive) + 1;
        }
    }

    dict["logical"] = ldisks;
    dict["network"] = ndisks;
    dict["others"] = odisks;

    return dict;
}