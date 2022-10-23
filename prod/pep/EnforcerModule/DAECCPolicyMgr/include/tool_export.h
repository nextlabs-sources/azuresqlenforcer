#ifndef TOOL_EXPORT_H_20201119
#define TOOL_EXPORT_H_20201119

#ifdef _WIN32
    #ifdef LIBTOOL_IMP
        #define TOOL_EXPORT  __declspec(dllimport)
    #else
        #define TOOL_EXPORT  __declspec( dllexport)
    #endif 
#else
    #define TOOL_EXPORT  __attribute__((visibility("default")))
#endif 

#endif 