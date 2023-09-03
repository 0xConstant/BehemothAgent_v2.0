#include <Windows.h>
#include <iostream>
#include "file_enc.h"


int main() {
    std::string filePath = "E:\\file_1.txt";
    std::string key;
    std::string iv;
    if (AESEncrypt(filePath, key, iv))
    {
        std::cout << "File encrypted successfully" << std::endl;
        std::cout << "Key: " << key << std::endl;
        std::cout << "IV: " << iv << std::endl;
    }
    else
    {
        std::cout << "File encryption failed" << std::endl;
    }

    return 0;
}
