#include <iostream>
#include <Windows.h>
#include "Communication/sendrequest.h"

int main() {
    nlohmann::json jsonData;
    jsonData["data"] = "Hello, World!";

    // Call your function
    std::wstring url = L"http://10.0.0.113:3000/jsoncpp.php";
    std::string response = sendrequest(url, jsonData);

    // Check the response
    std::cout << "Response: " << response << std::endl;

    // Check if the response is valid JSON
    try {
        nlohmann::json jsonResponse = nlohmann::json::parse(response);

        // Check if the response is of reasonable length
        if (response.length() < 1000) { // adjust this number based on your needs

            // Check if the "response" key is in the JSON response
            if (jsonResponse.find("response") != jsonResponse.end()) {
                std::cout << "Response is valid" << std::endl;
            }
            else {
                std::cout << "Response does not contain 'response' key" << std::endl;
            }

        }
        else {
            std::cout << "Response is too long" << std::endl;
        }

    }
    catch (nlohmann::json::parse_error&) {
        std::cout << "Response is not valid JSON" << std::endl;
    }

    return 0;
}
