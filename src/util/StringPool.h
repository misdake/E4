#pragma once

#include <vector>
#include <cstdint>
#include <map>
#include <string>

namespace E4 {

    class StringPool {
        //TODO each string is stored twice, optimize?
        std::vector<std::string> array;
        std::map<std::string, uint32_t> indexMap;
    public:
        StringPool();

        uint32_t indexOf(const std::string& name);
        uint32_t indexFor(const std::string& name);

        const std::string& get(uint32_t index);

    };

}
