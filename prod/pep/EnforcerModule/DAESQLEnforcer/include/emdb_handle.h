#ifndef EMDB_HANDLE_H
#define EMDB_HANDLE_H

//1. UserContextInfo magicnumber = 43120
//2. CEMDBResult magicnumber = 43121
#define HANDLE_USERCONTEXTINFO 43120
#define HANDLE_CEMDBRESULT 43121

class CEMDBHandle
{
    public:
    CEMDBHandle(uint64_t magicNumber):m_magicNumber(magicNumber){

    }
    virtual ~CEMDBHandle(){
        m_magicNumber = 0;
    }
    virtual bool IsType(uint64_t value){
        return m_magicNumber==value;
    }

    protected:
    uint64_t m_magicNumber;

};













#endif