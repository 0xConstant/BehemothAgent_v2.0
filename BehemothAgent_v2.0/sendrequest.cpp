#include "sendrequest.h"


std::string sendrequest(const std::wstring& server, const std::wstring& path, const nlohmann::json& jsonData)
{
    DWORD dwSize = 0;
    DWORD dwDownloaded = 0;
    HINTERNET  hSession = NULL, hConnect = NULL, hRequest = NULL;
    BOOL  bResults = FALSE;
    std::string responseJson;

    // Use WinHttpOpen to obtain a session handle.
    hSession = WinHttpOpen(L"WinHTTP Example/1.0",
        WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
        WINHTTP_NO_PROXY_NAME,
        WINHTTP_NO_PROXY_BYPASS, 0);

    // Specify an HTTP server.
    if (hSession)
        hConnect = WinHttpConnect(hSession, server.c_str(),
            INTERNET_DEFAULT_HTTP_PORT, 0);

    // Create an HTTP request handle.
    if (hConnect)
        hRequest = WinHttpOpenRequest(hConnect, L"POST", path.c_str(),
            NULL, WINHTTP_NO_REFERER,
            WINHTTP_DEFAULT_ACCEPT_TYPES,
            0);

    // Add the Content-Type header
    if (hRequest)
        bResults = WinHttpAddRequestHeaders(hRequest,
            L"Content-Type: application/json",
            (DWORD)-1, WINHTTP_ADDREQ_FLAG_ADD);

    // Send a request.
    if (bResults) {
        std::string jsonString = jsonData.dump();
        bResults = WinHttpSendRequest(hRequest,
            WINHTTP_NO_ADDITIONAL_HEADERS,
            0, (LPVOID)jsonString.c_str(),
            jsonString.size(),
            jsonString.size(), 0);
    }

    // End the request.
    if (bResults)
        bResults = WinHttpReceiveResponse(hRequest, NULL);

    // Get the response status code
    if (bResults)
    {
        DWORD dwStatusCode = 0;
        dwSize = sizeof(dwStatusCode);
        WinHttpQueryHeaders(hRequest, WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER,
            NULL, &dwStatusCode, &dwSize, NULL);
        std::wcout << L"Status code: " << dwStatusCode << std::endl;
    }

    // Keep checking for data until there is nothing left.
    if (bResults)
    {
        do
        {
            // Check for available data.
            dwSize = 0;
            if (!WinHttpQueryDataAvailable(hRequest, &dwSize))
                printf("Error %u in WinHttpQueryDataAvailable.\n",
                    GetLastError());

            // Allocate space for the buffer.
            char* pszOutBuffer = new char[dwSize + 1];
            if (!pszOutBuffer)
            {
                printf("Out of memory\n");
                dwSize = 0;
            }
            else
            {
                // Read the data.
                ZeroMemory(pszOutBuffer, dwSize + 1);

                if (!WinHttpReadData(hRequest, (LPVOID)pszOutBuffer,
                    dwSize, &dwDownloaded))
                    printf("Error %u in WinHttpReadData.\n", GetLastError());
                else
                    responseJson += pszOutBuffer;

                // Free the memory allocated to the buffer.
                delete[] pszOutBuffer;
            }
        } while (dwSize > 0);
    }

    // Close any open handles.
    if (hRequest) WinHttpCloseHandle(hRequest);
    if (hConnect) WinHttpCloseHandle(hConnect);
    if (hSession) WinHttpCloseHandle(hSession);

    return responseJson;
}


