#ifndef STRPTIME_H
#define STRPTIME_H

#ifdef WIN32

#define gmtime_r(a,b) gmtime_s(b,a)
#define localtime_r(a,b) localtime_s(b,a)

#endif 

#include <time.h>

char * strptime(const char *buf, const char *fmt, struct tm *tm);




#endif // !STRPTIME_H