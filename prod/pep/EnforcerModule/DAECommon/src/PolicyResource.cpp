#include "PolicyResource.h"
#include "DAELog.h"
#include "commfun.h"

CPolicyResource::CPolicyResource()
{
}
CPolicyResource::~CPolicyResource() {

}

void CPolicyResource::AddedAttribute(const char *szName, const char *szValue, CEAttributeType attrType)
{
    //if (NULL == FindAttribute(szName))
    std::string name(szName);
    if(name.empty()){
        return;
    }
    //const ResourceAttrValue *attrValue = FindAttribute(szName);
    //if (NULL != attrValue)
    //{
    //    delete (attrValue);
    //}
    {
        
        m_dicAttrValue[szName] = std::make_shared<ResourceAttrValue>(szValue, attrType);
    }
}

void CPolicyResource::CopyDictionary(EMResDictionary & dic){
    for(auto it = m_dicAttrValue.begin(); it != m_dicAttrValue.end(); ++it){
        if(dic.find(it->first) == dic.end()){
            ResourceAttrValue * attrValue = new ResourceAttrValue(it->second->strValue,it->second->attrType);
            dic[it->first] = attrValue;
        }
    }
}

void CPolicyResource::CopyDictionary2(EMResDictionary2 & dic){
    for(auto it = m_dicAttrValue.begin(); it != m_dicAttrValue.end(); ++it){
        if(dic.find(it->first) == dic.end()){
            std::shared_ptr<ResourceAttrValue> ptrb = std::make_shared<ResourceAttrValue>(it->second->strValue,it->second->attrType);
            //ResourceAttrValue * attrValue = new ResourceAttrValue(it->second->strValue,it->second->attrType);
            dic[it->first] = ptrb;
        }
    }
}

bool CPolicyResource::ValidAttribute(const char *szName, const char *szAttrValue, const char *szOperator,
                                     CEAttributeType attrType)
{
    bool bValid = false;
    const ResourceAttrValue *attrValue = FindAttribute(szName);
    if (NULL != attrValue)
    {
        const std::string& value = attrValue->strValue;

        if (CommonFun::StrCaseCmp(szOperator, CONDITION_EQUAL) == 0 || CommonFun::StrCaseCmp(szOperator, CONDITION_NOT_EQUAL) == 0)
        { //compare value as string
            bool bEqual = CommonFun::StrCaseCmp(szAttrValue, value.c_str()) == 0;
            bValid = (bEqual && CommonFun::StrCaseCmp(szOperator, CONDITION_EQUAL) == 0) ||
                     (!bEqual && CommonFun::StrCaseCmp(szOperator, CONDITION_NOT_EQUAL) == 0);
        }
        else
        { //compare value as number
            double attrvalue = atof(value.c_str());
            double inValue = atof(szAttrValue);

            if (CommonFun::StrCaseCmp(szOperator, CONDITION_SMALL) == 0)
            {
                bValid = attrvalue < inValue;
            }
            else if (CommonFun::StrCaseCmp(szOperator, CONDITION_SMALL_EQUAL) == 0)
            {
                bValid = attrvalue <= inValue;
            }
            else if (CommonFun::StrCaseCmp(szOperator, CONDITION_LARGE) == 0)
            {
                bValid = attrvalue > inValue;
            }
            else if (CommonFun::StrCaseCmp(szOperator, CONDITION_LARGE_EQUAL) == 0)
            {
                bValid = attrvalue >= inValue;
            }
            else
            {
                theLog->WriteLog(log_error, "UserAttr::ValidUserAttr, unsupport operator:%s", szOperator);
                bValid = false;
            }
        }
    }
    return bValid;
}

const ResourceAttrValue *CPolicyResource::FindAttribute(const char *szName)
{
    auto itAttr = m_dicAttrValue.find(szName);
    if (itAttr != m_dicAttrValue.end())
    {
        return itAttr->second.get();
    }
    return NULL;
}