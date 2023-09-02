#include <Windows.h>
#include <iostream>



int main() {
    std::string filePath = "E:\\file_1.txt";
    std::string key = "your_key_here"; // 32 bytes
    std::string iv = "your_iv_here";    // 16 bytes
    auto fileKeys = FileEncrypt(filePath, key, iv);

    return 0;
}
