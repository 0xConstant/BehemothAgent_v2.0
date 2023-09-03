#ifndef C2_CONN_H
#define C2_CONN_H

#include <windows.h>
#include <winhttp.h>
#include <iostream>
#include <algorithm>

#pragma comment(lib, "winhttp.lib")


int SendRequest(const std::wstring& url);

#endif
