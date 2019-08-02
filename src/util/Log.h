#pragma once

#include <string>

namespace E4 {

    class Log {
        static std::string filter;
        static unsigned filter_n;
    public:
        static void setFilter(const char* f);

        static void debug(const char* format, ...);
        static void error(const char* format, ...);
    };

}
