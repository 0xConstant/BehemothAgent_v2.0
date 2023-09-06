#ifndef CHECK_PERMS_H
#define CHECK_PERMS_H

#include <iostream>
#include <windows.h>

bool CheckPerms(const std::wstring& path, DWORD accessRights);


#endif
