#ifndef CHECK_PERMS_H
#define CHECK_PERMS_H

#include <windows.h>
#include <Aclapi.h>
#include <AccCtrl.h>
#include <Sddl.h>
#include <iostream>

bool checkPerms(const std::wstring& path, DWORD accessRights);


#endif
