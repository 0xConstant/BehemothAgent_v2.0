#include <iostream>
#include <Windows.h>
#include "sendrequest.h"


int main() {
    nlohmann::json jsonData;
    jsonData["data"] = "Hello, World!";

    // Call your function
    std::wstring url = L"http://10.0.0.113:3000/jsoncpp.php";
    std::string response = sendrequest(url, jsonData);

    // Check the response
    std::cout << "Response: " << response << std::endl;

    return 0;
}
