#include <windows.h>
#include <iostream>
#include <iphlpapi.h>
#include <icmpapi.h>
#include "google_conn.h"

#pragma comment(lib, "iphlpapi.lib")


bool googleConn() {
    HANDLE hIcmp;
    unsigned long ipaddr = INADDR_NONE;
    DWORD dwRetVal = 0;
    char SendData[] = "Data Buffer";
    LPVOID ReplyBuffer = nullptr;
    DWORD ReplySize = 0;
    int timeout = 10000; // 10 seconds
    int retry = 3;

    ipaddr = inet_addr("8.8.8.8");  // Google DNS
    hIcmp = IcmpCreateFile();
    if (hIcmp == INVALID_HANDLE_VALUE) {
        std::cerr << "IcmpCreateFile failed: " << GetLastError() << std::endl;
        return false;
    }

    ReplySize = sizeof(ICMP_ECHO_REPLY) + sizeof(SendData);
    ReplyBuffer = (VOID*)malloc(ReplySize);
    if (ReplyBuffer == nullptr) {
        std::cerr << "Unable to allocate memory" << std::endl;
        return false;
    }

    for (int i = 0; i < retry; i++) {
        dwRetVal = IcmpSendEcho(hIcmp, ipaddr, SendData, sizeof(SendData),
            nullptr, ReplyBuffer, ReplySize, timeout);

        if (dwRetVal != 0) {
            PICMP_ECHO_REPLY pingReply = (PICMP_ECHO_REPLY)ReplyBuffer;
            if (pingReply->Status == IP_SUCCESS) {
                free(ReplyBuffer);
                return true;
            }
            else {
                // std::cerr << "Ping returned with status: " << pingReply->Status << std::endl;
            }
        }
    }

    free(ReplyBuffer);
    return false;
}
