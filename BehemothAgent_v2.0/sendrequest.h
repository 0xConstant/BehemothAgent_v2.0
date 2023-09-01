#ifndef SENDREQUEST_H
#define SENDREQUEST_H

#include <windows.h>
#include <winhttp.h>
#include <iostream>
#include "json.hpp"

#pragma comment(lib, "winhttp.lib")

std::string sendrequest(const std::wstring& fullUrl, const nlohmann::json& jsonData);

#endif