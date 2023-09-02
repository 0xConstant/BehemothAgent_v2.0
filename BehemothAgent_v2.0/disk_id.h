#ifndef DISK_ID_H
#define DISK_ID_H

#include <windows.h>
#include <map>
#include <vector>
#include <string>


std::map<std::string, std::vector<std::wstring>> Disk_ID();
bool CheckPerms(const std::wstring& path, DWORD accessRights);

#endif // DISKIDENTIFICATION_H
