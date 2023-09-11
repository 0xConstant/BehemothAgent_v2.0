#ifndef GOOGLE_CONN_H
#define GOOGLE_CONN_H


/// <summary>
/// This function is used to verify active internet connection by connecting to Google.
/// The function sends an ICMP ping request with a timeout of 10 seconds. Addotionally,
/// it will retry sending a ping request 3 times, each with a timeout of 10 seconds
/// to accurately verify internet connection.
/// </summary>
/// <returns></returns>
bool googleConn();

#endif
