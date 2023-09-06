#include <Windows.h>
#include <iostream>
#include <fstream>
#include <shellapi.h> 
#include "self_dest.h"


void self_destruct() {
    char executablePath[MAX_PATH];
    GetModuleFileNameA(NULL, executablePath, MAX_PATH);

    std::string command = "/C timeout 10 & del \"" + std::string(executablePath) + "\"";

    ShellExecuteA(NULL, "open", "cmd.exe", command.c_str(), NULL, SW_HIDE);
    exit(0);
}

