#ifndef FILE_ENCRYPT_H
#define FILE_ENCRYPT_H

#include <string>

bool AESEncrypt(const std::string& inputFilePath, const std::string& outputFilePath, std::string& key, std::string& iv);

#endif // FILE_ENCRYPT_H
