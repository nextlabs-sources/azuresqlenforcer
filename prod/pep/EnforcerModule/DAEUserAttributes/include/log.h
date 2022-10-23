#ifndef USERATTRIBUTE_LOG_H
#define USERATTRIBUTE_LOG_H
#include "userattr_export.h"



enum emLogLevel
{
    log_trace = 0,
    log_debug ,
    log_info,
    log_warning,
    log_error,
    log_fatal
};

class Log
{

    public:
     //log callback
     static WriteLog_Fun m_logFun;

     public:
     static int  WriteLog(int lvl, const char* fmt, ...);

};

#endif 
