#ifndef RSA_ENC_H
#define RSA_ENC_H

#include <string>


/// <summary>
/// This function performs RSA encryption. It takes a public key string and data.
/// It then performs RSA encryption with OAEP padding for secure data encryption.
/// </summary>
/// <param name="pubKey"></param>
/// <param name="data"></param>
/// <returns></returns>
std::string RSAEncrypt(const std::string& pubKey, const std::string& data);


#endif
