#include "Log.h"

#include <stdio.h>
#include <stdarg.h>

void E4::Log::debug(const char* format, ...) {
#ifdef DEBUG
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
