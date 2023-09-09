#include <windows.h>
#include <tlhelp32.h>
#include <string>
#include "kill_procs.h"



// Callback function to enumerate through windows
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
    DWORD lpdwProcessId;
    // Get the process ID for the window handle:
    GetWindowThreadProcessId(hwnd, &lpdwProcessId);

    // If the process ID matches the target, return FALSE to stop the enumeration:
    if (lpdwProcessId == lParam) {
        return FALSE;
    }

    return TRUE;
}


bool HasWindow(DWORD processID) {
    // Enumerate through all windows and check if any belong to the given process ID
    return !EnumWindows(EnumWindowsProc, processID);
}


// Function to kill processes that have an associated window, excluding the current one
void KillProcs() {
    // Get the current process ID to avoid killing it
    DWORD currentProcessID = GetCurrentProcessId();

    // Structure to hold process entry details
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);

    // Create a snapshot of all running processes
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

    // If we are able to get the first process from the snapshot:
    if (Process32First(snapshot, &entry)) {
        // Continue enumerating through all processes:
        while (Process32Next(snapshot, &entry)) {
            // If the process isn't the current one and it has a window:
            if (entry.th32ProcessID != currentProcessID && HasWindow(entry.th32ProcessID)) {
                // Attempt to open the process with terminate rights:
                HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, entry.th32ProcessID);
                if (hProcess) {
                    // Kill the process:
                    TerminateProcess(hProcess, 0);
                    // Close the process handle:
                    CloseHandle(hProcess);
                }
            }
        }
    }

    // Close the snapshot handle:
    CloseHandle(snapshot);
}
