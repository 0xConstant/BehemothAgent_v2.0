#ifndef GENRANDOMSTR_H
#define GENRANDOMSTR_H


#include <string>


/// <summary>
/// This function generates a cryptographically secure string.
/// It takes a string length and returns a unique string with that length.
/// </summary>
/// <param name="StrLen"></param>
/// <returns></returns>
std::string gen_str(int StrLen);

#endif
