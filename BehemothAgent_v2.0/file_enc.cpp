#include <iostream>
#include <fstream>
#include <cryptopp/aes.h>
#include <cryptopp/gcm.h>
#include <cryptopp/filters.h>
#include <cryptopp/osrng.h>
#include <filesystem>

#include "gen_str.h"
#include "rsa_enc.h"
#include "file_enc.h"


std::string AESEncrypt(const std::string& data, std::string& key, std::string& iv) {
    using namespace CryptoPP;

    key = gen_str(32);
    iv = gen_str(16);

    std::string cipherText;

    try {
        GCM<AES>::Encryption encryptor;
        encryptor.SetKeyWithIV(reinterpret_cast<const byte*>(key.data()), key.size(), reinterpret_cast<const byte*>(iv.data()), iv.size());

        // PKCS#7 padding
        std::string paddedData = data;
        int paddingSize = AES::BLOCKSIZE - (data.size() % AES::BLOCKSIZE);
        paddedData.append(paddingSize, char(paddingSize));

        AuthenticatedEncryptionFilter ef(encryptor,
            new StringSink(cipherText));
        ef.Put(reinterpret_cast<const byte*>(paddedData.data()), paddedData.size());
        ef.MessageEnd();
    }
    catch (const CryptoPP::Exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return cipherText;
}