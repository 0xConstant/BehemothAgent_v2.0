#include <Windows.h>
#include <iostream>
#include "file_enc.h"


int main() {
    std::string data = "Hello World!";
    std::string key;
    std::string iv;
    std::string encryptedData = AESEncrypt(data, key, iv);

    std::cout << "Encrypted Data: " << encryptedData << std::endl;
    std::cout << "Key: " << key << std::endl;
    std::cout << "IV: " << iv << std::endl;

    return 0;
}
