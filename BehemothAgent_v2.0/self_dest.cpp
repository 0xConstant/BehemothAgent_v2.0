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

    // Write commands to the batch file to delete the executable and the batch file
    batchFile << "@echo off\n";
    batchFile << ":retry\n";
    batchFile << "del \"" << executablePath << "\"\n";
    batchFile << "if exist \"" << executablePath << "\" goto retry\n";
    batchFile << "del \"%~f0\"\n";
    batchFile.close();

    // Start the batch file
    system("start delete.bat");

    // Exit the program
    exit(0);
}