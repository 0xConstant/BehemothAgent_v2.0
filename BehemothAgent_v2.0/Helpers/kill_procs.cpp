#include <windows.h>
#include <tlhelp32.h>
#include <string>
#include "kill_procs.h"



BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
    DWORD lpdwProcessId;
    GetWindowThreadProcessId(hwnd, &lpdwProcessId);
    if (lpdwProcessId == lParam) {
        return FALSE;
    }
    return TRUE;
}


bool HasWindow(DWORD processID) {
    return !EnumWindows(EnumWindowsProc, processID);
}


void KillProcs() {
    DWORD currentProcessID = GetCurrentProcessId();

    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

    if (Process32First(snapshot, &entry)) {
        while (Process32Next(snapshot, &entry)) {
            // Ignoring cmd.exe and your application's process
            if (wcscmp(entry.szExeFile, L"cmd.exe") == 0 || entry.th32ProcessID == currentProcessID) {
                continue;
            }

            if (HasWindow(entry.th32ProcessID)) {
                HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, entry.th32ProcessID);
                if (hProcess) {
                    TerminateProcess(hProcess, 0);
                    CloseHandle(hProcess);
                }
            }
        }
    }

    CloseHandle(snapshot);
}


