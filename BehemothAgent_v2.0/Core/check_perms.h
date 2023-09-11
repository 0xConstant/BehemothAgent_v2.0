#ifndef CHECK_PERMS_H
#define CHECK_PERMS_H

#include <iostream>
#include <windows.h>


/// <summary>
/// This function is used for verifying write access over a file.
/// It is utilized during file search to only identify writable files.
/// </summary>
/// <param name="path"></param>
/// <param name="accessRights"></param>
/// <returns></returns>
bool CheckPerms(const std::wstring& path, DWORD accessRights);


#endif
