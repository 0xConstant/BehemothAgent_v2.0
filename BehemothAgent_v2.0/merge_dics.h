#ifndef MERGE_DICS_H
#define MERGE_DICS_H

#include <unordered_map>
#include <vector>
#include <string>

template <typename TValue>
std::unordered_map<std::string, TValue> merge_dics(const std::vector<std::unordered_map<std::string, TValue>>& dictionaries) {
    std::unordered_map<std::string, TValue> mergedDictionary;
    for (const auto& dictionary : dictionaries) {
        for (const auto& kvp : dictionary) {
            mergedDictionary[kvp.first] = kvp.second;
        }
    }
    return mergedDictionary;
}

#endif // MERGE_DICS_H
