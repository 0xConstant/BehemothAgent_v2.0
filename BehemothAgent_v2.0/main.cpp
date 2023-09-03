#include <iostream>
#include <Windows.h>
#include "c2_conn.h"


int main() {
    std::wstring url = L"https://10.0.0.113:5000/check-payment";
    int statusCode = SendRequest(url);

    std::wcout << L"Status Code: " << statusCode << std::endl;

    return 0;
}
