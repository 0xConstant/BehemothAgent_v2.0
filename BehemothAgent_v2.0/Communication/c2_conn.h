#ifndef C2_CONN_H
#define C2_CONN_H

#include <vector>
#include <iostream>

#pragma comment(lib, "winhttp.lib")


std::wstring c2_conn(const std::vector<std::wstring>& urls);

#endif
