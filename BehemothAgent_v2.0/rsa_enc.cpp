#include <cryptopp/rsa.h>
#include <cryptopp/base64.h>
#include <cryptopp/osrng.h>
#include <cryptopp/files.h>


std::string RSAEncrypt(const std::string& base64PubKey, const std::string& data)
{
    try {
        // Decode public key from base64
        CryptoPP::ByteQueue bytes;
        CryptoPP::StringSource ss(base64PubKey, true, new CryptoPP::Base64Decoder);
        ss.TransferTo(bytes);
        bytes.MessageEnd();
        CryptoPP::RSA::PublicKey publicKey;

        try {
            publicKey.Load(bytes);
        }
        catch (const CryptoPP::Exception& e) {
            std::cerr << "Error loading public key: " << e.what() << std::endl;
            return "";
        }

        std::string encryptedData;

        // Encrypt the data
        CryptoPP::RSAES_OAEP_SHA_Encryptor encryptor(publicKey);
        CryptoPP::AutoSeededRandomPool rng;
        try {
            CryptoPP::StringSource(data, true, new CryptoPP::PK_EncryptorFilter(rng, encryptor, new CryptoPP::StringSink(encryptedData)));
        }
        catch (const CryptoPP::Exception& e) {
            std::cerr << "Error encrypting data: " << e.what() << std::endl;
            return "";
        }

        // Base64 encode the encrypted data
        std::string base64EncryptedData;
        try {
            CryptoPP::StringSource(encryptedData, true, new CryptoPP::Base64Encoder(new CryptoPP::StringSink(base64EncryptedData), false));
        }
        catch (const CryptoPP::Exception& e) {
            std::cerr << "Error encoding encrypted data: " << e.what() << std::endl;
            return "";
        }

        return base64EncryptedData;
    }
    catch (const std::exception& e) {
        std::cerr << "An unexpected error occurred: " << e.what() << std::endl;
        return "";
    }
}

