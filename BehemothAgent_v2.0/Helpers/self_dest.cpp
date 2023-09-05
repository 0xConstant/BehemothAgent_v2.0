#include <Windows.h>
#include <iostream>
#include <fstream>
#include "self_dest.h"


#include <Windows.h>
#include <iostream>
#include <fstream>
#include <shellapi.h>  // Necessary for ShellExecute
#include "self_dest.h"

void self_destruct() {
    // Get the full path of the current executable
    char executablePath[MAX_PATH];
    GetModuleFileNameA(NULL, executablePath, MAX_PATH);

    // Create a temporary batch file
    std::ofstream batchFile("delete.bat");
    if (!batchFile) {
        std::cerr << "Could not create batch file.\n";
        return;
    }

    // Write commands to the batch file to delete the executable and then the batch file itself
    batchFile << "@echo off\n";
    batchFile << "timeout /T 5\n";  // wait for 5 seconds
    batchFile << ":retry\n";
    batchFile << "del \"" << executablePath << "\"\n";
    batchFile << "if exist \"" << executablePath << "\" goto retry\n";
    batchFile << "del \"%~f0\"\n";  // Delete the batch file itself
    batchFile.close();

    // Start the batch file hidden using ShellExecute
    ShellExecuteA(NULL, "open", "delete.bat", NULL, NULL, SW_HIDE);

    // Exit the program
    exit(0);
}
