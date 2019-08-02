#include "StringPool.h"

E4::StringPool::StringPool() {
    indexFor("");
}

uint32_t E4::StringPool::indexOf(const std::string& name) {
    auto iter = indexMap.find(name);
    if (iter == indexMap.end()) {
        return 0;
    } else {
        return iter->second;
    }
}

uint32_t E4::StringPool::indexFor(const std::string& name) {
    auto iter = indexMap.find(name);
    if (iter == indexMap.end()) {
        uint32_t index = array.size();
        indexMap[name] = index;
        array.push_back(name);
        return index;
    } else {
        return iter->second;
    }
}

const std::string& E4::StringPool::get(uint32_t index) {
    return array[index];
}
