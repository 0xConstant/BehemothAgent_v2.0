#ifndef DISK_ID_H
#define DISK_ID_H

#include <windows.h>
#include <map>
#include <vector>
#include <string>


/// <summary>
/// This function is used to identify and categorize connected disks/drives.
/// It then performs READ permissions over each drive to ensure that we can
/// enumerate files.
/// </summary>
/// <returns></returns>
std::map<std::string, std::vector<std::wstring>> Disk_ID();
bool CheckPerms(const std::wstring& path, DWORD accessRights);

#endif 
