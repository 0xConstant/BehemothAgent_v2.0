#include <cryptopp/sha3.h>
#include <cryptopp/filters.h>
#include <cryptopp/hex.h>
#include <Windows.h>
#include "gen_uid.h"



std::string GetUUID() {
	char uuid[100] = { 0 }; // Initializing with zeros
	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(sa);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;

	HANDLE hRead, hWrite;
	if (!CreatePipe(&hRead, &hWrite, &sa, 0)) {
		return "";
	}

	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	si.hStdError = hWrite;
	si.hStdOutput = hWrite;
	si.hStdInput = hRead;
	si.dwFlags |= STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_HIDE;

	ZeroMemory(&pi, sizeof(pi));

	WCHAR cmd[] = L"wmic csproduct get uuid";
	if (!CreateProcess(NULL, cmd, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi)) {
		CloseHandle(hWrite);
		CloseHandle(hRead);
		return "";
	}

	WaitForSingleObject(pi.hProcess, INFINITE);
	DWORD bytesRead;
	if (!ReadFile(hRead, uuid, sizeof(uuid) - 1, &bytesRead, NULL) || bytesRead == 0) {
		CloseHandle(hWrite);
		CloseHandle(hRead);
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
		return "";
	}

	CloseHandle(hWrite);
	CloseHandle(hRead);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	std::string uuidStr(uuid);

	// Remove "UUID" prefix and any leading whitespace
	std::size_t pos = uuidStr.find("UUID");
	if (pos != std::string::npos) {
		uuidStr = uuidStr.substr(pos + 4); // Removes "UUID"
	}

	// Trim leading and trailing spaces, and null characters.
	uuidStr.erase(uuidStr.begin(), std::find_if(uuidStr.begin(), uuidStr.end(), [](unsigned char ch) {
		return !std::isspace(ch) && ch != '\0';
		}));
	uuidStr.erase(std::find_if(uuidStr.rbegin(), uuidStr.rend(), [](unsigned char ch) {
		return !std::isspace(ch) && ch != '\0';
		}).base(), uuidStr.end());

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


