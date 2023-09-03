#ifndef SENDREQUEST_H
#define SENDREQUEST_H

#include "json.hpp"
#include <windows.h>
#include <winhttp.h>
#include <iostream>


#pragma comment(lib, "winhttp.lib")

std::string sendrequest(const std::wstring& fullUrl, const nlohmann::json& jsonData);

#endif
