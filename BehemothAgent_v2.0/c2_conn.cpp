#include "c2_conn.h"


int SendRequest(const std::wstring& url)
{
    HINTERNET hSession = NULL, hConnect = NULL, hRequest = NULL;
    int statusCode = -1;
    DWORD statusCodeSize = sizeof(statusCode);

    URL_COMPONENTS urlComp;
    ZeroMemory(&urlComp, sizeof(urlComp));
    urlComp.dwStructSize = sizeof(urlComp);
    urlComp.dwSchemeLength = (DWORD)-1;
    urlComp.dwHostNameLength = (DWORD)-1;
    urlComp.dwUrlPathLength = (DWORD)-1;
    urlComp.dwExtraInfoLength = (DWORD)-1;

    if (!WinHttpCrackUrl(url.c_str(), (DWORD)url.length(), 0, &urlComp))
    {
        std::wcout << L"Error in WinHttpCrackUrl: " << GetLastError() << std::endl;
        return -1;
    }

    if (urlComp.nScheme != INTERNET_SCHEME_HTTP && urlComp.nScheme != INTERNET_SCHEME_HTTPS)
    {
        std::wcout << L"Error: Unsupported scheme in the URL." << std::endl;
        return -1;
    }

    std::wstring hostname, path;
    INTERNET_PORT port;
    size_t pos = url.find(L"://");
    if (pos != std::wstring::npos)
    {
        size_t start = pos + 3; // Skip ://
        size_t end = url.find(L":", start);
        if (end != std::wstring::npos)
        {
            hostname = url.substr(start, end - start);
            unsigned int tempPort;
            const wchar_t* startOfPort = url.c_str() + end + 1;
            wchar_t* endOfPort;
            port = (INTERNET_PORT)std::wcstoul(startOfPort, &endOfPort, 10);
        }
        else
        {
            end = url.find(L"/", start);
            if (end != std::wstring::npos)
            {
                hostname = url.substr(start, end - start);
                port = (urlComp.nScheme == INTERNET_SCHEME_HTTPS) ? INTERNET_DEFAULT_HTTPS_PORT : INTERNET_DEFAULT_HTTP_PORT;
                path = url.substr(end);
            }
            else
            {
                hostname = url.substr(start);
                port = (urlComp.nScheme == INTERNET_SCHEME_HTTPS) ? INTERNET_DEFAULT_HTTPS_PORT : INTERNET_DEFAULT_HTTP_PORT;
            }
        }
    }
    //
    do
    {
        hSession = WinHttpOpen(NULL, WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
        if (!hSession)
        {
            std::wcout << L"Error in WinHttpOpen: " << GetLastError() << std::endl;
            break;
        }

        hConnect = WinHttpConnect(hSession, hostname.c_str(), port, 0);
        if (!hConnect)
        {
            std::wcout << L"Error in WinHttpConnect: " << GetLastError() << std::endl;
            break;
        }

        hRequest = WinHttpOpenRequest(hConnect, L"GET", path.c_str(), NULL, WINHTTP_NO_REFERER,
            WINHTTP_DEFAULT_ACCEPT_TYPES, urlComp.nScheme == INTERNET_SCHEME_HTTPS ? WINHTTP_FLAG_SECURE : 0);
        if (!hRequest)
        {
            std::wcout << L"Error in WinHttpOpenRequest: " << GetLastError() << std::endl;
            break;
        }

        // Ignore SSL certificate errors
        DWORD dwFlags = SECURITY_FLAG_IGNORE_UNKNOWN_CA | SECURITY_FLAG_IGNORE_CERT_WRONG_USAGE |
            SECURITY_FLAG_IGNORE_CERT_CN_INVALID | SECURITY_FLAG_IGNORE_CERT_DATE_INVALID;
        if (!WinHttpSetOption(hRequest, WINHTTP_OPTION_SECURITY_FLAGS, &dwFlags, sizeof(DWORD)))
        {
            std::wcout << L"Error in WinHttpSetOption: " << GetLastError() << std::endl;
            break;
        }

        if (!WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0))
        {
            std::wcout << L"Error in WinHttpSendRequest: " << GetLastError() << std::endl;
            break;
        }

        if (!WinHttpReceiveResponse(hRequest, NULL))
        {
            std::wcout << L"Error in WinHttpReceiveResponse: " << GetLastError() << std::endl;
            break;
        }

        if (!WinHttpQueryHeaders(hRequest, WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER, WINHTTP_HEADER_NAME_BY_INDEX,
            &statusCode, &statusCodeSize, WINHTTP_NO_HEADER_INDEX))
        {
            std::wcout << L"Error in WinHttpQueryHeaders: " << GetLastError() << std::endl;
            break;
        }
    } while (0);

    if (hRequest) WinHttpCloseHandle(hRequest);
    if (hConnect) WinHttpCloseHandle(hConnect);
    if (hSession) WinHttpCloseHandle(hSession);

    return statusCode;
}




