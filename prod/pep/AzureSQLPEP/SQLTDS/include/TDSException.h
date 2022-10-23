#pragma once


#ifndef __TDS_EXCEPTION_H__
#define __TDS_EXCEPTION_H__

#include <stdexcept>
#include <string>

class TDSException : public std::exception
{
public:
    TDSException(const std::string& err)
        : m_except(err.c_str())
    {}


    TDSException(const char* err)
        : m_except(err)
    {}

    virtual char const* what() const
    {
        return m_except.what();
    }
   
private:
    std::exception m_except;
};


void ThrowTdsException(const char* fmt, ...);

#endif