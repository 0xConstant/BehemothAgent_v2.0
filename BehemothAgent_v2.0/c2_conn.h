#ifndef C2_CONN_H
#define C2_CONN_H

#include <Windows.h>
#include <winhttp.h>
#include <vector>
#include <string>
#include <thread>
#include <chrono>

#pragma comment(lib, "winhttp.lib")


std::string c2_conn(const std::vector<std::string>& urls);

#endif
