#include "gen_str.h"


std::string gen_str(int StrLength) {
    const std::string chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
    std::string res;
    res.reserve(StrLength);

    HCRYPTPROV hCryptProv;
    if (CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT)) {
        while (StrLength-- > 0) {
            unsigned int num;
            CryptGenRandom(hCryptProv, sizeof(num), (BYTE*)&num);
            res.push_back(chars[num % chars.size()]);
        }
        CryptReleaseContext(hCryptProv, 0);
    }
    return res;
}