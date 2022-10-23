//
// Created by jeff on 2020/3/18.
//

#ifndef EMDB_MASK_HARDCODE_H
#define EMDB_MASK_HARDCODE_H

#include "node.h"
#include "EMColumnRef.h"
#include "TableRef.h"

using namespace resolve;


enum MaskOprType{
    MASK_2ASLABEL,
    MASK_DIRECT
};

bool mask_fields_v3(ISelectStmt * stmt,RawExpr *root, const MaskConditionMap *map_condition, EMDB_DB_TYPE dbtype, MaskOprType masktype, SqlException & e);

bool check_date_time_format(EMDB_DB_TYPE dbtype, EMDataType type, const std::string & symbols);



#endif //EMDB_MASK_HARDCODE_H
