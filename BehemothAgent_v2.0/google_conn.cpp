#include "google_conn.h"


bool googleConn() {
    HANDLE hIcmp;
    unsigned long ipaddr = INADDR_NONE;
    DWORD dwRetVal = 0;
    char SendData[] = "Data Buffer";
    LPVOID ReplyBuffer = nullptr;
    DWORD ReplySize = 0;
    int timeout = 10000; // 10 seconds
    int retry = 3;
    int interval = 1; // 1 minute

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

    while (true) {
        for (int i = 0; i < retry; i++) {
            dwRetVal = IcmpSendEcho(hIcmp, ipaddr, SendData, sizeof(SendData),
                nullptr, ReplyBuffer, ReplySize, timeout);

            if (dwRetVal != 0) {
                PICMP_ECHO_REPLY pEchoReply = (PICMP_ECHO_REPLY)ReplyBuffer;
                struct in_addr ReplyAddr;
                ReplyAddr.S_un.S_addr = pEchoReply->Address;
                // std::cout << "Sent icmp message to " << inet_ntoa(ReplyAddr) << " received " << dwRetVal << " replies" << std::endl;
                free(ReplyBuffer);
                return true;
            }
            else {
                std::cerr << "IcmpSendEcho failed: " << GetLastError() << std::endl;
            }
        }
        Sleep(interval * 60 * 1000);
    }

    free(ReplyBuffer);
    return false;
}