#include "gen_uid.h"
#include <iostream>
#include <windows.h>
#include <bcrypt.h>
#include <string>

#pragma comment(lib, "bcrypt.lib")

#define NT_SUCCESS(Status)          (((NTSTATUS)(Status)) >= 0)
#define STATUS_UNSUCCESSFUL         ((NTSTATUS)0xC0000001L)


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

std::string GetSHA256Hash(const std::string& input)
{
    BCRYPT_ALG_HANDLE       hAlg = NULL;
    BCRYPT_HASH_HANDLE      hHash = NULL;
    NTSTATUS                status = STATUS_UNSUCCESSFUL;
    DWORD                   cbData = 0,
        cbHash = 0,
        cbHashObject = 0;
    PBYTE                   pbHashObject = NULL;
    PBYTE                   pbHash = NULL;
    std::string             result;

    //open an algorithm handle
    if (!NT_SUCCESS(status = BCryptOpenAlgorithmProvider(
        &hAlg,
        BCRYPT_SHA256_ALGORITHM,
        NULL,
        0)))
    {
        goto Cleanup;
    }

    //calculate the size of the buffer to hold the hash object
    if (!NT_SUCCESS(status = BCryptGetProperty(
        hAlg,
        BCRYPT_OBJECT_LENGTH,
        (PBYTE)&cbHashObject,
        sizeof(DWORD),
        &cbData,
        0)))
    {
        goto Cleanup;
    }

    //allocate the hash object on the heap
    pbHashObject = (PBYTE)HeapAlloc(GetProcessHeap(), 0, cbHashObject);
    if (NULL == pbHashObject)
    {
        goto Cleanup;
    }

    //calculate the length of the hash
    if (!NT_SUCCESS(status = BCryptGetProperty(
        hAlg,
        BCRYPT_HASH_LENGTH,
        (PBYTE)&cbHash,
        sizeof(DWORD),
        &cbData,
        0)))
    {
        goto Cleanup;
    }

    //allocate the hash buffer on the heap
    pbHash = (PBYTE)HeapAlloc(GetProcessHeap(), 0, cbHash);
    if (NULL == pbHash)
    {
        goto Cleanup;
    }

    //create a hash
    if (!NT_SUCCESS(status = BCryptCreateHash(
        hAlg,
        &hHash,
        pbHashObject,
        cbHashObject,
        NULL,
        0,
        0)))
    {
        goto Cleanup;
    }

    //hash some data
    if (!NT_SUCCESS(status = BCryptHashData(
        hHash,
        (PBYTE)input.data(),
        (ULONG)input.size(),
        0)))
    {
        goto Cleanup;
    }

    //close the hash
    if (!NT_SUCCESS(status = BCryptFinishHash(
        hHash,
        pbHash,
        cbHash,
        0)))
    {
        goto Cleanup;
    }

    for (DWORD i = 0; i < cbHash; i++)
    {
        char buf[3];
        snprintf(buf, sizeof(buf), "%02x", pbHash[i]);
        result.append(buf);
    }

Cleanup:

    if (hAlg)
    {
        BCryptCloseAlgorithmProvider(hAlg, 0);
    }

    if (hHash)
    {
        BCryptDestroyHash(hHash);
    }

    if (pbHashObject)
    {
        HeapFree(GetProcessHeap(), 0, pbHashObject);
    }

    if (pbHash)
    {
        HeapFree(GetProcessHeap(), 0, pbHash);
    }

    return result;
}



std::string GenUID() {
    std::string uuid = GetUUID();
    return GetSHA256Hash(uuid);
}


