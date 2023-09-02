#include "file_enc.h"
#include "gen_str.h"
#include <cryptopp/aes.h>
#include <cryptopp/filters.h>
#include <cryptopp/gcm.h>
#include <cryptopp/files.h>
#include <iostream>
#include <fstream>


bool AESEncrypt(const std::string& inputFilePath, const std::string& outputFilePath, std::string& key, std::string& iv)
{
    using namespace CryptoPP;

    key = gen_str(32);
    iv = gen_str(16);

    try
    {
        GCM<AES>::Encryption encryptor;
        encryptor.SetKeyWithIV(reinterpret_cast<const byte*>(key.data()), key.size(), reinterpret_cast<const byte*>(iv.data()), iv.size());

        std::ifstream inputFile(inputFilePath, std::ios::binary);
        if (!inputFile)
        {
            std::cerr << "Error opening input file" << std::endl;
            return false;
        }

        std::ofstream outputFile(outputFilePath, std::ios::binary);
        if (!outputFile)
        {
            std::cerr << "Error opening output file" << std::endl;
            return false;
        }

        FileSource fs(inputFile, true,
            new AuthenticatedEncryptionFilter(encryptor,
                new FileSink(outputFile)
            )
        );

        inputFile.close();
        outputFile.close();
    }
    catch (const CryptoPP::Exception& e)
    {
        std::cerr << e.what() << std::endl;
        return false;
    }

    return true;
}