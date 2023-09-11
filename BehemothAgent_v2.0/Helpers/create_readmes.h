#ifndef CREATE_READMES_H
#define CREATE_READMES_H

#include <iostream>


/// <summary>
/// This function takes a string of text and a file name.
/// It then write the given text to the given file and saves the output to
/// 4 special directories such as Desktop, Documents, Pictures and Downloads.
/// </summary>
/// <param name="text"></param>
/// <param name="filename"></param>
void create_readmes(const std::string& text, const std::string& filename);

#endif