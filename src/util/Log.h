#pragma once

namespace E4 {

    class Log {
    public:
        static void debug(const char* format, ...);
        static void error(const char* format, ...);
    };

}
