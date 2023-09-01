#include <iostream>
#include <Windows.h>
#include "gen_uid.h"
#include "kill_procs.h"
#include "gen_str.h"
#include "del_shadows.h"
#include "msg_screen.h"
#include "create_readmes.h"


/*
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    return ShowFullScreenWindow(hInstance, nCmdShow);
}
*/

int main() {
    std::string text = "This is the text to write to the file.";
    std::string filename = "behemoth_readme.txt";
    create_readmes(text, filename);
    return 0;
}

