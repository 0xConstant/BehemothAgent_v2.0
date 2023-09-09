#include <cryptopp/sha3.h>
#include <cryptopp/filters.h>
#include <cryptopp/hex.h>
#include "gen_uid.h"



std::string GetUUID() {
    char uuid[100];
    FILE* fp = _popen("wmic csproduct get uuid", "r");
    if (fp == nullptr) {
        return "";
    }
    int i = 0;
    while (fgets(uuid, sizeof(uuid) - 1, fp) != nullptr) {
        i++;
        if (i == 2) break;
    }
    _pclose(fp);
    std::string uuidStr(uuid);
    uuidStr.erase(std::remove(uuidStr.begin(), uuidStr.end(), '\n'), uuidStr.end());
    return uuidStr;
}


std::string SHA3(const std::string& input)
{
    // Generate SHA3 256 bit hash
    CryptoPP::SHA3_256 hasher;  
    std::string digest;

    // Use StringSource to hash the string and store in digest.
    CryptoPP::StringSource(input, true,
        new CryptoPP::HashFilter(hasher,
            // return a hex encoded hash
            new CryptoPP::HexEncoder(
                new CryptoPP::StringSink(digest)
            )
        )
    );

    return digest;
}



std::string gen_uid() {
    std::string uuid = GetUUID();
    return SHA3(uuid);
}


