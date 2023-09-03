#include <Windows.h>
#include <iostream>
#include "file_enc.h"


int main() {
    std::string filePath = "E:\\file_1.txt";
    auto result = AESEncrypt(filePath);

    if (!result.empty())
    {
        auto newFilePath = result.begin()->first;

        std::cout << "File encrypted successfully" << std::endl;
        std::cout << "New File Path: " << newFilePath << std::endl;
        std::cout << "Key: " << result[newFilePath]["key"] << std::endl;
        std::cout << "IV: " << result[newFilePath]["iv"] << std::endl;
    }
    else
    {
        std::cout << "File encryption failed" << std::endl;
    }

    return 0;
}
