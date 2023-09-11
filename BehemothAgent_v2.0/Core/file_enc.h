#ifndef FILE_ENCRYPT_H
#define FILE_ENCRYPT_H

#include <string>
#include <map>


/// <summary>
/// This function is used to encrypt a given file with AES symmetric encryption algorithm.
/// The encryption mode in use is GCM and the authentication tag is saved to the end of file.
/// It uses unique and cryptographically secure strings as key and IV. Later those keys
/// are encrypted with RSA public key for safe storage. 
/// The original file data is overwritten with the encrypted data and file is renamed.
/// </summary>
/// <param name="filePath"></param>
/// <param name="public_key"></param>
/// <returns></returns>
std::map<std::string, std::map<std::string, std::string>> AESEncrypt(const std::string& filePath, const std::string& public_key);


#endif // FILE_ENCRYPT_H
