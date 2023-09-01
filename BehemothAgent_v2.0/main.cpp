#include <Windows.h>
#include <iostream>
#include "sendrequest.h"

#pragma comment(lib, "iphlpapi.lib")


int main() {
    std::wstring server = L"10.0.0.113:3000";
    std::wstring path = L"/fact";
    nlohmann::json jsonData;
    jsonData["data"] = "value1";
    std::string response = sendrequest(server, path, jsonData);
    std::cout << "Response: " << response << std::endl;

    return 0;
}
