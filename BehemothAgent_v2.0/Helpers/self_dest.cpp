#include <Windows.h>
#include <iostream>
#include <fstream>
#include <shellapi.h> 
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
    // wait for 10 seconds for the application to exit
    batchFile << "timeout /T 10\n";  
    batchFile << ":retry\n";
    // delete the current executable
    batchFile << "del \"" << executablePath << "\"\n";
    batchFile << "if exist \"" << executablePath << "\" goto retry\n";
    // Delete the batch file itself
    batchFile << "del \"%~f0\"\n";  
    batchFile.close();

    // Start the batch file hidden using ShellExecute
    ShellExecuteA(NULL, "open", "delete.bat", NULL, NULL, SW_HIDE);

    // Exit the program
    exit(0);
}

