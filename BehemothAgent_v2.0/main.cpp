#include <Windows.h>
#include <iostream>
#include "c2_conn.h"

#pragma comment(lib, "iphlpapi.lib")


int main() {
    std::vector<std::string> urls = { "www.hfsjkdfgsjhserk.com", "www.google.com", "www.example.com" };
    std::string connectedWebsite = c2_conn(urls);
    if (!connectedWebsite.empty()) {
        std::cout << "Connected to " << connectedWebsite << std::endl;
    }
    else {
        std::cerr << "Unable to connect to any website" << std::endl;
    }

    return 0;
}

