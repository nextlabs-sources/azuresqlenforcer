#ifndef POLICY_H
#define POLICY_H

#include <string>
#include <set>
#include <json/json.h>

class AstExpr;
class PolicyModelList;
class PolicyModel;
class Policy {
public:
    Policy() {_expr = NULL;}
    ~Policy();
    PolicyEngineReturn ParseFromJson(const Json::Value & root , PolicyModelList * ppmlst);
    
//    bool GetMaskInfos(MaskConditionMap & mask_map, S4HException &exc);
//    bool GetUpdateControl(DenyUpdateItemMap & dupi_map, S4HException &exc);
//    bool GetResTables(std::set<std::string> &tables, S4HException &exc);
    bool GetPolicyBasicInfo(TablePolicyInfoMap & tbs_map, S4HException &exc);

private:
    Json::Value _obligation;
    Json::Value _deny_obligation;
    AstExpr        *    _expr;
    bool _allow = true;

};

#endif