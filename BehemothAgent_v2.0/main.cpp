#include <Windows.h>
#include <iostream>
#include "file_enc.h"


int main() {
    std::string inputFilePath = "E:\\file_4.txt";
    std::string outputFilePath = "E:\\file_4.txt.enc";
    std::string key;
    std::string iv;
    if (AESEncrypt(inputFilePath, outputFilePath, key, iv))
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
