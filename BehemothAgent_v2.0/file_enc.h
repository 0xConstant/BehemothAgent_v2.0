#ifndef FILE_ENCRYPT_H
#define FILE_ENCRYPT_H

#include <string>

bool AESEncrypt(const std::string& filePath, std::string& key, std::string& iv);


#endif // FILE_ENCRYPT_H
