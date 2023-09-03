#ifndef C2_CONN_H
#define C2_CONN_H

#include <windows.h>
#include <winhttp.h>
#include <iostream>
#include <algorithm>
#include <vector>

#pragma comment(lib, "winhttp.lib")


std::wstring SendRequest(const std::vector<std::wstring>& urls);

#endif
