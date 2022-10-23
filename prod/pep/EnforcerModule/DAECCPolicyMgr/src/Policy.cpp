#include "tool.h"
#include "Handle.h"
#include "policy_expression.h"
#include "json/json.h"
#include "policy_regex.h"
#include "parse_expression.h"
#include "Lex.h"
#include "Policy.h"
#include <assert.h>
#include <vector>
#include "PolicyModelList.h"
//#define PE_FOR_TEST
#pragma warning(push)
#pragma warning(disable: 5045)

//std::string serialize(AstExpr * pexpr);
bool is_digit(char c) { return '0' <= c && c <= '9'; }
bool is_underline(char c) { return '_' == c; }
bool is_letter(char c) { return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z'); }
//bool is_percent(char c) { return '%' == c; }
//bool is_end(const std::string& s, unsigned int iIndex) { return s.length() <= iIndex; }
//bool is_double_quote(char c) { return '"' == c; }
bool is_wirte(char c) {
    if(c == ' ' || c == '\n' || c == '\t' || c == '\f' || c == '\r') return true;
    return false;
}



AstColumnRef::COL_TYPE  transform_type(PolicyModel::PM_TYPE pm_type) {
    switch (pm_type) {
        case PolicyModel::PM_RES:
            return AstColumnRef::COL_TYPE::RES;
        case PolicyModel::PM_SUB_USER:
            return AstColumnRef::COL_TYPE::SUB;
        case PolicyModel::PM_SUB_APP:
            return AstColumnRef::COL_TYPE::APP;
        case PolicyModel::PM_SUB_HOST:
            return AstColumnRef::COL_TYPE::HOST;
        default:
            return AstColumnRef::COL_TYPE::OTHER;
    }
}
AstColumnRef::VAL_TYPE  transform_type(AttributeInfo::ATTR_TYPE attr_type) {
    switch (attr_type) {
        case AttributeInfo::A_NUMBER:
            return AstColumnRef::VAL_TYPE::CC_NUMBER;
        case AttributeInfo::A_STRING:
            return AstColumnRef::VAL_TYPE::CC_STRING;
        case AttributeInfo::A_MULTI:
            return AstColumnRef::VAL_TYPE::CC_MULTI;
        default:
            return AstColumnRef::VAL_TYPE::CC_OTHER;
    }
}

#define COLUMN_REF_ACTION "ACTION"

const std::map<std::string,const AstExpr::EXPR_TYPE, CommonFun::IgnoreCaseCmp> OPERATOR_TABLE{
        {"=",   AstExpr::EXPR_TYPE::COMP_EQ},
        {"!=",  AstExpr::EXPR_TYPE::COMP_NEQ},
        {">",   AstExpr::EXPR_TYPE::COMP_GT},
        {">=",  AstExpr::EXPR_TYPE::COMP_GE},
        {"<",   AstExpr::EXPR_TYPE::COMP_LT},
        {"<=",  AstExpr::EXPR_TYPE::COMP_LE},
        {"INCLUDES",           AstExpr::EXPR_TYPE::INCLUDES},
        {"EQUALS_UNORDERED",   AstExpr::EXPR_TYPE::EQUALS_UNORDERED}
};
AstExpr::EXPR_TYPE get_asttype_from_oprtable(const std::string & sopr) {
    auto it = OPERATOR_TABLE.find(sopr);
    if (it == OPERATOR_TABLE.end()) return AstExpr::EXPR_TYPE::EXPER_NOT_SUPPORT;
    return it->second;
}

AstExpr * parse_from_condition(const Json::Value & json, PolicyModelList * ppmlst, const uint64_t pm_id);
AstExpr * parse_from_conditions(const Json::Value & conditions, PolicyModelList * ppmlst, const uint64_t pm_id);
AstExpr * parse_from_components(const Json::Value & components, PolicyModelList * ppmlst, bool is_not = false);
AstExpr * parse_from_com_components(const Json::Value & com_components, PolicyModelList * ppmlst);

AstExpr * parse_from_actions(const Json::Value & actions );
AstExpr * parse_from_components_for_action(const Json::Value & components,  bool is_not = false);
AstExpr * parse_from_action_components(const Json::Value & action_components);

AstExpr * parse_from_expression(const Json::Value & action_components, PolicyModelList * ppmlst);

void analyze_reference( AstExpr * pexpr, std::set<std::string> & actions, std::set<std::string> & attributes, std::set<std::string> & resourceattrs, std::set<std::string> & rhost, std::set<std::string> & rapp); /* actions and attibutes recursive query function */

bool parse_column_ref( std::string & pm_name, std::string & attr_name, const std::string & s, const AstColumnRef::COL_TYPE  type); /* parser r-value to column-ref */

Policy::~Policy() {
    if(_expr) delete (_expr);
    _expr = nullptr;
}

bool parse_column_ref( std::string & pm_name, std::string & attr_name, const std::string & s, const AstColumnRef::COL_TYPE  type) {
    if(s.length() < 4) return false;
    if(s[0] != '$') return false;
    if(s[1] != '{') return false;
    // ${abc.def.efg...}
    size_t len = s.length();
    if(s[len-1] != '}') return false;
    int i = 2;
    int start = i;
    //filter first dot ,  if type = resource
    if(type == AstColumnRef::COL_TYPE::RES) {
        while(i <= len-1) {
            if(s[i] == '.'  ) {
                i++;
                break;
            }
            i++;
        }
    }
    //second dot is resource/subject model name
    start = i;
    while(i <= len-1) {
        if (s[i] == '.' ) {
            pm_name = s.substr(start, ((size_t)i - (size_t)start));
            i++;
            break;
        }
        i++;
    }
    //final is attribute name
    start = i;
    while (i <= len-1) {
        if (s[i] == '}' && i == len-1) {
            attr_name = s.substr(start, ((size_t)i - (size_t)start));
        }
        i++;
    }
    return true;
}

AstExpr * parse_from_condition(const Json::Value & json, PolicyModelList * ppmlst, const uint64_t pm_id){

    std::string column_ref = json["attribute"].asString();
    if(column_ref.empty()) // if attr is null
        return new AstConstantValue(AstExpr::EXPR_TYPE::C_TRUE);
    AstId * pastid = new AstId(column_ref); 
    AstExpr * pexpr_left = NULL;
    {
        AstIds ids;
        ids.push_back(pastid);
        if (ppmlst != NULL) {
            AstColumnRef::COL_TYPE col_type = transform_type(ppmlst->GetPMTypeByID(pm_id));
            AstColumnRef::VAL_TYPE val_type = transform_type(ppmlst->GetAttrTypeByPmidAttrName(pm_id, column_ref));
            pexpr_left = new AstColumnRef(col_type, val_type, ids);
        } else {
            pexpr_left = new AstColumnRef(AstColumnRef::COL_TYPE::OTHER, AstColumnRef::VAL_TYPE::CC_OTHER, ids);
        }
    }  

    AstExpr * pexpr_right = NULL;
    std::string constant_value = json["value"].asString();
    std::string rhs_type = json["rhsType"].asString();
    if (rhs_type.compare("CONSTANT") == 0) {
        if ( is_policy_regex(constant_value) ) {
            AstConstantValue * pexpr_temp = new AstConstantValue(AstExpr::EXPR_TYPE::C_PATTERN);
            pexpr_temp->SetValue(constant_value);
            pexpr_right = pexpr_temp;
        } else {
            AstConstantValue * pexpr_temp = new AstConstantValue(AstExpr::EXPR_TYPE::C_STRING);
            pexpr_temp->SetValue(constant_value);
            pexpr_right = pexpr_temp;
        }

    } else if (rhs_type.compare("SUBJECT") == 0){
        AstIds ids;
        std::string pm_name, attr_name;
        parse_column_ref(pm_name, attr_name, constant_value, AstColumnRef::COL_TYPE::SUB);
        ids.push_back(new AstId(pm_name));
        ids.push_back(new AstId(attr_name));
        if (ppmlst != NULL) {
            AstColumnRef::COL_TYPE col_type = transform_type(ppmlst->GetPMTypeByPmname(pm_name));
            AstColumnRef::VAL_TYPE val_type = transform_type(ppmlst->GetAttrTypeByPmnameAttrName(pm_name, attr_name));
            pexpr_right = new AstColumnRef(col_type, val_type, ids);
        } else {
            pexpr_right = new AstColumnRef(AstColumnRef::COL_TYPE::OTHER, AstColumnRef::VAL_TYPE::CC_OTHER, ids);
        }

    } else if (rhs_type.compare("RESOURCE") == 0) {
        AstIds ids;
        std::string pm_name, attr_name;
        parse_column_ref(pm_name, attr_name, constant_value,AstColumnRef::COL_TYPE::RES);
        ids.push_back(new AstId(pm_name));
        ids.push_back(new AstId(attr_name));
        if (ppmlst != NULL) {
            AstColumnRef::COL_TYPE col_type = transform_type(ppmlst->GetPMTypeByPmname(pm_name));
            AstColumnRef::VAL_TYPE val_type = transform_type(ppmlst->GetAttrTypeByPmnameAttrName(pm_name, attr_name));
            pexpr_right = new AstColumnRef(col_type, val_type, ids);
        } else {
            pexpr_right = new AstColumnRef(AstColumnRef::COL_TYPE::OTHER, AstColumnRef::VAL_TYPE::CC_OTHER, ids);
        }

    } else {
        return new AstConstantValue(AstExpr::EXPR_TYPE::C_UNKNOWN);
    }

    std::string op_cond = json["operator"].asString();
    AstExpr::EXPR_TYPE ast_type = get_asttype_from_oprtable(op_cond);

    if (ast_type == AstExpr::EXPR_TYPE::EXPER_NOT_SUPPORT) {
        return new AstExpr(AstExpr::EXPR_TYPE::EXPER_NOT_SUPPORT);
    }

    if (pexpr_right->GetExprType() == AstExpr::EXPR_TYPE::C_PATTERN) {
        if (ast_type == AstExpr::EXPR_TYPE::COMP_EQ) {
            ast_type = AstExpr::EXPR_TYPE::LIKE;
        } else if (ast_type == AstExpr::EXPR_TYPE::COMP_NEQ) {
            ast_type = AstExpr::EXPR_TYPE::NOT_LIKE;
        }
    }
    return new AstBinaryOpExpr(ast_type, pexpr_left, pexpr_right);

}

AstExpr * parse_from_conditions(const Json::Value & conditions, PolicyModelList * ppmlst, const uint64_t pm_id) {
    const Json::Value & js_cond_first = conditions[0];
    ///BinaryExpr
    AstExpr * pexp = parse_from_condition(js_cond_first, ppmlst, pm_id);

    int i_cond_size = conditions.size();
    for (int i = 1; i < i_cond_size; ++i) {//AND
        const Json::Value & js_cond = conditions[i];
        ///BinaryExpr
        AstExpr * pexpr_condition = parse_from_condition(js_cond, ppmlst, pm_id);
        ///
        AstExpr * pexpr_temp = pexp;
        pexp = new AstBinaryOpExpr(AstExpr::EXPR_TYPE::AND, pexpr_temp, pexpr_condition);
    }

    return  pexp;
}

AstExpr * parse_from_components(const Json::Value & components, PolicyModelList * ppmlst, bool is_not) {
    const Json::Value & js_comp_first = components[0];
    uint64_t pm_id = js_comp_first["policy_model_id"].asInt();
    const Json::Value & js_conditons_first = js_comp_first["conditions"];
    AstExpr * pexp =  parse_from_conditions(js_conditons_first, ppmlst, pm_id);

    int i_comp_size = components.size();
    for (int i = 1; i < i_comp_size; ++i) {

        const Json::Value & js_comp = components[i];
        pm_id = js_comp["policy_model_id"].asInt();
        const Json::Value & js_conditons = js_comp["conditions"];
        AstExpr * pexp_sub = parse_from_conditions(js_conditons, ppmlst, pm_id);
        ///
        AstExpr * pexpr_temp = pexp;
        pexp = new AstBinaryOpExpr(AstExpr::EXPR_TYPE::OR, pexpr_temp, pexp_sub);

    }
    // NOT
    if (is_not && pexp) {
        AstExpr * pexpr_temp = pexp;
        pexp = new AstUnaryOpExpr(AstExpr::EXPR_TYPE::NOT, pexpr_temp);
    }

    return  pexp;
}

AstExpr * parse_from_com_components(const Json::Value & com_components, PolicyModelList * ppmlst) {
    if (com_components.size() == 0) {
        return  new AstConstantValue(AstExpr::EXPR_TYPE::C_TRUE);
    }
    const Json::Value & js_opcomp_first = com_components[0];
    std::string op_first = js_opcomp_first["operator"].asString();
    bool is_not_first = op_first.compare("IN") != 0;
    const Json::Value & js_components_first = js_opcomp_first["components"];
    AstExpr * pexp = parse_from_components(js_components_first, ppmlst, is_not_first);

    for (unsigned int i = 1; i < com_components.size(); ++i) {
        const Json::Value & js_opcomp = com_components[i];
        std::string op = js_opcomp["operator"].asString();
        bool is_not = op.compare("IN") != 0;
        const Json::Value & js_components = js_opcomp["components"];
        AstExpr * pexp_sub = parse_from_components(js_components, ppmlst, is_not);
        AstExpr * pexp_temp = pexp;
        pexp = new AstBinaryOpExpr(AstExpr::EXPR_TYPE::AND, pexp_temp, pexp_sub);
    }

    return pexp;
}

AstExpr *  parse_from_actions(const Json::Value & actions ){
    AstExpr * pexp = NULL;
    {
        std::string str_action = actions[0].asString();
        AstConstantValue * pexp_right = new AstConstantValue(AstExpr::EXPR_TYPE::C_STRING);
        pexp_right->SetValue(str_action);

        AstId * pid = new AstId(COLUMN_REF_ACTION);
        AstIds ids ;
        ids.push_back(pid);
        AstExpr * pexp_left = new AstColumnRef(AstColumnRef::COL_TYPE::ACTION, AstColumnRef::VAL_TYPE::CC_STRING,  ids);

        pexp = new AstBinaryOpExpr(AstExpr::EXPR_TYPE::COMP_EQ, pexp_left, pexp_right);
    }

    for (unsigned int i = 1; i < actions.size(); ++i) {
        std::string str_action = actions[i].asString();
        AstConstantValue * pexp_right = new AstConstantValue(AstExpr::EXPR_TYPE::C_STRING);
        pexp_right->SetValue(str_action);

        AstId * pid = new AstId(COLUMN_REF_ACTION);
        AstIds ids ;
        ids.push_back(pid);
        AstExpr * pexp_left = new AstColumnRef(AstColumnRef::COL_TYPE::ACTION, AstColumnRef::VAL_TYPE::CC_STRING,  ids);

        AstExpr * pexp_sub = new AstBinaryOpExpr(AstExpr::EXPR_TYPE::COMP_EQ, pexp_left, pexp_right);

        AstExpr * pexp_temp = pexp;
        pexp = new AstBinaryOpExpr(AstExpr::EXPR_TYPE::OR, pexp_temp, pexp_sub);
    }

    return  pexp;
}

AstExpr * parse_from_components_for_action(const Json::Value & components,  bool is_not) {
    AstExpr * pexp = NULL;
    {
        const Json::Value & js_comp = components[0];
        std::string comp_id = js_comp["id"].asString();
        std::string comp_name = js_comp["name"].asString();
        const Json::Value & js_actions = js_comp["actions"];
        pexp =  parse_from_actions(js_actions);
    }

    for (unsigned int i = 1; i < components.size(); ++i) {
        const Json::Value & js_comp = components[i];
        std::string comp_id = js_comp["id"].asString();
        std::string comp_name = js_comp["name"].asString();
        const Json::Value & js_actions = js_comp["actions"];
        AstExpr * pexp_sub =  parse_from_actions(js_actions);
        ///
        AstExpr * pexp_temp = pexp;
        pexp = new AstBinaryOpExpr(AstExpr::EXPR_TYPE::OR, pexp_temp, pexp_sub);
    }
    // NOT
    if (is_not && pexp) {
        AstExpr * pexpr_temp = pexp;
        pexp = new AstUnaryOpExpr(AstExpr::EXPR_TYPE::NOT, pexpr_temp);
    }

    return  pexp;
}

AstExpr * parse_from_action_components(const Json::Value & action_components){
    if (action_components.size() == 0) {
        return  new AstConstantValue(AstExpr::EXPR_TYPE::C_TRUE);
    }
    AstExpr * pexp = NULL;
    {
        const Json::Value & js_opcomp = action_components[0];
        std::string op = js_opcomp["operator"].asString();
        bool is_not = op.compare("IN") != 0;
        const Json::Value & js_components = js_opcomp["components"];
        pexp = parse_from_components_for_action(js_components,  is_not);
    }

    for ( unsigned int i = 1; i < action_components.size(); ++i) {
        const Json::Value & js_opcomp = action_components[i];
        std::string op = js_opcomp["operator"].asString();
        bool is_not = op.compare("IN") != 0;
        const Json::Value & js_components = js_opcomp["components"];
        AstExpr * pexp_sub = parse_from_components_for_action(js_components,  is_not);
        AstExpr * pexp_temp = pexp;
        pexp = new AstBinaryOpExpr(AstExpr::EXPR_TYPE::OR, pexp_temp, pexp_sub);
    }

    return  pexp;
}

AstExpr * parse_from_expression(const Json::Value & action_components, PolicyModelList * ppmlst) {
    std::string str_expression = action_components.asString();
    if (str_expression.compare("null") == 0 ||str_expression.empty()) {
        return new AstConstantValue(AstExpr::EXPR_TYPE::C_TRUE);
    }
    Lex lex(str_expression);
    lex.Next();
    ParseException e;
    e._syms = ppmlst;
    AstExpr *expr = parse_boolean_expr(&lex, &e);

    if (e._code != ParseException::RET::SUCCESS || lex.GetCurrent()->GetType() != Token::TK_END_P) {
        return  new AstConstantValue(AstExpr::EXPR_TYPE::C_UNKNOWN);
    }
    return  expr;
}



void analyze_reference( AstExpr * pexpr, std::set<std::string> & actions, std::set<std::string> & attributes, std::set<std::string> & resourceattrs, std::set<std::string> & rhost, std::set<std::string> & rapp) {
    switch(pexpr->GetExprType()) {
        case AstExpr::EXPR_TYPE::AND:
        case AstExpr::EXPR_TYPE::OR:
        case AstExpr::EXPR_TYPE::LIKE:
        case AstExpr::EXPR_TYPE::NOT_LIKE:
        case AstExpr::EXPR_TYPE::COMP_EQ:
        case AstExpr::EXPR_TYPE::COMP_NEQ:
        case AstExpr::EXPR_TYPE::COMP_GT:
        case AstExpr::EXPR_TYPE::COMP_GE:
        case AstExpr::EXPR_TYPE::COMP_LT:
        case AstExpr::EXPR_TYPE::COMP_LE:
        case AstExpr::EXPR_TYPE::INCLUDES:
        case AstExpr::EXPR_TYPE::EQUALS_UNORDERED:{
            analyze_reference(dynamic_cast< AstBinaryOpExpr*>(pexpr)->GetLeft(), actions, attributes, resourceattrs, rhost, rapp);
            analyze_reference(dynamic_cast< AstBinaryOpExpr*>(pexpr)->GetRight(), actions, attributes, resourceattrs, rhost, rapp);
        } break;
        case AstExpr::EXPR_TYPE::NOT: {
            analyze_reference(dynamic_cast< AstUnaryOpExpr*>(pexpr)->GetExpr(), actions, attributes, resourceattrs, rhost, rapp);
        } break;
        case AstExpr::EXPR_TYPE::EXPR_COLUMN_REF: {
            AstColumnRef * pexpr_col = dynamic_cast<AstColumnRef*>(pexpr);
            if(AstColumnRef::COL_TYPE::ACTION == pexpr_col->GetColType()) {

                AstExpr * pparent = pexpr_col->GetParent();
                AstExpr * pbrother = dynamic_cast<AstBinaryOpExpr*>(pparent)->GetRight();
                assert(pbrother->GetExprType() == AstExpr::EXPR_TYPE::C_STRING);
                actions.insert(dynamic_cast<AstConstantValue*>(pbrother)->GetValueAsStr());
            } else if (AstColumnRef::COL_TYPE::SUB == pexpr_col->GetColType()) {
                attributes.insert(pexpr_col->GetColumn().back()->GetId());
            } else  if (AstColumnRef::COL_TYPE::RES == pexpr_col->GetColType()) {
                resourceattrs.insert(pexpr_col->GetColumn().back()->GetId());
            } else  if (AstColumnRef::COL_TYPE::HOST == pexpr_col->GetColType()) {
                rhost.insert(pexpr_col->GetColumn().back()->GetId());
            } else  if (AstColumnRef::COL_TYPE::APP == pexpr_col->GetColType()) {
                rapp.insert(pexpr_col->GetColumn().back()->GetId());
            }
        } break;
        default: break;
    }
}

std::string parse_identifier(const std::string & str, unsigned int & pos) {
    std::string ret;
    if(str[pos] >= '0' && str[pos] <= '9') {
        //error;
    } else if(str[pos] == '"') {//"aaa"
        pos++;
        if(is_digit(str[pos]) ) {
            //error;
        } else {
            while(str[pos] != '\0') {
                if(str[pos] == '"' ) {
                    pos++;
                    if(str[pos] != '"'){
                        break;
                    }
                }
                ret += str[pos];
                pos++;
            }
        }
    } else if(is_underline(str[pos]) || is_letter(str[pos])  ){
        ret += str[pos];
        pos++;
        while(str[pos] != '\0') {
            if(is_underline(str[pos]) || is_letter(str[pos]) || is_digit(str[pos]) ) {
                ret += str[pos];
            } else if(str[pos] == '.') {
                break;
            } else {
                break;
            }
            pos++;
        }
    } else if(str[pos]  == '*') {
        ret += '*';/*support format: * */
    } else {
    }

    while(str[pos] != ',' && str[pos] != '.' && str[pos] != '\0') {
        pos++;
    }

    return  ret;
}

void parse_fields(const std::string & ss, std::set<std::string> & set_fields) {
    unsigned int ipos = 0;
    while(ipos < ss.length()) {
        if(ss[ipos] == ' '|| ss[ipos] == ',' ){
        } else {
            std::string scol = parse_identifier(ss, ipos);
            if(!scol.empty()) {
                set_fields.insert(scol);
            }
        }
        ipos++;
    }
}


PolicyEngineReturn Policy::ParseFromJson(const Json::Value & root , PolicyModelList * ppmlst) {
#ifdef TEST_JEFF
        if (root.isMember("name")) printf("-----POLICY : %s-----\n", root["name"].asCString());
        Json::StreamWriterBuilder builder;
        const std::string json_file = Json::writeString(builder, root);
        //printf("%s\n",json_file.c_str());
#endif
        std::string sallow = root["effectType"].asString();
        if(sallow.compare("deny") == 0) _allow = false;
        //action
        const Json::Value & actions_components = root["actionComponents"];
        AstExpr*act_expr = parse_from_action_components(actions_components);
        //subject
//        const Json::Value & subject_components = root["subjectComponents"];
//        AstExpr*sub_expr = parse_from_com_components(subject_components, ppmlst);
        //resource
        const Json::Value & resource_components = root["fromResourceComponents"];
        AstExpr*res_expr = parse_from_com_components(resource_components, ppmlst);
        //advance
        const Json::Value & expression = root["expression"];
        AstExpr * pexp_expression = parse_from_expression(expression, ppmlst);

        _obligation = root["allowObligations"];
        _deny_obligation = root["denyObligations"];

        //AstExpr * pexpr_resource_result = res_expr; /// resource tree  can't be calculated
        //AstExpr * pexp_sa = new AstBinaryOpExpr(AstExpr::AND, act_expr, sub_expr );
        //AstExpr * pexp_sar = new AstBinaryOpExpr(AstExpr::AND, pexp_sa, res_expr );

        AstExpr * pexp_sar = new AstBinaryOpExpr(AstExpr::EXPR_TYPE::AND, act_expr, res_expr );
        _expr = new AstBinaryOpExpr(AstExpr::EXPR_TYPE::AND, pexp_sar, pexp_expression);

        //_pres_expr = _p_resource;
   
#ifdef TEST_JEFF
    Json::StreamWriterBuilder builder;
    const std::string json_file = Json::writeString(builder, root);
    //printf("----POLICY----\n%s\n",json_file.c_str());
#endif
    return POLICY_ENGINE_SUCCESS;
}

//bool parser_obligation_about_mask(MaskConditionMap & mask_map, const Json::Value & json_obgs){
//    //parse condition
//    if(!json_obgs.isArray())
//        return false;
//
//    const std::string strAllTables= "*";
//    const std::string strExcludeTableKey = "- ";
//    for (unsigned int i = 0; i < json_obgs.size(); ++i) {
//        const Json::Value & json_obg = json_obgs[i];
//        //std::string strname = json_obg["name"].asString();
//        std::string name = json_obg["name"].asString();
//        const Json::Value & json_params = json_obg["params"];
//        if (name.compare("data_masking") == 0) {
//            std::string tables, fields, format, symbols, cond;
//            std::set<std::string> set_fields;
//            std::set<std::string> set_tables;
//            std::set<std::string> set_excludeTables; //exclude table
//            if(!json_params["mask_tables"].isNull()){
//                tables = json_params["mask_tables"].asString();
//
//                //if tables begin with "- ", means these table must be exclude.
//                #if SUPPORT_EXCLUDE_MASKTABLE
//                if (tables.find(strExcludeTableKey)==0){
//                    //special the exclude table
//                    set_tables.insert(strAllTables);
//                    std::string strRealTables = tables.substr(strExcludeTableKey.length());
//                    parse_fields(strRealTables,set_excludeTables);
//                }
//                else
//                #endif
//                {
//                    //speical the include table
//                     parse_fields(tables,set_tables);
//                }
//            }
//
//            if(!json_params["mask_fields"].isNull()){
//                fields = json_params["mask_fields"].asString();
//                parse_fields(fields,set_fields);
//            }
//
//            if(!json_params["mask_format"].isNull()){
//                format = json_params["mask_format"].asString();
//            }
//
//            if(!json_params["mask_symbol"].isNull()){
//                symbols = json_params["mask_symbol"].asString();
//            }
//
//            if(!json_params["mask_cond1"].isNull()){
//                std::string cond1 = json_params["mask_cond1"].asString();
//                if(!cond1.empty()){
//                    cond = cond1;
//                }
//            }
//
//            if(!json_params["mask_cond2"].isNull()){
//                std::string cond2 = json_params["mask_cond2"].asString();
//                if(!cond2.empty()){
//                    if(cond.empty()){
//                        cond = cond2;
//                    } else {
//                        cond += " OR ";
//                        cond += cond2;
//                    }
//                }
//            }
//
//            if(!json_params["mask_cond3"].isNull()){
//                std::string cond3 = json_params["mask_cond3"].asString();
//                if(!cond3.empty()){
//                    if(cond.empty()){
//                        cond = cond3;
//                    } else {
//                        cond += " OR ";
//                        cond += cond3;
//                    }
//                }
//            }
//
//            if(tables.empty() || fields.empty() || symbols.empty()){
//                continue;
//            }
//
//            for(auto tb:set_tables){
//                auto it = mask_map.find(tb);
//                if(it == mask_map.end()){
//                    MaskItemMap *sub = new MaskItemMap();
//                    for(auto field:set_fields){
//                        MaskItem * item = new MaskItem();
//                        //item->_cond_fields.insert(cond_fields.begin(), cond_fields.end());
//                        item->_condition = cond;
//                        item->_format = MaskItem::GetMaskFormat(format);
//                        item->_symbols = symbols;
//                        item->_field = field;
//
//                        #if SUPPORT_EXCLUDE_MASKTABLE
//                        item->_excludeTable = set_excludeTables;
//                        #endif
//
//                        sub->insert(std::make_pair(field, item));
//                    }
//                    mask_map.insert(std::make_pair(tb, sub));
//                } else {
//                    MaskItemMap *sub = it->second;
//                    for(auto field:set_fields){
//                        auto itsub = sub->find(field);
//                        if(itsub == sub->end()){
//                            MaskItem * item = new MaskItem();
//                            //item->_cond_fields.insert(cond_fields.begin(), cond_fields.end());
//                            item->_condition = cond;
//                            item->_format = MaskItem::GetMaskFormat(format);
//                            item->_symbols = symbols;
//                            item->_field = field;
//
//                            #if SUPPORT_EXCLUDE_MASKTABLE
//                            item->_excludeTable = set_excludeTables;
//                            #endif
//
//                            sub->insert(std::make_pair(field, item));
//                        } else {
//                            //ignore
//                        }
//                    }
//                }
//            }
//        }
//    }
//    return true;
//}

bool check_obligation(bool & bfilter, bool & bmask,bool & bdup, const Json::Value & json_obgs){
    if(!json_obgs.isArray())
        return false;
    for (unsigned int i = 0; i < json_obgs.size(); ++i) {
        const Json::Value & json_obg = json_obgs[i];
        std::string name = json_obg["name"].asString();
        if (name.compare("predicate_condition") == 0){
            bfilter = true;
        } else if(name.compare("data_masking") == 0) {
            bmask = true;
        } else if(name.compare("update_control") == 0){
            bdup = true;
        }
        if(bfilter && bmask && bdup) break;
    }
    return true;
}
bool get_obligation_tables(std::set<std::string> & tbs, const Json::Value & json_obgs){
    if(!json_obgs.isArray())
        return false;
    for (unsigned int i = 0; i < json_obgs.size(); ++i) {
        const Json::Value & json_obg = json_obgs[i];
        std::string name = json_obg["name"].asString();
        if(name.compare("data_masking") == 0) {
            const Json::Value & json_params = json_obg["params"];
            if(!json_params["mask_tables"].isNull()){
                std::string tables = json_params["mask_tables"].asString();
                parse_fields(tables,tbs);
            }
        } 
    }
    return true;
}

//bool parser_obligation_about_deny_up(DenyUpdateItemMap & dupi_map, const Json::Value & json_obgs){
//    //parse condition
//    if(!json_obgs.isArray())
//        return false;
//
//    const std::string strAllTables= "*";
//    const std::string strExcludeTableKey = "- ";
//    for (unsigned int i = 0; i < json_obgs.size(); ++i) {
//        const Json::Value & json_obg = json_obgs[i];
//        //std::string strname = json_obg["name"].asString();
//        std::string name = json_obg["name"].asString();
//        const Json::Value & json_params = json_obg["params"];
//        if (name.compare("update_control") == 0) {
//            //std::string tables, fields, cond;
//            std::set<std::string> set_fields;
//            //std::set<std::string> set_tables;
//            //std::set<std::string> set_excludeTables; //exclude table
//            std::string fields,cond;
////            if(!json_params["deny_update_tables"].isNull()){
////                tables = json_params["deny_update_tables"].asString();
////
////                //if tables begin with "- ", means these table must be exclude.
////                if (tables.find(strExcludeTableKey)==0){
////                    //special the exclude table
////                    set_tables.insert(strAllTables);
////                    std::string strRealTables = tables.substr(strExcludeTableKey.length());
////                    parse_fields(strRealTables,set_excludeTables);
////                }
////                else{
////                    //speical the include table
////                    parse_fields(tables,set_tables);
////                }
////            }
//
//            if(!json_params["fields"].isNull()){
//                fields = json_params["fields"].asString();
//                parse_fields(fields,set_fields);
//            }
//
//            if(!json_params["condition1"].isNull()){
//                std::string cond1 = json_params["condition1"].asString();
//                if(!cond1.empty()){
//                    cond = cond1;
//                }
//            }
//
//            if(!json_params["condition2"].isNull()){
//                std::string cond2 = json_params["condition2"].asString();
//                if(!cond2.empty()){
//                    if(cond.empty()){
//                        cond = cond2;
//                    } else {
//                        cond += " OR ";
//                        cond += cond2;
//                    }
//                }
//            }
//
//            if(!json_params["conditon3"].isNull()){
//                std::string cond3 = json_params["condition3"].asString();
//                if(!cond3.empty()){
//                    if(cond.empty()){
//                        cond = cond3;
//                    } else {
//                        cond += " OR ";
//                        cond += cond3;
//                    }
//                }
//            }
//
//            if( fields.empty() ){
//                continue;
//            }
//            for(auto field:set_fields){
//                auto itsub = dupi_map.find(field);
//                if(itsub == dupi_map.end()){
//                    UpdateItem * item = new UpdateItem();
//                    //item->_cond_fields.insert(cond_fields.begin(), cond_fields.end());
//                    item->_condition = cond;
//                    item->_field = field;
//                    //item->_excludeTable = set_excludeTables;
//                    dupi_map.insert(std::make_pair(field, item));
//                } else {
//                    //ignore
//                }
//            }
//
////            for(auto tb:set_tables){
////                auto it = dup_map.find(tb);
////                if(it == dup_map.end()){
////                    DenyUpdateItemMap *sub = new DenyUpdateItemMap();
////                    for(auto field:set_fields){
////                        UpdateItem * item = new UpdateItem();
////                        //item->_cond_fields.insert(cond_fields.begin(), cond_fields.end());
////                        item->_condition = cond;
////                        item->_field = field;
////                        item->_excludeTable = set_excludeTables;
////
////                        sub->insert(std::make_pair(field, item));
////                    }
////                    dup_map.insert(std::make_pair(tb, sub));
////                } else {
////                    DenyUpdateItemMap *sub = it->second;
////                    for(auto field:set_fields){
////                        auto itsub = sub->find(field);
////                        if(itsub == sub->end()){
////                            UpdateItem * item = new UpdateItem();
////                            //item->_cond_fields.insert(cond_fields.begin(), cond_fields.end());
////                            item->_condition = cond;
////                            item->_field = field;
////                            item->_excludeTable = set_excludeTables;
////                            sub->insert(std::make_pair(field, item));
////                        } else {
////                            //ignore
////                        }
////                    }
////                }
////            }
//        }
//    }
//    return true;
//}

//bool Policy::GetMaskInfos(MaskConditionMap & mask_map, S4HException &/*exc*/){
//    if(_obligation.isNull()){
//        return false;
//    }
//    parser_obligation_about_mask( mask_map, _obligation);
//    return true;
//}
//
//bool Policy::GetUpdateControl(DenyUpdateItemMap & dupi_map, S4HException &/*exc*/){
//    if(_deny_obligation.isNull()){
//        return false;
//    }
//    parser_obligation_about_deny_up( dupi_map, _deny_obligation);
//    return true;
//}
//
//bool Policy::GetResTables(std::set<std::string> &tables, S4HException &/*exc*/){
//    get_attribute_value_vec(tables,  _expr, AstColumnRef::RES, "table" );
//    return true;
//}

bool Policy::GetPolicyBasicInfo(TablePolicyInfoMap & tbs_map, S4HException &/*exc*/){
    std::set<std::string> tbs;
    get_attribute_value_vec(tbs,  _expr, AstColumnRef::COL_TYPE::RES, "table" );
    //std::set<std::string> dbs;
    //get_attribute_value_vec(dbs, _expr, AstColumnRef::COL_TYPE::RES, "database");
    //std::set<std::string> scs;
    //get_attribute_value_vec(scs, _expr, AstColumnRef::COL_TYPE::RES, "schema");
    bool bfiler_valid = tbs.size()==0?false:true;
    if(tbs.size()==0){
        get_obligation_tables(tbs, _obligation);//get mask tbs
    }
    if(tbs.size()==0){
        return false;
    }
    //actions
    std::set<std::string> actions;
    get_attribute_value_vec(actions,  _expr, AstColumnRef::COL_TYPE::ACTION, "ACTION" );
    //bool values
    bool bfilter = false;
    bool bmask = false;
    bool bdup = false;
    check_obligation(bfilter, bmask, bdup, _obligation);  // todo resource tables not in masktables?
    check_obligation(bfilter, bmask, bdup, _deny_obligation); 
    if(!bfiler_valid){//only mask then filter invalid
      bfilter = false;  
    }
    if (!_allow) { //deny
        bfilter = true;
    }
    auto it = tbs.begin();
    for(;it != tbs.end();++it){
        auto itm = tbs_map.find(*it);
        if(itm != tbs_map.end()){
            TablePolicyInfo & info = itm->second;
            for(auto itact :actions){
                info._actions = info._actions | TablePolicyInfo::GetEnumActionFromString(itact);
            }
            if(bfilter) info._bfilter = true;
            if(bmask) info._bmask = true;
            if(bdup) info._bdup = true;
        } else {
            TablePolicyInfo info;
            //for (auto & it:dbs) {
            //    info._dbname = it;//todo 
            //    break;
            //}
            //for (auto & it : scs) {
            //    info._schema = it;//todo 
            //    break;
            //}
            info._actions = ACT_None;
            for(auto itact :actions){
                info._actions = info._actions | TablePolicyInfo::GetEnumActionFromString(itact);
            }
            if(bfilter) info._bfilter = true;
            if(bmask) info._bmask = true;
            if(bdup) info._bdup = true;
            tbs_map[*it] = info;
        }
    }

    return true;
}


#pragma warning(pop)