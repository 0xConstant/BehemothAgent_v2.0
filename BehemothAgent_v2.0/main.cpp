#include <iostream>
#include <Windows.h>
#include "gen_uid.h"
#include "kill_procs.h"
#include "gen_str.h"
#include "del_shadows.h"
#include "msg_screen.h"
#include "create_readmes.h"
#include "self_dest.h"
#include "system_id.h"
#include "profiler.h"
#include "merge_dics.h"


/*
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    return ShowFullScreenWindow(hInstance, nCmdShow);
}
*/

int main() {
    std::unordered_map<std::string, int> dict1 = { {"a", 1}, {"b", 2} };
    std::unordered_map<std::string, int> dict2 = { {"b", 3}, {"c", 4} };
    std::vector<std::unordered_map<std::string, int>> dicts = { dict1, dict2 };
    auto mergedDict = merge_dics(dicts);
    for (const auto& kvp : mergedDict) {
        std::cout << kvp.first << ": " << kvp.second << std::endl;
    }

    return 0;
}

