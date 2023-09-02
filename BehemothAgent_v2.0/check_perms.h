#ifndef CHECK_PERMS_H
#define CHECK_PERMS_H

#include <windows.h>
#include <Aclapi.h>
#include <AccCtrl.h>
#include <Sddl.h>
#include <iostream>

bool CheckPerms(const std::wstring& path, DWORD accessRights);


#endif
