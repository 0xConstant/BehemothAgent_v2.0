#include <iostream>
#include "gen_uid.h"
#include "kill_procs.h"
#include "gen_str.h"
#include "del_shadows.h"


int main()
{
    std::cout << nuke_vss() << std::endl;
    return 0;
}
