#include "Log.h"

#include <cstdio>
#include <cstdarg>
#include <cstring>

std::string E4::Log::filter;
unsigned E4::Log::filter_n = 0;

void E4::Log::setFilter(const char* f) {
    if (f) {
        filter = f;
        filter_n = filter.length();
    } else {
        filter = "";
        filter_n = 0;
    }
}

void E4::Log::debug(const char* format, ...) {
#ifdef DEBUG
    if (filter_n > 0) {
        if (strncmp(format, filter.c_str(), filter_n) != 0) {
            return;
        }
    }
    va_list arglist;
    va_start(arglist, format);
    vprintf(format, arglist);
    va_end(arglist);
    printf("\n");
    fflush(stdout);
#endif
}
void E4::Log::error(const char* format, ...) {
    va_list arglist;
    va_start(arglist, format);
    vprintf(format, arglist);
    va_end(arglist);
    printf("\n");
    fflush(stdout);
}
