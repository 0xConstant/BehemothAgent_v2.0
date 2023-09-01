#include <iostream>
#include "gen_uid.h"
#include "kill_procs.h"
#include "gen_str.h"


int main()
{
    std::string randomstr = GenRandomStr(32);
    std::cout << randomstr << std::endl;
    return 0;
}
