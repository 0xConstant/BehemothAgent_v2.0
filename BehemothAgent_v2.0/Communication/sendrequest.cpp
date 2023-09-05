#include "sendrequest.h"


std::string sendrequest(const std::wstring& fullUrl, const nlohmann::json& jsonData)
{
    DWORD dwSize = 0;
    DWORD dwDownloaded = 0;
    DWORD dwFlags = 0;
    DWORD dwTimeout = 10 * 1000; // 10 seconds in milliseconds
    HINTERNET  hSession = NULL, hConnect = NULL, hRequest = NULL;
    BOOL  bResults = FALSE;
    std::string responseJson;
    URL_COMPONENTS urlComponents;
    ZeroMemory(&urlComponents, sizeof(urlComponents));
    urlComponents.dwStructSize = sizeof(urlComponents);
    urlComponents.dwSchemeLength = (DWORD)-1;
    urlComponents.dwHostNameLength = (DWORD)-1;
    urlComponents.dwUrlPathLength = (DWORD)-1;
    urlComponents.dwExtraInfoLength = (DWORD)-1;
    urlComponents.lpszHostName = new wchar_t[fullUrl.length()];
    urlComponents.lpszUrlPath = new wchar_t[fullUrl.length()];

    // Crack the URL.
    if (!WinHttpCrackUrl(fullUrl.c_str(), (DWORD)wcslen(fullUrl.c_str()), 0, &urlComponents))
    {
        printf("Error %u in WinHttpCrackUrl.\n", GetLastError());
        goto cleanup;
    }
    // wprintf(L"URL Components: HostName: %s, UrlPath: %s, Port: %d\n", urlComponents.lpszHostName, urlComponents.lpszUrlPath, urlComponents.nPort);

    // Use WinHttpOpen to obtain a session handle.
    hSession = WinHttpOpen(L"WinHTTP Example/1.0",
        WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
        WINHTTP_NO_PROXY_NAME,
        WINHTTP_NO_PROXY_BYPASS, 0);

    // Specify an HTTP server.
    if (hSession)
        hConnect = WinHttpConnect(hSession, urlComponents.lpszHostName,
            urlComponents.nPort, 0);
    if (!hConnect) {
        printf("Error %u in WinHttpConnect.\n", GetLastError());
        goto cleanup;
    }

    // Create an HTTP request handle.
    if (hConnect)
        hRequest = WinHttpOpenRequest(hConnect, L"POST", urlComponents.lpszUrlPath,
            NULL, WINHTTP_NO_REFERER,
            WINHTTP_DEFAULT_ACCEPT_TYPES,
            urlComponents.nScheme == INTERNET_SCHEME_HTTPS ? WINHTTP_FLAG_SECURE : 0);
    if (!hRequest) {
        printf("Error %u in WinHttpOpenRequest.\n", GetLastError());
        goto cleanup;
    }

    // Set timeout
    if (hRequest)
        WinHttpSetOption(hRequest, WINHTTP_OPTION_RECEIVE_TIMEOUT, &dwTimeout, sizeof(dwTimeout));

    // Ignore SSL certificate errors.
    if (hRequest) {
        dwFlags |= SECURITY_FLAG_IGNORE_UNKNOWN_CA |
            SECURITY_FLAG_IGNORE_CERT_WRONG_USAGE |
            SECURITY_FLAG_IGNORE_CERT_CN_INVALID |
            SECURITY_FLAG_IGNORE_CERT_DATE_INVALID;
        WinHttpSetOption(
            hRequest,
            WINHTTP_OPTION_SECURITY_FLAGS,
            &dwFlags,
            sizeof(dwFlags)
        );
    }

    // Add headers.
    if (hRequest) {
        WinHttpAddRequestHeaders(hRequest, L"Content-Type: application/json", (DWORD)-1, WINHTTP_ADDREQ_FLAG_REPLACE);
    }

    // Send a request.
    if (hRequest) {
        std::string jsonString = jsonData.dump();
        // std::cout << "Sending JSON Data: " << jsonString << std::endl;
        bResults = WinHttpSendRequest(hRequest,
            WINHTTP_NO_ADDITIONAL_HEADERS, 0,
            (LPVOID)jsonString.c_str(),
            jsonString.size(),
            jsonString.size(), 0);
        if (!bResults) {
            printf("Error %u in WinHttpSendRequest.\n", GetLastError());
            goto cleanup;
        }
    }

    // End the request.
    if (bResults)
        bResults = WinHttpReceiveResponse(hRequest, NULL);

    // Keep checking for data until there is nothing left.
    if (bResults)
    {
        do
        {
            // Check for available data.
            dwSize = 0;
            if (!WinHttpQueryDataAvailable(hRequest, &dwSize))
            {
                printf("Error %u in WinHttpQueryDataAvailable.\n", GetLastError());
                goto cleanup;
            }

            // Allocate space for the buffer.
            std::vector<char> buffer(dwSize);
            if (!WinHttpReadData(hRequest, (LPVOID)buffer.data(), dwSize, &dwDownloaded))
            {
                printf("Error %u in WinHttpReadData.\n", GetLastError());
                goto cleanup;
            }
            else
                responseJson.append(buffer.data(), dwDownloaded);
        } while (dwSize > 0);
    }

cleanup:
    // Close any open handles.
    if (hRequest) WinHttpCloseHandle(hRequest);
    if (hConnect) WinHttpCloseHandle(hConnect);
    if (hSession) WinHttpCloseHandle(hSession);

    if (urlComponents.lpszHostName)
        delete[] urlComponents.lpszHostName;
    if (urlComponents.lpszUrlPath)
        delete[] urlComponents.lpszUrlPath;

    return responseJson;
}
