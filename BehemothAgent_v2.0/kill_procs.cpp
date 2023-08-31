#include "kill_procs.h"
#include <windows.h>
#include <tlhelp32.h>
#include <string>


void KillProcs() {
    DWORD currentProcessID = GetCurrentProcessId();
    HWND currentWindow = GetForegroundWindow();

    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

    if (Process32First(snapshot, &entry)) {
        while (Process32Next(snapshot, &entry)) {
            if (entry.th32ProcessID != currentProcessID) {
                HWND window = FindWindow(NULL, entry.szExeFile);
                if (window) {
                    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, entry.th32ProcessID);
                    if (hProcess) {
                        TerminateProcess(hProcess, 0);
                        CloseHandle(hProcess);
                    }
                }
            }
        }
    }

    CloseHandle(snapshot);
}
