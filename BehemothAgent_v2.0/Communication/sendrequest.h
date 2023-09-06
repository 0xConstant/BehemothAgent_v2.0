#ifndef SENDREQUEST_H
#define SENDREQUEST_H

#include "../Helpers/json.hpp"

#pragma comment(lib, "winhttp.lib")

std::string sendrequest(const std::wstring& fullUrl, const nlohmann::json& jsonData);

#endif
