#include <iostream>
#include <Windows.h>
#include "c2_conn.h"


int main() {
    // Create a list of URLs to check
    std::vector<std::wstring> urls = { L"http://www.fhfdhsdffg.com", L"https://www.dsfsdgf.com", L"https://10.0.0.113:5000"};

    // Call the function
    std::wstring validUrl = SendRequest(urls);

    // Check the result
    if (!validUrl.empty())
    {
        std::wcout << L"Valid URL found: " << validUrl << std::endl;
    }
    else
    {
        std::cout << 0 << std::endl;
    }

    return 0;
}
