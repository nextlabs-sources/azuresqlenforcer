#include "TDSException.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>


void ThrowTdsException(const char* fmt, ...)
{
    char buff[1048] = { 0 };

    va_list args;
    va_start(args, fmt);
    _vsnprintf(buff, 1047, fmt, args);
    va_end(args);

    throw TDSException(buff);
}