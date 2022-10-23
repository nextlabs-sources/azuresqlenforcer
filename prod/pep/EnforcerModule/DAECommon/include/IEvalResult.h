#ifndef IEVAL_RESULT_H
#define IEVAL_RESULT_H

#include "IPolicy.h"

class IEvalResult
{
    public:
    virtual ~IEvalResult() {}
    virtual bool IsDeny()=0;
    virtual bool IsAllow()=0;
    virtual bool IsDontCare()=0;
    virtual size_t  PolicyCount()=0;
    virtual IPolicy* GetPolicyByIndex(size_t nIndex)=0;

};
#endif 