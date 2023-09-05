#ifndef FILE_ENCRYPT_H
#define FILE_ENCRYPT_H

#include <string>
#include <map>

std::map<std::string, std::map<std::string, std::string>> AESEncrypt(const std::string& filePath);


#endif // FILE_ENCRYPT_H
