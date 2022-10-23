#ifndef IUSER_ATTR_H
#define IUSER_ATTR_H
#include <string>
#include "IPolicyResource.h"


class IUserAttr
{
   public:
   virtual ~IUserAttr(){}
   virtual const std::string& GetUserNameAttr() const = 0;
   virtual std::string GetUserAttr(const char* szAttrName) = 0;
   virtual bool ValidUserAttr(const char* szAttrName, const char* szAttrValue, const char* szOperator) = 0;
   virtual void CopyDictionary(EMResDictionary  & dic) = 0;
    virtual void CopyDictionary2(EMResDictionary2  & dic) = 0;
   virtual uint64_t GetAttributeCount() = 0;
   virtual std::string Print() = 0;
   virtual void AddAttrWithType(const char* szName, const char* szValue, CEAttributeType attrType) = 0;
};

#endif