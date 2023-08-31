#include <iostream>
#include "gen_uid.h"


int main()
{
    std::string uid = GenUID();
    std::cout << "UID: " << uid << std::endl;
    return 0;
}
