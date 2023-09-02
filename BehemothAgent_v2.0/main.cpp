#include <Windows.h>
#include <iostream>
#include "file_enc.h"
#include "rsa_enc.h"


int main() {
    std::string base64PublicKey = "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEA47K3YD3OsBNUykIQeMwqvpPttBxfzoRT0O9z5tAq+Xu/Pq6a7rBbqHi7FqK0RbpNYBd0zKnz/dOSHqS6RNNB5L6eKUMBBb5zoHzWliDnigIA6FNQkObzx9TxZwIii8UXn3cPvi7Xl86w+wHpbOh9lU7SWAtrFuLPoqZTUWskJL1JKxh1zamH9aEwbPb/6o7obPB+mQE6ApPzMjj1sBfAM+VJL59o3WWK77J3bxV7/O+VJx8N/PvV0u2ItaeNPTDbFMcpLuB0rlTvSI4+N799+qsJIIRWJF0nf2m490PNd7zQXYO89S7Q+ZwrDm744vuxupgSO12DgkzU8ghqjR/uswIDAQAB";

    std::string data = "Hello, World!";
    std::string encryptedData = RSAEncrypt(base64PublicKey, data);
    std::cout << "Encrypted Data: " << encryptedData << std::endl;

    return 0;
}
