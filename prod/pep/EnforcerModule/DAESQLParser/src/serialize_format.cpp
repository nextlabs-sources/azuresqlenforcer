#include "serialize_format.h"

/*----SQLSERVER-----*/
extern Form SP_EXECUTESQL_SERIALIZE_FORMAT_C {{1,"SP_EXECUTESQL "},{0,"",0,""}};
extern FmCB SP_EXECUTESQL_SERIALIZE_FORMAT {&SP_EXECUTESQL_SERIALIZE_FORMAT_C, &SP_EXECUTESQL_SERIALIZE_FORMAT_C};

extern Form PIVOT_CLAUSE_SERIALIZE_FORMAT_C {{0,"",0,"("},{0,"",1,""},{1," FOR "},{0,"",2,""},{1," IN "}, {0,"",3," "},{1, ")"},{0,"",4," "}};
extern FmCB PIVOT_CLAUSE_SERIALIZE_FORMAT  {&PIVOT_CLAUSE_SERIALIZE_FORMAT_C, &PIVOT_CLAUSE_SERIALIZE_FORMAT_C};

extern Form FUNC_TRIM_SERIALIZE_FORMAT_C            {{0,"",0,""},{1,"("},{0,"",2,""},{1," FROM "},{0,"",1,""},{1,")"}};
extern FmCB FUNC_TRIM_SERIALIZE_FORMAT              {&FUNC_TRIM_SERIALIZE_FORMAT_C, &FUNC_TRIM_SERIALIZE_FORMAT_C};

/*----BD2-----*/

extern Form DB2_TABLE_SERIALIZE_FORMAT_C  {{1, "TABLE("},{0,"",0," "},{0,"",1,""},{1,")"},{0,"",2," "}, {0,"",3," "}};
extern FmCB DB2_TABLE_SERIALIZE_FORMAT  {&DB2_TABLE_SERIALIZE_FORMAT_C, &DB2_TABLE_SERIALIZE_FORMAT_C};

/*----ORACLE---*/
extern Form FUN_EXTRACT_SERIALIZE_FORMAT_ORA_C  {{0,"",0,"("},{0,"",1," FROM "},{0,"",2,""},{1, ")"}};
extern FmCB FUN_EXTRACT_SERIALIZE_FORMAT_ORA {&FUN_EXTRACT_SERIALIZE_FORMAT_ORA_C,&FUN_EXTRACT_SERIALIZE_FORMAT_ORA_C};

extern Form STMT_BODYS_SERIALIZE_FORMAT_C {{0,"BEGIN\n ",0," END "}};
extern FmCB STMT_BODYS_SERIALIZE_FORMAT_ORA {&STMT_BODYS_SERIALIZE_FORMAT_C, &STMT_BODYS_SERIALIZE_FORMAT_C};

extern Form RETURNING_INTO_SERIALIZE_FORMAT_C {{0," RETURNING ",0,""},{0," INTO ",1," "}};
extern FmCB RETURNING_INTO_SERIALIZE_FORMAT {&RETURNING_INTO_SERIALIZE_FORMAT_C, &RETURNING_INTO_SERIALIZE_FORMAT_C};

extern Form FETCH_FIRST_ROW_ONLY_C {{0," FETCH FIRST ",0," ROW ONLY "}};
extern FmCB FETCH_FIRST_ROW_SERIALIZE_FORMAT  {&FETCH_FIRST_ROW_ONLY_C, &FETCH_FIRST_ROW_ONLY_C};
extern Form FETCH_FIRST_ROWS_ONLY_C {{0," FETCH FIRST ",0," ROWS ONLY "}};
extern FmCB FETCH_FIRST_ROWS_SERIALIZE_FORMAT  {&FETCH_FIRST_ROWS_ONLY_C, &FETCH_FIRST_ROWS_ONLY_C};

extern Form COMMON_TABLE_EXPR_SERIALIZE_FORMAT_C_ORA     {{0,"",0,""}, {0,"",1,""}, {1," AS "}, {0,"",2,""}, {0,"",3,""}, {0,"",4,""}};
//extern Form COMMON_TABLE_EXPR_SERIALIZE_FORMAT_B     {{0,"",0,""}, {0,"",1,""}, {1," AS "}, {0,"",2,""}};
extern FmCB COMMON_TABLE_EXPR_SERIALIZE_FORMAT_ORA       {&COMMON_TABLE_EXPR_SERIALIZE_FORMAT_C_ORA, &COMMON_TABLE_EXPR_SERIALIZE_FORMAT_C_ORA};

extern Form SEARCH_CLAUSE_SERIALIZE_FORMAT_C_ORA     {{0,"",0," "}, {0,"",1," "}, {0,"",2," "}, {0,"",3," "}, {0," SET ",4,""}};
extern FmCB  SEARCH_CLAUSE_SERIALIZE_FORMAT_ORA     {&SEARCH_CLAUSE_SERIALIZE_FORMAT_C_ORA, &SEARCH_CLAUSE_SERIALIZE_FORMAT_C_ORA};

extern Form  CYCLE_CLAUSE_SERIALIZE_FORMAT_C_ORA     {{0," CYCLE ",0," "}, {0,"SET ",1," "}, {0,"TO ",2," "}, {0,"DEFAULT ",3," "}};
extern FmCB  CYCLE_CLAUSE_SERIALIZE_FORMAT_ORA      {&CYCLE_CLAUSE_SERIALIZE_FORMAT_C_ORA, &CYCLE_CLAUSE_SERIALIZE_FORMAT_C_ORA};


extern Form SAMPLE_CLAUSE_SERIALIZE_FORMAT_C                 { {0,"",0,""},  {0,"(",1,")"},  {0,"",2,""}};
extern Form SAMPLE_CLAUSE_SERIALIZE_FORMAT_B                 { {0,"",0,""}, {0,"(",1,")"},  {0,"",2,""}};
extern FmCB SAMPLE_CLAUSE_SERIALIZE_FORMAT             {&SAMPLE_CLAUSE_SERIALIZE_FORMAT_C, &SAMPLE_CLAUSE_SERIALIZE_FORMAT_B};
extern Form EORA_SEED_VALUE_SERIALIZE_FORMAT_C                 {   {0,"SEED(",0,")"}};
extern FmCB EORA_SEED_VALUE_SERIALIZE_FORMAT        {&EORA_SEED_VALUE_SERIALIZE_FORMAT_C, &EORA_SEED_VALUE_SERIALIZE_FORMAT_C};
extern Form OFFSET_SERIALIZE_FORMAT_C                 {{1,"OFFSET "}, {0,"",0," "},  {0,"",1,""}};
extern FmCB OFFSET_SERIALIZE_FORMAT        {&OFFSET_SERIALIZE_FORMAT_C, &OFFSET_SERIALIZE_FORMAT_C};

extern Form SELECT_SERIALIZE_FORMAT_ORA_C                {{0,"",16," "}, {1,"SELECT "}, {0,"",13," "}, {0,"",0," "}, {0,"",15," "}, \
    {0,"",1," "}, {0,"",18," "}, {0,"",2," "}, {0,"",3," "}, {0,"",19," "}, {0,"",4," "}, {0,"",5," "}, {0,"",8," "}, {0,"",6," "},\
    {0,"",7," "}, {0,"",9," "}, {0,"",10," "}, {0,"",11," "}, {0,"",12," "}, {0,"",14," "}, {0,"",17,""}};
extern Form SELECT_SERIALIZE_FORMAT_ORA_B                {{0,"",16,"\n"}, {1,"SELECT "}, {0,"",13," "}, {0,"",0," "}, {0,"",15," "}, \
    {0,"",1," "}, {0,"",18," "}, {0,"\n",2," "}, {0,"\n",3," "}, {0,"",19," "}, {0,"\n",4," "}, {0,"\n",5," "}, {0,"",8," "}, {0,"",6," "},\
    {0,"",7," "}, {0,"",9," "}, {0,"\n",10," "}, {0,"",11," "}, {0,"\n",12," "}, {0,"",14," "}, {0,"",17,""}};
extern FmCB SELECT_SERIALIZE_FORMAT_ORA                  {&SELECT_SERIALIZE_FORMAT_ORA_C, &SELECT_SERIALIZE_FORMAT_ORA_B};

extern Form SELECT_SERIALIZE_FORMAT_ORA_C2                {{0,"",16," "}, {1,"SELECT UNIQUE "},
                                                    {0,"",13," "},
                                                    {0,"",0," "},
                                                    {0,"",15," "},
                                                    {0,"",1," "},
                                                    {0,"",18," "},
                                                    {0,"",2," "},
                                                    {0,"",3," "},
                                                    {0,"",19," "},
                                                    {0,"",4," "},
                                                    {0,"",5," "},
                                                    {0,"",8," "},
                                                    {0,"",6," "},
                                                    {0,"",7," "},
                                                    {0,"",9," "},
                                                    {0,"",10," "},
                                                    {0,"",11," "},
                                                    {0,"",12," "},
                                                    {0,"",14," "},
                                                    {0,"",17,""}};
extern Form SELECT_SERIALIZE_FORMAT_ORA_B2                {{0,"",16,"\n"}, {1,"SELECT UNIQUE "}, {0,"",13," "}, {0,"",0," "}, {0,"",15," "}, \
    {0,"",1," "}, {0,"",18," "}, {0,"\n",2," "}, {0,"\n",3," "}, {0,"",19," "}, {0,"\n",4," "}, {0,"\n",5," "}, {0,"",8," "}, {0,"",6," "},\
    {0,"",7," "}, {0,"",9," "}, {0,"\n",10," "}, {0,"",11," "}, {0,"\n",12," "}, {0,"",14," "}, {0,"",17,""}};
extern FmCB SELECT_SERIALIZE_FORMAT2_ORA                  {&SELECT_SERIALIZE_FORMAT_ORA_C2, &SELECT_SERIALIZE_FORMAT_ORA_B2};
extern Form PARTITION_BY_SERIALIZE_FORMAT_C                {{0,"PARTITION BY(",0,")"}};
extern FmCB PARTITION_BY_SERIALIZE_FORMAT                  {&PARTITION_BY_SERIALIZE_FORMAT_C, &PARTITION_BY_SERIALIZE_FORMAT_C};

extern Form ALTER_SESSION_SERIALIZE_FORMAT_C               {{0,"ALTER SESSION SET ",0," = "},{0,"",1,""}};
extern FmCB ALTER_SESSION_SERIALIZE_FORMAT_ORA             {&ALTER_SESSION_SERIALIZE_FORMAT_C, &ALTER_SESSION_SERIALIZE_FORMAT_C};
/*----BIGQUERY----*/
extern Form EXPR_COMMA_SERIALIZE_FORMAT_C              {{0,"",0," ,"}};
extern FmCB EXPR_COMMA_SERIALIZE_FORMAT                {&EXPR_COMMA_SERIALIZE_FORMAT_C,&EXPR_COMMA_SERIALIZE_FORMAT_C};

extern Form UNNEST_DEF_SERIALIZE_FORMAT_C              {{0,"UNNEST(",0,")"}};
extern FmCB UNNEST_DEF_SERIALIZE_FORMAT                {&UNNEST_DEF_SERIALIZE_FORMAT_C,&UNNEST_DEF_SERIALIZE_FORMAT_C};

extern Form STRUCT_TYPE_SERIALIZE_FORMAT_C             {{1,"STRUCT"},{0,"",0,""}};
extern FmCB STRUCT_TYPE_SERIALIZE_FORMAT               {&STRUCT_TYPE_SERIALIZE_FORMAT_C,&STRUCT_TYPE_SERIALIZE_FORMAT_C};
extern Form ARRAY_TYPE_SERIALIZE_FORMAT_C              {{1,"ARRAY"},{0,"",0,""}};
extern FmCB ARRAY_TYPE_SERIALIZE_FORMAT                {&ARRAY_TYPE_SERIALIZE_FORMAT_C,&ARRAY_TYPE_SERIALIZE_FORMAT_C};

extern Form MEMBER_DEF_SERIALIZE_FORMAT_C       {{0,"<",0,"> "}};
extern FmCB MEMBER_DEF_SERIALIZE_FORMAT         {&MEMBER_DEF_SERIALIZE_FORMAT_C, &MEMBER_DEF_SERIALIZE_FORMAT_C};

extern Form ASTERISK_QUALIFY_SERIALIZE_FORMAT_C            {{0,"",0,"."}, {0,"",1,"."}, {0,"",2,"."},{0,"",3," "}};
extern FmCB ASTERISK_QUALIFY_SERIALIZE_FORMAT              {&ASTERISK_QUALIFY_SERIALIZE_FORMAT_C, &ASTERISK_QUALIFY_SERIALIZE_FORMAT_C};

extern Form EXCEPT_EXPR_SERIALIZE_FORMAT_C            {{0,"EXCEPT",0," "}};
extern Form EXCEPT_EXPR_SERIALIZE_FORMAT_B            {{0,"EXCEPT",0," "},{1,"\n"}};
extern FmCB EXCEPT_EXPR_SERIALIZE_FORMAT              {&EXCEPT_EXPR_SERIALIZE_FORMAT_C, &EXCEPT_EXPR_SERIALIZE_FORMAT_B};

extern Form REPLACE_EXPR_SERIALIZE_FORMAT_C            {{0,"REPLACE(",0,")"}};
extern FmCB REPLACE_EXPR_SERIALIZE_FORMAT              {&REPLACE_EXPR_SERIALIZE_FORMAT_C, &REPLACE_EXPR_SERIALIZE_FORMAT_C};

extern Form ROW_EXPR_AS_SERIALIZE_FORMAT_C            {{0,"",0," "}, {0,"AS ",1," "}};
extern FmCB ROW_EXPR_AS_SERIALIZE_FORMAT              {&ROW_EXPR_AS_SERIALIZE_FORMAT_C, &ROW_EXPR_AS_SERIALIZE_FORMAT_C};



extern Form OFFSET_NUM_FORMAT_C                        {{0,"OFFSET ",0," "}};
extern FmCB OFFSET_NUM_FORMAT                          {&OFFSET_NUM_FORMAT_C, &OFFSET_NUM_FORMAT_C};
extern Form LIMIT_NUM_FORMAT_C                         {{0,"LIMIT ",0," "}, {0,"",1," "}};
extern FmCB LIMIT_NUM_FORMAT                           {&LIMIT_NUM_FORMAT_C, &LIMIT_NUM_FORMAT_C};
/*----COMMON----*/
extern Form SEMICOLON_LIST_SERIALIZE_FORMAT_C        {{0,"",0,""}, {1,"; "}, {0,"",1,""}};
extern Form SEMICOLON_LIST_SERIALIZE_FORMAT_B        {{0,"",0,""}, {1,";\n"}, {0,"\n",1,""}};
extern FmCB SEMICOLON_LIST_SERIALIZE_FORMAT          {&SEMICOLON_LIST_SERIALIZE_FORMAT_C, &SEMICOLON_LIST_SERIALIZE_FORMAT_B};

extern Form SPACE_LIST_SERIALIZE_FORMAT_C            {{0,"",0,""}, {0," ",1,""}};
extern Form SPACE_LIST_SERIALIZE_FORMAT_B            {{0,"",0,""}, {0,"\n",1,""}};
extern FmCB SPACE_LIST_SERIALIZE_FORMAT              {&SPACE_LIST_SERIALIZE_FORMAT_C, &SPACE_LIST_SERIALIZE_FORMAT_B};

extern Form COMMA_LIST_SERIALIZE_FORMAT_C            {{0,"",0,""}, {1,", "}, {0,"",1,""}};
extern Form COMMA_LIST_SERIALIZE_FORMAT_B            {{0,"",0,""}, {1,", "}, {0,"\n",1,""}};
extern FmCB COMMA_LIST_SERIALIZE_FORMAT              {&COMMA_LIST_SERIALIZE_FORMAT_C, &COMMA_LIST_SERIALIZE_FORMAT_B};

extern Form SELECT_SERIALIZE_FORMAT_C                {{0,"",16," "}, {1,"SELECT "}, {0,"",13," "}, {0,"",15," "}, {0,"",0," "}, {0,"",19," "}, \
    {0,"",1," "}, {0,"",18," "}, {0,"",2," "}, {0,"",3," "}, {0,"",4," "}, {0,"",5," "}, {0,"",8," "}, {0,"",6," "},\
    {0,"",7," "}, {0,"",9," "}, {0,"",10," "}, {0,"",11," "}, {0,"",12," "}, {0,"",14," "}, {0,"",17,""}};
extern Form SELECT_SERIALIZE_FORMAT_B                {{0,"",16,"\n"}, {1,"SELECT "}, {0,"",13," "}, {0,"",15," "}, {0,"",0," "}, {0,"",19," "}, \
    {0,"",1," "}, {0,"",18," "}, {0,"\n",2," "}, {0,"\n",3," "}, {0,"\n",4," "}, {0,"\n",5," "}, {0,"",8," "}, {0,"",6," "},\
    {0,"",7," "}, {0,"",9," "}, {0,"\n",10," "}, {0,"",11," "}, {0,"\n",12," "}, {0,"",14," "}, {0,"",17,""}};
extern FmCB SELECT_SERIALIZE_FORMAT                  {&SELECT_SERIALIZE_FORMAT_C, &SELECT_SERIALIZE_FORMAT_B};


extern Form SELECT_DIRECT_SERIALIZE_FORMAT_C         {{0,"",0," "}, {0,"",1," "}, {0,"",2," "}, {0,"",3," "}, {0,"",4," "}};
extern Form SELECT_DIRECT_SERIALIZE_FORMAT_B         {{0,"",0,"\n"}, {0,"",1," "}, {0,"\n",2," "}, {0,"\n",3," "}, {0,"",4," "}};
extern FmCB SELECT_DIRECT_SERIALIZE_FORMAT           {&SELECT_DIRECT_SERIALIZE_FORMAT_C, &SELECT_DIRECT_SERIALIZE_FORMAT_B};


extern Form SINGLE_WITH_PARENS_SERIALIZE_FORMAT_C    {{1,"("}, {0,"",0,""}, {1,")"}};
/* SINGLE_WITH_PARENS_SERIALIZE_FORMAT_B */
extern FmCB SINGLE_WITH_PARENS_SERIALIZE_FORMAT      {&SINGLE_WITH_PARENS_SERIALIZE_FORMAT_C, &SINGLE_WITH_PARENS_SERIALIZE_FORMAT_C};

extern Form SINGLE_WITH_BRACKETS_SERIALIZE_FORMAT_C    {{1,"["}, {0,"",0,""}, {1,"]"}};
/* SINGLE_WITH_PARENS_SERIALIZE_FORMAT_B */
extern FmCB SINGLE_WITH_BRACKETS_SERIALIZE_FORMAT      {&SINGLE_WITH_BRACKETS_SERIALIZE_FORMAT_C, &SINGLE_WITH_BRACKETS_SERIALIZE_FORMAT_C};

extern Form SELECT_UNION_SERIALIZE_FORMAT_C          {{0,"",8,""}, {1, " UNION "}, {0,"",7," "}, {0,"",9,""}};
extern Form SELECT_UNION_SERIALIZE_FORMAT_B          {{0,"",8,""}, {1, "\n",-1,"UNION "}, {0,"",7," "}, {0,"\n",9,""}};
extern FmCB SELECT_UNION_SERIALIZE_FORMAT            {&SELECT_UNION_SERIALIZE_FORMAT_C, &SELECT_UNION_SERIALIZE_FORMAT_B};

extern Form SELECT_INTERSECT_SERIALIZE_FORMAT_C      {{0,"",8,""}, {1, " INTERSECT "}, {0,"",7," "}, {0,"",9,""}};
extern Form SELECT_INTERSECT_SERIALIZE_FORMAT_B      {{0,"",8,""}, {1, "\n",-1,"INTERSECT "}, {0,"",7," "}, {0,"\n",9,""}};
extern FmCB SELECT_INTERSECT_SERIALIZE_FORMAT        {&SELECT_INTERSECT_SERIALIZE_FORMAT_C, &SELECT_INTERSECT_SERIALIZE_FORMAT_B};

extern Form SELECT_EXCEPT_SERIALIZE_FORMAT_C         {{0,"",8,""}, {1, "  "},{0,"",6," "}, {0,"",7," "}, {0,"",9,""}};
extern Form SELECT_EXCEPT_SERIALIZE_FORMAT_B         {{0,"",8,""}, {1, "\n",-1," "},{0,"",6," "}, {0,"",7," "}, {0,"\n",9,""}};
extern FmCB SELECT_EXCEPT_SERIALIZE_FORMAT           {&SELECT_EXCEPT_SERIALIZE_FORMAT_C, &SELECT_EXCEPT_SERIALIZE_FORMAT_B};

extern Form WHERE_SERIALIZE_FORMAT_C                 {{1,"WHERE "}, {0,"",0,""}};
extern Form WHERE_SERIALIZE_FORMAT_B                 {{1,"WHERE  "}, {0,"",0,""}};
extern FmCB WHERE_SERIALIZE_FORMAT                   {&WHERE_SERIALIZE_FORMAT_C, &WHERE_SERIALIZE_FORMAT_B};

extern Form FROM_SERIALIZE_FORMAT_C                  {{1,"FROM "}, {0,"",0,""}};
extern Form FROM_SERIALIZE_FORMAT_B                  {{1,"FROM   "}, {0,"",0,""}};
extern FmCB FROM_SERIALIZE_FORMAT                    {&FROM_SERIALIZE_FORMAT_C, &FROM_SERIALIZE_FORMAT_B};

extern Form GROUP_BY_SERIALIZE_FORMAT_C              {{1,"GROUP BY "}, {0,"",1," "}, {0,"",0,""}};
/* GROUP_BY_SERIALIZE_FORMAT_B */
extern FmCB GROUP_BY_SERIALIZE_FORMAT                {&GROUP_BY_SERIALIZE_FORMAT_C, &GROUP_BY_SERIALIZE_FORMAT_C};

extern Form ORDER_BY_SERIALIZE_FORMAT_C              {{1,"ORDER BY "}, {0,"",0,""}, {0," ",1,""}};
/* ORDER_BY_SERIALIZE_FORMAT_B */
extern FmCB ORDER_BY_SERIALIZE_FORMAT                {&ORDER_BY_SERIALIZE_FORMAT_C, &ORDER_BY_SERIALIZE_FORMAT_C};
extern Form ORDER_SIBLINGS_BY_SERIALIZE_FORMAT_C     {{1,"ORDER SIBLINGS BY "}, {0,"",0,""}, {0," ",1,""}};
/* ORDER_BY_SERIALIZE_FORMAT_B */
extern FmCB ORDER_SIBLINGS_BY_SERIALIZE_FORMAT       {&ORDER_SIBLINGS_BY_SERIALIZE_FORMAT_C, &ORDER_SIBLINGS_BY_SERIALIZE_FORMAT_C};

extern Form SORT_KEY_SERIALIZE_FORMAT_C              {{0,"",0,""}, {0," ",1,""}};
/* SORT_KEY_SERIALIZE_FORMAT_B */
extern FmCB SORT_KEY_SERIALIZE_FORMAT                {&SORT_KEY_SERIALIZE_FORMAT_C, &SORT_KEY_SERIALIZE_FORMAT_C};


extern Form HAVING_SERIALIZE_FORMAT_C                {{1,"HAVING "}, {0,"",0,""}};
/* HAVING_SERIALIZE_FORMAT_B */
extern FmCB HAVING_SERIALIZE_FORMAT                  {&HAVING_SERIALIZE_FORMAT_C, &HAVING_SERIALIZE_FORMAT_C};

/*----------------------------------------------------------------------------------------------------------------------------------------------*/
extern Form SINGLE_SERIALIZE_FORMAT_C                {{0,"",0,""}};
/* SINGLE_SERIALIZE_FORMAT_B */
extern FmCB SINGLE_SERIALIZE_FORMAT                  {&SINGLE_SERIALIZE_FORMAT_C, &SINGLE_SERIALIZE_FORMAT_C};
extern Form DOUBLE_SERIALIZE_FORMAT_C                {{0,"",0," "}, {0,"",1,""}};
/* DOUBLE_SERIALIZE_FORMAT_B */
extern FmCB DOUBLE_SERIALIZE_FORMAT                  {&DOUBLE_SERIALIZE_FORMAT_C, &DOUBLE_SERIALIZE_FORMAT_C};
extern Form TRIPLE_SERIALIZE_FORMAT_C                {{0,"",0," "}, {0,"",1," "}, {0,"",2," "}};
extern FmCB TRIPLE_SERIALIZE_FORMAT                  {&TRIPLE_SERIALIZE_FORMAT_C, &TRIPLE_SERIALIZE_FORMAT_C};
/*
 * ORACLE only support table alias without keyword AS
 * But, opt AS is supported by most DBMS.
 * */
extern Form AS_LABEL_SERIALIZE_FORMAT_C            {{0,"AS ",0,""}};
extern FmCB AS_LABEL_SERIALIZE_FORMAT              {&AS_LABEL_SERIALIZE_FORMAT_C, &AS_LABEL_SERIALIZE_FORMAT_C};

extern Form AS_SERIALIZE_FORMAT_C                    {{0,"",0,""}, {0," ",4,""}, {0," AS " ,1,""}, {0," ",2,""}, {0," ",3,""}};
/* AS_SERIALIZE_FORMAT_B */
extern FmCB AS_SERIALIZE_FORMAT                      {&AS_SERIALIZE_FORMAT_C, &AS_SERIALIZE_FORMAT_C};

extern Form ORA_AS_SERIALIZE_FORMAT_C                    {{0,"",0,""}, {0," ",4,""}, {0," " ,1,""}, {0," ",2,""}, {0," ",3,""}};
/* ORA_AS_SERIALIZE_FORMAT_B */
extern FmCB ORA_AS_SERIALIZE_FORMAT                      {&ORA_AS_SERIALIZE_FORMAT_C, &ORA_AS_SERIALIZE_FORMAT_C};


extern Form QUADRUPLE_SERIALIZE_FORMAT_C             {{0,"",0,""}, {0," ",1,""}, {0," ",2,""}, {0," ",3,""}};
/* QUADRUPLE_SERIALIZE_FORMAT_B */
extern FmCB QUADRUPLE_SERIALIZE_FORMAT               {&QUADRUPLE_SERIALIZE_FORMAT_C, &QUADRUPLE_SERIALIZE_FORMAT_C};

extern Form PENTA_SERIALIZE_FORMAT_C                 {{0,"",0,""}, {0," ",1,""}, {0," ",2,""}, {0," ",3,""}, {0," ",4,""}};
/* PENTA_SERIALIZE_FORMAT_C */
extern FmCB PENTA_SERIALIZE_FORMAT                   {&PENTA_SERIALIZE_FORMAT_C, &PENTA_SERIALIZE_FORMAT_C};

extern Form PENTA_SERIALIZE_FORMAT_0_C               {{0,"",0,""}, {0," ",4,""}, {0," ",1,""}, {0," ",2,""}, {0," ",3,""}};
/* PENTA_SERIALIZE_FORMAT_0_B */
extern FmCB PENTA_SERIALIZE_FORMAT_0                 {&PENTA_SERIALIZE_FORMAT_0_C, &PENTA_SERIALIZE_FORMAT_0_C};

extern Form JOINED_TB_1_SERIALIZE_FORMAT_C           {{0,"",1," "}, {0,"",0,""}, {1," JOIN "}, {0,"",2,""}, {1,""}, {0," ON ",3,""}};
extern Form JOINED_TB_1_SERIALIZE_FORMAT_B           {{0,"",1," "}, {0,"\n",0," "}, {1,"JOIN "}, {0,"",2,""}, {1,""}, {0," ON ",3,""}};
extern FmCB JOINED_TB_1_SERIALIZE_FORMAT             {&JOINED_TB_1_SERIALIZE_FORMAT_C, &JOINED_TB_1_SERIALIZE_FORMAT_B};


extern Form JOINED_TB_USING_SERIALIZE_FORMAT_C       {{0,"",1," "}, {0,"",0,""}, {1," JOIN "}, {0,"",2,""}, {1," USING"}, {0,"",3,""}};
extern Form JOINED_TB_USING_SERIALIZE_FORMAT_B       {{0,"",1," "}, {0,"\n",0,""}, {1," JOIN "}, {0,"",2,""}, {1,"\n",-1,"USING"}, {0,"",3,""}};
extern FmCB JOINED_TB_USING_SERIALIZE_FORMAT         {&JOINED_TB_USING_SERIALIZE_FORMAT_C, &JOINED_TB_USING_SERIALIZE_FORMAT_B};

extern Form JOINED_TB_2_SERIALIZE_FORMAT_C           {{0,"",1," "}, {0,"",0,""}, {1," JOIN "}, {0,"",2,""}};
extern Form JOINED_TB_2_SERIALIZE_FORMAT_B           {{0,"",1," "}, {0,"\n",0,""}, {1," JOIN "}, {0,"",2,""}};
extern FmCB JOINED_TB_2_SERIALIZE_FORMAT             {&JOINED_TB_2_SERIALIZE_FORMAT_C, &JOINED_TB_2_SERIALIZE_FORMAT_B};

extern Form JOINED_TB_3_SERIALIZE_FORMAT_C           {{0,"",1," "}, {0,"",0,""}, {1," APPLY "}, {0,"",2,""}};
extern Form JOINED_TB_3_SERIALIZE_FORMAT_B           {{0,"",1," "}, {0,"\n",0,""}, {1," APPLY "}, {0,"",2,""}};
extern FmCB JOINED_TB_3_SERIALIZE_FORMAT             {&JOINED_TB_3_SERIALIZE_FORMAT_C, &JOINED_TB_3_SERIALIZE_FORMAT_B};

extern Form OP_EXISTS_SERIALIZE_FORMAT_C             {{1,"EXISTS "}, {0,"",0,""}};
/* OP_EXISTS_SERIALIZE_FORMAT_B */
extern FmCB OP_EXISTS_SERIALIZE_FORMAT               {&OP_EXISTS_SERIALIZE_FORMAT_C, &OP_EXISTS_SERIALIZE_FORMAT_C};

extern Form OP_POS_SERIALIZE_FORMAT_C                {{1,"+"}, {0,"",0,""}};
/* OP_POS_SERIALIZE_FORMAT_B */
extern FmCB OP_POS_SERIALIZE_FORMAT                  {&OP_POS_SERIALIZE_FORMAT_C, &OP_POS_SERIALIZE_FORMAT_C};

extern Form OP_NEG_SERIALIZE_FORMAT_C                {{1,"-"}, {0,"",0,""}};
/* OP_NEG_SERIALIZE_FORMAT_B */
extern FmCB OP_NEG_SERIALIZE_FORMAT                  {&OP_NEG_SERIALIZE_FORMAT_C, &OP_NEG_SERIALIZE_FORMAT_C};

extern Form OP_ADD_SERIALIZE_FORMAT_C                {{0,"",0,""}, {1," + "}, {0,"",1,""}};
/* OP_ADD_SERIALIZE_FORMAT_B */
extern FmCB OP_ADD_SERIALIZE_FORMAT                  {&OP_ADD_SERIALIZE_FORMAT_C, &OP_ADD_SERIALIZE_FORMAT_C};

extern Form OP_MINUS_SERIALIZE_FORMAT_C              {{0,"",0,""}, {1," - "}, {0,"",1,""}};
/* OP_MINUS_SERIALIZE_FORMAT_B */
extern FmCB OP_MINUS_SERIALIZE_FORMAT                {&OP_MINUS_SERIALIZE_FORMAT_C, &OP_MINUS_SERIALIZE_FORMAT_C};

extern Form OP_MUL_SERIALIZE_FORMAT_C                {{0,"",0,""}, {1," * "}, {0,"",1,""}};
/* OP_MUL_SERIALIZE_FORMAT_B */
extern FmCB OP_MUL_SERIALIZE_FORMAT                  {&OP_MUL_SERIALIZE_FORMAT_C, &OP_MUL_SERIALIZE_FORMAT_C};

extern Form OP_DIV_SERIALIZE_FORMAT_C                {{0,"",0,""}, {1," / "}, {0,"",1,""}};
/* OP_DIV_SERIALIZE_FORMAT_C */
extern FmCB OP_DIV_SERIALIZE_FORMAT                  {&OP_DIV_SERIALIZE_FORMAT_C, &OP_DIV_SERIALIZE_FORMAT_C};

extern Form OP_REM_SERIALIZE_FORMAT_C                {{0,"",0,""}, {1," % "}, {0,"",1,""}};
/* OP_REM_SERIALIZE_FORMAT_B */
extern FmCB OP_REM_SERIALIZE_FORMAT                  {&OP_REM_SERIALIZE_FORMAT_C, &OP_REM_SERIALIZE_FORMAT_C};

extern Form OP_POW_SERIALIZE_FORMAT_C                {{0,"",0,""}, {1," ^ "}, {0,"",1,""}};
/* OP_POW_SERIALIZE_FORMAT_C */
extern FmCB OP_POW_SERIALIZE_FORMAT                  {&OP_POW_SERIALIZE_FORMAT_C, &OP_POW_SERIALIZE_FORMAT_C};

extern Form OP_MOD_SERIALIZE_FORMAT_C                {{0,"",0,""}, {1," MOD "}, {0,"",1,""}};
/* OP_MOD_SERIALIZE_FORMAT_B */
extern FmCB OP_MOD_SERIALIZE_FORMAT                  {&OP_MOD_SERIALIZE_FORMAT_C, &OP_MOD_SERIALIZE_FORMAT_C};

extern Form OP_LE_SERIALIZE_FORMAT_C                 {{0,"",0,""}, {1," <= "}, {0,"",1,""}};
/* OP_LE_SERIALIZE_FORMAT_B */
extern FmCB OP_LE_SERIALIZE_FORMAT                   {&OP_LE_SERIALIZE_FORMAT_C, &OP_LE_SERIALIZE_FORMAT_C};

extern Form OP_LT_SERIALIZE_FORMAT_C                 {{0,"",0,""}, {1," < "}, {0,"",1,""}};
/* OP_LT_SERIALIZE_FORMAT_B */
extern FmCB OP_LT_SERIALIZE_FORMAT                   {&OP_LT_SERIALIZE_FORMAT_C, &OP_LT_SERIALIZE_FORMAT_C};

extern Form OP_EQ_SERIALIZE_FORMAT_C                 {{0,"",0,""}, {1," = "}, {0,"",1,""}};
/* OP_EQ_SERIALIZE_FORMAT_B */
extern FmCB OP_EQ_SERIALIZE_FORMAT                   {&OP_EQ_SERIALIZE_FORMAT_C, &OP_EQ_SERIALIZE_FORMAT_C};

extern Form OP_GE_SERIALIZE_FORMAT_C                 {{0,"",0,""}, {1," >= "}, {0,"",1,""}};
/* OP_GE_SERIALIZE_FORMAT_B */
extern FmCB OP_GE_SERIALIZE_FORMAT                   {&OP_GE_SERIALIZE_FORMAT_C, &OP_GE_SERIALIZE_FORMAT_C};

extern Form OP_GT_SERIALIZE_FORMAT_C                 {{0,"",0,""}, {1," > "}, {0,"",1,""}};
/* OP_GT_SERIALIZE_FORMAT_B */
extern FmCB OP_GT_SERIALIZE_FORMAT                   {&OP_GT_SERIALIZE_FORMAT_C, &OP_GT_SERIALIZE_FORMAT_C};

extern Form OP_NE_SERIALIZE_FORMAT_C                 {{0,"",0,""}, {1," <> "}, {0,"",1,""}};
/* OP_NE_SERIALIZE_FORMAT_B */
extern FmCB OP_NE_SERIALIZE_FORMAT                   {&OP_NE_SERIALIZE_FORMAT_C, &OP_NE_SERIALIZE_FORMAT_C};

extern Form OP_LE_ALL_SERIALIZE_FORMAT_C             {{0,"",0,""}, {1," <= ALL "}, {0,"",1,""}};
/* OP_LE_ALL_SERIALIZE_FORMAT_C */
extern FmCB OP_LE_ALL_SERIALIZE_FORMAT               {&OP_LE_ALL_SERIALIZE_FORMAT_C, &OP_LE_ALL_SERIALIZE_FORMAT_C};

extern Form OP_LT_ALL_SERIALIZE_FORMAT_C             {{0,"",0,""}, {1," < ALL "}, {0,"",1,""}};
/* OP_LT_ALL_SERIALIZE_FORMAT_B */
extern FmCB OP_LT_ALL_SERIALIZE_FORMAT               {&OP_LT_ALL_SERIALIZE_FORMAT_C, &OP_LT_ALL_SERIALIZE_FORMAT_C};

extern Form OP_EQ_ALL_SERIALIZE_FORMAT_C             {{0,"",0,""}, {1," = ALL "}, {0,"",1,""}};
/* OP_EQ_ALL_SERIALIZE_FORMAT_B */
extern FmCB OP_EQ_ALL_SERIALIZE_FORMAT               {&OP_EQ_ALL_SERIALIZE_FORMAT_C, &OP_EQ_ALL_SERIALIZE_FORMAT_C};

extern Form OP_GE_ALL_SERIALIZE_FORMAT_C             {{0,"",0,""}, {1," >= ALL "}, {0,"",1,""}};
/* OP_GE_ALL_SERIALIZE_FORMAT_B */
extern FmCB OP_GE_ALL_SERIALIZE_FORMAT               {&OP_GE_ALL_SERIALIZE_FORMAT_C, &OP_GE_ALL_SERIALIZE_FORMAT_C};

extern Form OP_GT_ALL_SERIALIZE_FORMAT_C             {{0,"",0,""}, {1," > ALL "}, {0,"",1,""}};
/* OP_GT_ALL_SERIALIZE_FORMAT_B */
extern FmCB OP_GT_ALL_SERIALIZE_FORMAT               {&OP_GT_ALL_SERIALIZE_FORMAT_C, &OP_GT_ALL_SERIALIZE_FORMAT_C};

extern Form OP_NE_ALL_SERIALIZE_FORMAT_C             {{0,"",0,""}, {1," <> ALL "}, {0,"",1,""}};
/* OP_NE_ALL_SERIALIZE_FORMAT_B */
extern FmCB OP_NE_ALL_SERIALIZE_FORMAT               {&OP_NE_ALL_SERIALIZE_FORMAT_C, &OP_NE_ALL_SERIALIZE_FORMAT_C};

extern Form OP_LE_SOME_SERIALIZE_FORMAT_C            {{0,"",0,""}, {1," <= SOME "}, {0,"",1,""}};
/* OP_LE_SOME_SERIALIZE_FORMAT_B */
extern FmCB OP_LE_SOME_SERIALIZE_FORMAT              {&OP_LE_SOME_SERIALIZE_FORMAT_C, &OP_LE_SOME_SERIALIZE_FORMAT_C};

extern Form OP_LT_SOME_SERIALIZE_FORMAT_C            {{0,"",0,""}, {1," < SOME "}, {0,"",1,""}};
/* OP_LT_SOME_SERIALIZE_FORMAT_B */
extern FmCB OP_LT_SOME_SERIALIZE_FORMAT              {&OP_LT_SOME_SERIALIZE_FORMAT_C, &OP_LT_SOME_SERIALIZE_FORMAT_C};

extern Form OP_EQ_SOME_SERIALIZE_FORMAT_C            {{0,"",0,""}, {1," = SOME "}, {0,"",1,""}};
/* OP_EQ_SOME_SERIALIZE_FORMAT_B */
extern FmCB OP_EQ_SOME_SERIALIZE_FORMAT              {&OP_EQ_SOME_SERIALIZE_FORMAT_C, &OP_EQ_SOME_SERIALIZE_FORMAT_C};

extern Form OP_GE_SOME_SERIALIZE_FORMAT_C            {{0,"",0,""}, {1," >= SOME "}, {0,"",1,""}};
/* OP_GE_SOME_SERIALIZE_FORMAT_B */
extern FmCB OP_GE_SOME_SERIALIZE_FORMAT              {&OP_GE_SOME_SERIALIZE_FORMAT_C, &OP_GE_SOME_SERIALIZE_FORMAT_C};

extern Form OP_GT_SOME_SERIALIZE_FORMAT_C            {{0,"",0,""}, {1," > SOME "}, {0,"",1,""}};
/* OP_GT_SOME_SERIALIZE_FORMAT_B */
extern FmCB OP_GT_SOME_SERIALIZE_FORMAT              {&OP_GT_SOME_SERIALIZE_FORMAT_C, &OP_GT_SOME_SERIALIZE_FORMAT_C};

extern Form OP_NE_SOME_SERIALIZE_FORMAT_C            {{0,"",0,""}, {1," <> SOME "}, {0,"",1,""}};
/* OP_NE_SOME_SERIALIZE_FORMAT_B */
extern FmCB OP_NE_SOME_SERIALIZE_FORMAT              {&OP_NE_SOME_SERIALIZE_FORMAT_C, &OP_NE_SOME_SERIALIZE_FORMAT_C};

extern Form OP_LE_ANY_SERIALIZE_FORMAT_C             {{0,"",0,""}, {1," <= ANY "}, {0,"",1,""}};
/* OP_LE_ANY_SERIALIZE_FORMAT_B */
extern FmCB OP_LE_ANY_SERIALIZE_FORMAT               {&OP_LE_ANY_SERIALIZE_FORMAT_C, &OP_LE_ANY_SERIALIZE_FORMAT_C};

extern Form OP_LT_ANY_SERIALIZE_FORMAT_C             {{0,"",0,""}, {1," < ANY "}, {0,"",1,""}};
/* OP_LT_ANY_SERIALIZE_FORMAT_B */
extern FmCB OP_LT_ANY_SERIALIZE_FORMAT               {&OP_LT_ANY_SERIALIZE_FORMAT_C, &OP_LT_ANY_SERIALIZE_FORMAT_C};

extern Form OP_EQ_ANY_SERIALIZE_FORMAT_C             {{0,"",0,""}, {1," = ANY "}, {0,"",1,""}};
/* OP_EQ_ANY_SERIALIZE_FORMAT_C */
extern FmCB OP_EQ_ANY_SERIALIZE_FORMAT               {&OP_EQ_ANY_SERIALIZE_FORMAT_C, &OP_EQ_ANY_SERIALIZE_FORMAT_C};

extern Form OP_GE_ANY_SERIALIZE_FORMAT_C             {{0,"",0,""}, {1," >= ANY "}, {0,"",1,""}};
/* OP_GE_ANY_SERIALIZE_FORMAT_B */
extern FmCB OP_GE_ANY_SERIALIZE_FORMAT               {&OP_GE_ANY_SERIALIZE_FORMAT_C, &OP_GE_ANY_SERIALIZE_FORMAT_C};

extern Form OP_GT_ANY_SERIALIZE_FORMAT_C             {{0,"",0,""}, {1," > ANY "}, {0,"",1,""}};
/* OP_GT_ANY_SERIALIZE_FORMAT_B */
extern FmCB OP_GT_ANY_SERIALIZE_FORMAT               {&OP_GT_ANY_SERIALIZE_FORMAT_C, &OP_GT_ANY_SERIALIZE_FORMAT_C};

extern Form OP_NE_ANY_SERIALIZE_FORMAT_C             {{0,"",0,""}, {1," <> ANY "}, {0,"",1,""}};
/* OP_NE_ANY_SERIALIZE_FORMAT_B */
extern FmCB OP_NE_ANY_SERIALIZE_FORMAT               {&OP_NE_ANY_SERIALIZE_FORMAT_C, &OP_NE_ANY_SERIALIZE_FORMAT_C};

extern Form OP_LIKE_SERIALIZE_FORMAT_C               {{0,"",0,""}, {1," LIKE "}, {0,"",1,""}, {0," ESCAPE ",2,""}};
/* OP_LIKE_SERIALIZE_FORMAT_B */
extern FmCB OP_LIKE_SERIALIZE_FORMAT                 {&OP_LIKE_SERIALIZE_FORMAT_C, &OP_LIKE_SERIALIZE_FORMAT_C};

extern Form OP_NOT_LIKE_SERIALIZE_FORMAT_C           {{0,"",0,""}, {1," NOT LIKE "}, {0,"",1,""}, {0," ESCAPE ",2,""}};
/* OP_NOT_LIKE_SERIALIZE_FORMAT_B */
extern FmCB OP_NOT_LIKE_SERIALIZE_FORMAT             {&OP_NOT_LIKE_SERIALIZE_FORMAT_C, &OP_NOT_LIKE_SERIALIZE_FORMAT_C};

extern Form OP_AND_SERIALIZE_FORMAT_C                {{0,"",0,""}, {1," AND "}, {0,"",1,""}};
extern Form OP_AND_SERIALIZE_FORMAT_B                {{0,"",0,""}, {1,"\n",-1,"AND "}, {0,"",1,""}};
extern FmCB OP_AND_SERIALIZE_FORMAT                  {&OP_AND_SERIALIZE_FORMAT_C, &OP_AND_SERIALIZE_FORMAT_B};

extern Form OP_OR_SERIALIZE_FORMAT_C                 {{0,"",0,""}, {1," OR "}, {0,"",1,""}};
extern Form OP_OR_SERIALIZE_FORMAT_B                 {{0,"",0,""}, {1,"\n",-1," OR "}, {0,"",1,""}};
extern FmCB OP_OR_SERIALIZE_FORMAT                   {&OP_OR_SERIALIZE_FORMAT_C, &OP_OR_SERIALIZE_FORMAT_B};

extern Form OP_NOT_SERIALIZE_FORMAT_C                {{1,"NOT "}, {0,"",0,""}};
/* OP_NOT_SERIALIZE_FORMAT_B */
extern FmCB OP_NOT_SERIALIZE_FORMAT                  {&OP_NOT_SERIALIZE_FORMAT_C, &OP_NOT_SERIALIZE_FORMAT_C};

extern Form OP_IS_SERIALIZE_FORMAT_C                 {{0,"",0,""}, {1," IS "}, {0,"",1,""}};
/* OP_IS_SERIALIZE_FORMAT_B */
extern FmCB OP_IS_SERIALIZE_FORMAT                   {&OP_IS_SERIALIZE_FORMAT_C, &OP_IS_SERIALIZE_FORMAT_C};

extern Form OP_IS_NOT_SERIALIZE_FORMAT_C             {{0,"",0,""}, {1," IS NOT "}, {0,"",1,""}};
/* OP_IS_NOT_SERIALIZE_FORMAT */
extern FmCB OP_IS_NOT_SERIALIZE_FORMAT               {&OP_IS_NOT_SERIALIZE_FORMAT_C, &OP_IS_NOT_SERIALIZE_FORMAT_C};

extern Form OP_BETWEEN_SERIALIZE_FORMAT_C            {{0,"",0,""}, {1," BETWEEN "}, {0,"",1,""}, {1," AND "}, {0,"",2,""}};
/* OP_BETWEEN_SERIALIZE_FORMAT_B */
extern FmCB OP_BETWEEN_SERIALIZE_FORMAT              {&OP_BETWEEN_SERIALIZE_FORMAT_C, &OP_BETWEEN_SERIALIZE_FORMAT_C};

extern Form OP_NOT_BETWEEN_SERIALIZE_FORMAT_C        {{0,"",0,""}, {1," NOT BETWEEN "}, {0,"",1,""}, {1," AND "}, {0,"",2,""}};
/* OP_NOT_BETWEEN_SERIALIZE_FORMAT_B */
extern FmCB OP_NOT_BETWEEN_SERIALIZE_FORMAT          {&OP_NOT_BETWEEN_SERIALIZE_FORMAT_C, &OP_NOT_BETWEEN_SERIALIZE_FORMAT_C};

extern Form OP_IN_SERIALIZE_FORMAT_C                 {{0,"",0,""}, {1," IN "}, {0,"",1,""}};
/* OP_IN_SERIALIZE_FORMAT_B */
extern FmCB OP_IN_SERIALIZE_FORMAT                   {&OP_IN_SERIALIZE_FORMAT_C, &OP_IN_SERIALIZE_FORMAT_C};

extern Form OP_NOT_IN_SERIALIZE_FORMAT_C             {{0,"",0,""}, {1," NOT IN "}, {0,"",1,""}};
/* OP_NOT_IN_SERIALIZE_FORMAT_B */
extern FmCB OP_NOT_IN_SERIALIZE_FORMAT               {&OP_NOT_IN_SERIALIZE_FORMAT_C, &OP_NOT_IN_SERIALIZE_FORMAT_C};

extern Form OP_CNN_SERIALIZE_FORMAT_C                {{0,"",0,""}, {1," || "}, {0,"",1,""}};
/* OP_CNN_SERIALIZE_FORMAT_B */
extern FmCB OP_CNN_SERIALIZE_FORMAT                  {&OP_CNN_SERIALIZE_FORMAT_C, &OP_CNN_SERIALIZE_FORMAT_C};

//E_BIT_OP_AND,// &//E_BIT_OP_OR, // |//E_BIT_OP_NOT,// ~//E_BIT_OP_LS,// <<//E_BIT_OP_RS, >>
extern Form OP_BIT_LS_SERIALIZE_FORMAT_C                {{0,"",0,""}, {1," << "}, {0,"",1,""}};
extern FmCB OP_BIT_LS_SERIALIZE_FORMAT                  {&OP_BIT_LS_SERIALIZE_FORMAT_C, &OP_BIT_LS_SERIALIZE_FORMAT_C};
extern Form OP_BIT_RS_SERIALIZE_FORMAT_C                {{0,"",0,""}, {1," >> "}, {0,"",1,""}};
extern FmCB OP_BIT_RS_SERIALIZE_FORMAT                  {&OP_BIT_RS_SERIALIZE_FORMAT_C, &OP_BIT_RS_SERIALIZE_FORMAT_C};
extern Form OP_BIT_AND_SERIALIZE_FORMAT_C                {{0,"",0,""}, {1," & "}, {0,"",1,""}};
extern FmCB OP_BIT_AND_SERIALIZE_FORMAT                  {&OP_BIT_AND_SERIALIZE_FORMAT_C, &OP_BIT_AND_SERIALIZE_FORMAT_C};
extern Form OP_BIT_OR_SERIALIZE_FORMAT_C                {{0,"",0,""}, {1," | "}, {0,"",1,""}};
extern FmCB OP_BIT_OR_SERIALIZE_FORMAT                  {&OP_BIT_OR_SERIALIZE_FORMAT_C, &OP_BIT_OR_SERIALIZE_FORMAT_C};
extern Form OP_BIT_NOT_SERIALIZE_FORMAT_C                { {1,"~"}, {0,"",0,""}};
extern FmCB OP_BIT_NOT_SERIALIZE_FORMAT                  {&OP_BIT_NOT_SERIALIZE_FORMAT_C, &OP_BIT_NOT_SERIALIZE_FORMAT_C};


extern Form CASE_SERIALIZE_FORMAT_C                  {{1,"CASE "}, {0,"",0," "}, {0,"",1,""}, {0," ",2,""}, {1," END"}};
extern Form CASE_SERIALIZE_FORMAT_B                  {{1,"CASE "}, {0,"",0," "}, {0,"\n",1,""}, {0,"\n",2,""}, {1,"\n",-1,"END"}};
extern FmCB CASE_SERIALIZE_FORMAT                    {&CASE_SERIALIZE_FORMAT_C, &CASE_SERIALIZE_FORMAT_B};

extern Form WHEN_SERIALIZE_FORMAT_C                  {{1,"  WHEN "}, {0,"",0,""}, {1," THEN "}, {0,"",1,""}};
/* WHEN_SERIALIZE_FORMAT_B */
extern FmCB WHEN_SERIALIZE_FORMAT                    {&WHEN_SERIALIZE_FORMAT_C, &WHEN_SERIALIZE_FORMAT_C};

extern Form ELSE_SERIALIZE_FORMAT_C                  {{1,"  ELSE "}, {0,"",0,""}};
/* ELSE_SERIALIZE_FORMAT_B */
extern FmCB ELSE_SERIALIZE_FORMAT                    {&ELSE_SERIALIZE_FORMAT_C, &ELSE_SERIALIZE_FORMAT_C};

extern Form FUN_CALL_1_OVER_SERIALIZE_FORMAT_C       {{0,"",0,""}, {1,"("}, {0,"",1,""}, {1,")"}, {0," ",2,""}};
/* FUN_CALL_1_OVER_SERIALIZE_FORMAT_B */
extern FmCB FUN_CALL_1_OVER_SERIALIZE_FORMAT         {&FUN_CALL_1_OVER_SERIALIZE_FORMAT_C, &FUN_CALL_1_OVER_SERIALIZE_FORMAT_C};

extern Form FUN_CALL_DIS_OVER_SERIALIZE_FORMAT_C   {{0,"",0,""}, {1,"("},
                                             {0,"",1," "},
                                             {0,"",2," "},
                                             {0,"",3," "}, {1,")"},
                                             {0," ",4,""}};
/* FUN_CALL_DIS_OVER_SERIALIZE_FORMAT_B */
extern FmCB FUN_CALL_DIS_OVER_SERIALIZE_FORMAT       {&FUN_CALL_DIS_OVER_SERIALIZE_FORMAT_C, &FUN_CALL_DIS_OVER_SERIALIZE_FORMAT_C};

extern Form FUN_CALL_BIGQUERY_S_FORMAT_C     {{0,"",0,""}, {1,"("}, {0,"",1,""}, {0,",",2,""}, {0,",",3,""}, {1,")"}, {0," ",4,""}};
/* FUN_CALL_BIGQUERY_SERIALIZE_FORMAT_B */
extern FmCB FUN_CALL_BIGQUERY_S_FORMAT       {&FUN_CALL_BIGQUERY_S_FORMAT_C, &FUN_CALL_BIGQUERY_S_FORMAT_C};


extern Form FUN_CALL_AS_SERIALIZE_FORMAT_C           {{0,"",0,""}, {1,"("}, {0,"",1,""}, {1," AS "}, {0,"",4,""}, {1,")"}};
/* FUN_CALL_AS_SERIALIZE_FORMAT_B */
extern FmCB FUN_CALL_AS_SERIALIZE_FORMAT             {&FUN_CALL_AS_SERIALIZE_FORMAT_C, &FUN_CALL_AS_SERIALIZE_FORMAT_C};

extern Form FUN_CALL_USING_SERIALIZE_FORMAT_C        {{0,"",0,""}, {1,"("}, {0,"",1,""}, {1," USING "}, {0,"",4,""}, {1,")"}};
/* FUN_CALL_USING_SERIALIZE_FORMAT_B */
extern FmCB FUN_CALL_USING_SERIALIZE_FORMAT          {&FUN_CALL_USING_SERIALIZE_FORMAT_C, &FUN_CALL_USING_SERIALIZE_FORMAT_C};

extern Form WITH_CLAUSE_SERIALIZE_FORMAT_C           {{1,"WITH "}, {0,"",0,""}};
/* WITH_CLAUSE_SERIALIZE_FORMAT_B */
extern FmCB WITH_CLAUSE_SERIALIZE_FORMAT             {&WITH_CLAUSE_SERIALIZE_FORMAT_C, &WITH_CLAUSE_SERIALIZE_FORMAT_C};

extern Form WITH_RECURSIVE_CLAUSE_SERIALIZE_FORMAT_C {{1,"WITH RECURSIVE "}, {0,"",0,""}};
/* WITH_RECURSIVE_CLAUSE_SERIALIZE_FORMAT_B */
extern FmCB WITH_RECURSIVE_CLAUSE_SERIALIZE_FORMAT   {&WITH_RECURSIVE_CLAUSE_SERIALIZE_FORMAT_C, &WITH_RECURSIVE_CLAUSE_SERIALIZE_FORMAT_C};

extern Form COMMON_TABLE_EXPR_SERIALIZE_FORMAT_C     {{0,"",0,""}, {0,"",1,""}, {1," AS "}, {0,"",2,""}};
//extern Form COMMON_TABLE_EXPR_SERIALIZE_FORMAT_B     {{0,"",0,""}, {0,"",1,""}, {1," AS "}, {0,"",2,""}};
extern FmCB COMMON_TABLE_EXPR_SERIALIZE_FORMAT       {&COMMON_TABLE_EXPR_SERIALIZE_FORMAT_C, &COMMON_TABLE_EXPR_SERIALIZE_FORMAT_C};


extern Form TABLE_IDENT_SERIALIZE_FORMAT_1_C         {{0,"",1,""}, {1,"."}, {0,"",0,""}};
/* TABLE_IDENT_SERIALIZE_FORMAT_1_B */
extern FmCB TABLE_IDENT_SERIALIZE_FORMAT_1           {&TABLE_IDENT_SERIALIZE_FORMAT_1_C, &TABLE_IDENT_SERIALIZE_FORMAT_1_C};

extern Form TABLE_IDENT_SERIALIZE_FORMAT_2_C         {{0,"",2,""}, {1,"."}, {0,"",1,""}, {1,"."}, {0,"",0,""}};
/* TABLE_IDENT_SERIALIZE_FORMAT_2_B */
extern FmCB TABLE_IDENT_SERIALIZE_FORMAT_2             {&TABLE_IDENT_SERIALIZE_FORMAT_2_C, &TABLE_IDENT_SERIALIZE_FORMAT_2_C};

extern Form TABLE_IDENT_SERIALIZE_FORMAT_3_C         {{0,"",3,""}, {1,"."}, {0,"",2,""}, {1,"."}, {0,"",1,""}, {1,"."}, {0,"",0,""}};
/* TABLE_IDENT_SERIALIZE_FORMAT_3_B */
extern FmCB TABLE_IDENT_SERIALIZE_FORMAT_3           {&TABLE_IDENT_SERIALIZE_FORMAT_3_C, &TABLE_IDENT_SERIALIZE_FORMAT_3_C};
//
//extern Form OP_NAME_FIELD_SERIALIZE_FORMAT_1_C       {{0,"",1,""}, {1,"."}, {0,"",0,""}};
///* OP_NAME_FIELD_SERIALIZE_FORMAT_1_B */
//extern FmCB OP_NAME_FIELD_SERIALIZE_FORMAT_1         {&OP_NAME_FIELD_SERIALIZE_FORMAT_1_C, &OP_NAME_FIELD_SERIALIZE_FORMAT_1_C};
//
//extern Form OP_NAME_FIELD_SERIALIZE_FORMAT_2_C       {{0,"",2,""}, {1,"."}, {0,"",1,""}, {1,"."}, {0,"",0,""}};
///* OP_NAME_FIELD_SERIALIZE_FORMAT_2_B */
//extern FmCB OP_NAME_FIELD_SERIALIZE_FORMAT_2         {&OP_NAME_FIELD_SERIALIZE_FORMAT_2_C, &OP_NAME_FIELD_SERIALIZE_FORMAT_2_C};
//
//extern Form OP_NAME_FIELD_SERIALIZE_FORMAT_3_C       {{0,"",3,""}, {1,"."}, {0,"",2,""}, {1,"."}, {0,"",1,""}, {1,"."}, {0,"",0,""}};
///* OP_NAME_FIELD_SERIALIZE_FORMAT_3_B */
//extern FmCB OP_NAME_FIELD_SERIALIZE_FORMAT_3         {&OP_NAME_FIELD_SERIALIZE_FORMAT_3_C, &OP_NAME_FIELD_SERIALIZE_FORMAT_3_C};

extern Form OP_NAME_FIELD_SERIALIZE_FORMAT_4_C       {{0,"",4,"."},  {0,"",3,"."}, {0,"",2,"."},  {0,"",1,"."},  {0,"",0,""}};
/* OP_NAME_FIELD_SERIALIZE_FORMAT_4_B */
extern FmCB OP_NAME_FIELD_SERIALIZE_FORMAT_4         {&OP_NAME_FIELD_SERIALIZE_FORMAT_4_C, &OP_NAME_FIELD_SERIALIZE_FORMAT_4_C};

extern Form OVER_CLAUSE_SERIALIZE_FORMAT_C           {{1,"OVER"}, {0,"",0,""}};
/* OVER_CLAUSE_SERIALIZE_FORMAT_B */
extern FmCB OVER_CLAUSE_SERIALIZE_FORMAT             {&OVER_CLAUSE_SERIALIZE_FORMAT_C, &OVER_CLAUSE_SERIALIZE_FORMAT_C};

extern Form WINDOW_SPECIFIC_CLAUSE_SERIALIZE_FORMAT_C           {{1,"("}, {0,"",0,""}, {0," PARTITION BY ",1,""}, {0," ",2,""}, {0," ",3,""}, {1, ")"}};
/* WINDOW_SPECIFIC_CLAUSE_SERIALIZE_FORMAT_B */
extern FmCB WINDOW_SPECIFIC_CLAUSE_SERIALIZE_FORMAT             {&WINDOW_SPECIFIC_CLAUSE_SERIALIZE_FORMAT_C, &WINDOW_SPECIFIC_CLAUSE_SERIALIZE_FORMAT_C};

extern Form TABLE_VALUE_CTOR_SERIALIZE_FORMAT_C      {{1,"VALUES "}, {0,"",0,""}};
/* TABLE_VALUE_CTOR_SERIALIZE_FORMAT_B */
extern FmCB TABLE_VALUE_CTOR_SERIALIZE_FORMAT        {&TABLE_VALUE_CTOR_SERIALIZE_FORMAT_C, &TABLE_VALUE_CTOR_SERIALIZE_FORMAT_C};

extern Form CREATE_TABLE_SERIALIZE_FORMAT_C          {{1,"CREATE "}, {1,"TABLE "}, {0, "", 0, " "}, {0,"",1}};
extern FmCB CREATE_TABLE_SERIALIZE_FORMAT            {&CREATE_TABLE_SERIALIZE_FORMAT_C, &CREATE_TABLE_SERIALIZE_FORMAT_C};

extern Form DROP_TABLE_SERIALIZE_FORMAT_C            {{1,"DROP "}, {1,"TABLE "}, {0,"",0," "}, {0,"",1,""}};
extern Form DROP_TABLE_SERIALIZE_FORMAT_B            {{1,"DROP "}, {1,"TABLE "}, {0,"",0," "}, {0,"",1,""}};
extern FmCB DROP_TABLE_SERIALIZE_FORMAT              {&DROP_TABLE_SERIALIZE_FORMAT_C, &DROP_TABLE_SERIALIZE_FORMAT_B};

extern Form UPDATE_SERIALIZE_FORMAT_C                {{0,"",0," "}, {1,"UPDATE "}, {0,"",1," "}, {0,"",2," "}, {0,"",3," "}
        ,{1,"SET "}, {0,"",4," "}, {0,"",5," "}, {0,"",6," "}, {0,"",7," "}, {0,"",8,""}};
extern Form UPDATE_SERIALIZE_FORMAT_B                {{0,"",0,"\n"}, {1,"UPDATE "}, {0,"",1," "}, {0,"",2," "}, {0,"",3," "}
        ,{1,"\n",-1,"SET "}, {0,"",4," "}, {0,"",5," "}, {0,"",6," "}, {0,"\n",7," "}, {0,"",8,""}};
extern FmCB UPDATE_SERIALIZE_FORMAT                  {&UPDATE_SERIALIZE_FORMAT_C, &UPDATE_SERIALIZE_FORMAT_B};

extern Form DELETE_SERIALIZE_FORMAT_C                {{0,"",0," "}, {1, "DELETE "}, {0,"",1," "}, {0,"FROM ",2," "}, {0,"",3," "}
        , {0,"",4," "}, {0,"",5," "}, {0,"",6," "}, {0,"",7," "}};
extern Form DELETE_SERIALIZE_FORMAT_B                {{0,"",0," "}, {1, "DELETE "}, {0,"",1," "}, {0,"FROM ",2," "}, {0,"",3," "}
        , {0,"",4," "}, {0,"",5," "}, {0,"\n",6," "}, {0,"",7," "}};
extern FmCB DELETE_SERIALIZE_FORMAT                  {&DELETE_SERIALIZE_FORMAT_C, &DELETE_SERIALIZE_FORMAT_B};

extern Form DELETE_SERIALIZE_FORMAT_V2_C                {{0,"",0," "}, {1, "DELETE "}, {0,"",1," "}, {0," ",2," "}, {0,"",3," "}
        , {0,"",4," "}, {0,"",5," "}, {0,"",6," "}, {0,"",7," "}};
extern Form DELETE_SERIALIZE_FORMAT_V2_B                {{0,"",0," "}, {1, "DELETE "}, {0,"",1," "}, {0," ",2," "}, {0,"",3," "}
        , {0,"",4," "}, {0,"",5," "}, {0,"\n",6," "}, {0,"",7," "}};
extern FmCB DELETE_SERIALIZE_FORMAT_V2                  {&DELETE_SERIALIZE_FORMAT_V2_C, &DELETE_SERIALIZE_FORMAT_V2_B};

extern Form INSERT_SERIALIZE_FORMAT_C                {{1,"INSERT INTO "}, {0,"",0," "}, {0, "",1,""}};
extern Form INSERT_SERIALIZE_FORMAT_B                {{1,"INSERT INTO "}, {0,"",0," "}, {0, "\n",1,""}};
extern FmCB INSERT_SERIALIZE_FORMAT                  {&INSERT_SERIALIZE_FORMAT_C, &INSERT_SERIALIZE_FORMAT_B};
extern Form INSERT_SERIALIZE_FORMAT_V2_C                {{1,"INSERT "}, {0,"",0," "}, {0, "",1,""}};
extern Form INSERT_SERIALIZE_FORMAT_V2_B                {{1,"INSERT "}, {0,"",0," "}, {0, "\n",1,""}};
extern FmCB INSERT_SERIALIZE_FORMAT_V2                  {&INSERT_SERIALIZE_FORMAT_V2_C, &INSERT_SERIALIZE_FORMAT_V2_B};
extern Form INSERT_SERIALIZE_FORMAT_V3_C                {{1,"INSERT "}, {0, "",2,""},{1," INTO "}, {0,"",0," "}, {0, "",1," "}};
extern Form INSERT_SERIALIZE_FORMAT_V3_B                {{1,"INSERT "}, {0, "",2,""},{1," INTO "}, {0,"",0," "}, {0, "\n",1," "}};
extern FmCB INSERT_SERIALIZE_FORMAT_V3                  {&INSERT_SERIALIZE_FORMAT_V3_C, &INSERT_SERIALIZE_FORMAT_V3_B};

extern Form CALL_SERIALIZE_FORMAT_C                  {{1, "CALL "}, {0,"",0,"("}, {0,"",1,""}, {1,")"}};
/* CALL_SERIALIZE_FORMAT_B */
extern FmCB CALL_SERIALIZE_FORMAT                    {&CALL_SERIALIZE_FORMAT_C, &CALL_SERIALIZE_FORMAT_C};

extern Form CALL_SQL_SERVER_SERIALIZE_FORMAT_C       {{1, "{CALL "}, {0,"",0,"("}, {0,"",1,""}, {1,")}"}};
/* CALL_SQL_SERVER_SERIALIZE_FORMAT_B */
extern FmCB CALL_SQL_SERVER_SERIALIZE_FORMAT         {&CALL_SQL_SERVER_SERIALIZE_FORMAT_C, &CALL_SQL_SERVER_SERIALIZE_FORMAT_C};

extern Form CALL_HANA_SERIALIZE_FORMAT_C       {{1, "CALL "}, {0,"",0," "}, {1,"("},{0,"",1,""},{1,")"}, {0,"",2,""}};
/* CALL_SQL_SERVER_SERIALIZE_FORMAT_B */
extern FmCB CALL_HANA_SERIALIZE_FORMAT         {&CALL_HANA_SERIALIZE_FORMAT_C, &CALL_HANA_SERIALIZE_FORMAT_C};

extern Form CALL_HANA_SERIALIZE_FORMAT2_C       {{1, "CALL "}, {0,"",0," "}, {0,"",1," "}, {0,"",2,""}};
/* CALL_SQL_SERVER_SERIALIZE_FORMAT_B */
extern FmCB CALL_HANA_SERIALIZE_FORMAT2         {&CALL_HANA_SERIALIZE_FORMAT2_C, &CALL_HANA_SERIALIZE_FORMAT2_C};

extern Form SQL_ARGUMENT_SERIALIZE_FORMAT_C          {{0,"",0,""}, {0," AS ",1,""}};
/* SQL_ARGUMENT_SERIALIZE_FORMAT_B */
extern FmCB SQL_ARGUMENT_SERIALIZE_FORMAT            {&SQL_ARGUMENT_SERIALIZE_FORMAT_C, &SQL_ARGUMENT_SERIALIZE_FORMAT_C};

extern Form USE_SERIALIZE_FORMAT_C                     {{0," USE ",0,""}};
extern FmCB USE_SERIALIZE_FORMAT                       {&USE_SERIALIZE_FORMAT_C,&USE_SERIALIZE_FORMAT_C};



/*--------HANA----------*/

extern Form OPTION_STRING_FORMAT_C {{0,"",0," "},{1,"= "},{0,"",1," "}};
extern FmCB OPTION_STRING_FORMAT   {&OPTION_STRING_FORMAT_C,&OPTION_STRING_FORMAT_C};
extern Form RETURNS_CLAUSE_FORMAT_C {{1," RETURNS "}, {0,"",0," "}};
extern FmCB RETURNS_CLAUSE_FORMAT  {&RETURNS_CLAUSE_FORMAT_C,&RETURNS_CLAUSE_FORMAT_C};
extern Form FOR_CLAUSE_FORMAT_C {{1,"FOR "},{0,"",0," "},{0,"",1," "},{0,"",2," "},{0,"",3," "}};
extern FmCB FOR_CLAUSE_FORMAT      {&FOR_CLAUSE_FORMAT_C,&FOR_CLAUSE_FORMAT_C};

extern Form WITHIN_GROUP_SF  {{0,"WITHIN GROUP(",0,")"}};
extern FmCB WITHIN_GROUP_SERIALIZE_FORMAT         {&WITHIN_GROUP_SF,&WITHIN_GROUP_SF};

extern Form TIME_TRAVEL_FORMAT_C  {{1,"AS OF "},{0,"",0," "},{0,"",1," "}};
extern FmCB TIME_TRAVEL_FORMAT {&TIME_TRAVEL_FORMAT_C,&TIME_TRAVEL_FORMAT_C};

extern Form TABLE_VAR_SF  {{0,":",0,""}};
extern FmCB TABLE_VAR_SERIALIZE_FORMAT         {&TABLE_VAR_SF,&TABLE_VAR_SF};
extern Form ASSOCIATED_TAB_SF {{0,"",0,""},{0,"[",1,"]"},{0,":",2," "}};//relation_factor '[' opt_search_condition ']' ':' associated_ref_list
extern FmCB ASSOCIATED_TAB_SERIALIZE_FORMAT    {&ASSOCIATED_TAB_SF,&ASSOCIATED_TAB_SF};
extern Form ASSOCIATED_LIST_SF {{0,"",0,""},{0,".",1," "}};
extern FmCB ASSOCIATED_LIST_SERIALIZE_FORMAT   {&ASSOCIATED_LIST_SF,&ASSOCIATED_LIST_SF};
extern Form ASSOCIATED_REF_SF {{0,"",0,""},{1,"["},{0,"",1," "},{0,"",2," "},{1,"]"}};//relation_factor '[' opt_search_condition opt_many2one_part ']'
extern FmCB ASSOCIATED_REF_SERIALIZE_FORMAT    {&ASSOCIATED_REF_SF,&ASSOCIATED_REF_SF};


extern Form LATERAL_QUERY_SERIALIZE_FORMAT_C {{1,"LATERAL"},{0,"(",0,")"}};
extern FmCB LATERAL_QUERY_SERIALIZE_FORMAT {&LATERAL_QUERY_SERIALIZE_FORMAT_C,&LATERAL_QUERY_SERIALIZE_FORMAT_C};

extern Form CASE_JOIN_RET_SF_C {{0,"RETURN",0," "}, {0,"FROM ",1," "},{0,"ON ",2," "}};//RETURN select_expr_list_parens FROM table_reference ON search_condition
extern FmCB CASE_JOIN_RET_SERIALIZE_FORMAT     {&CASE_JOIN_RET_SF_C,&CASE_JOIN_RET_SF_C};
extern Form CASE_JOIN_WHEN_SF_C {{0,"WHEN ",0," "}, {0,"THEN ",1," "}};//WHEN search_condition THEN ret_join_on
extern FmCB CASE_JOIN_WHEN_SERIALIZE_FORMAT    {&CASE_JOIN_WHEN_SF_C,&CASE_JOIN_WHEN_SF_C};
extern Form CASE_JOIN_ELSE_SF_C {{0,"ELSE ",0," "}};//ELSE ret_join_on
extern FmCB CASE_JOIN_ELSE_SERIALIZE_FORMAT    {&CASE_JOIN_ELSE_SF_C,&CASE_JOIN_ELSE_SF_C};
extern Form CASE_JOIN_SF_C {{0,"CASE JOIN ",0," "}, {0,"",1,""},{1," END "}};//CASE JOIN  case_join_when_list opt_case_join_else  END
extern FmCB CASE_JOIN_SERIALIZE_FORMAT         {&CASE_JOIN_SF_C,&CASE_JOIN_SF_C};

extern Form POINT_VALUE_FORMAT_C {{0," ",0," "},{1,"=>"}, {0," ",1,""}};
extern FmCB POINT_VALUE_FORMAT  {&POINT_VALUE_FORMAT_C, &POINT_VALUE_FORMAT_C};

extern Form UNNEST_TABLE_FORMAT_C {{0,"UNNEST(",0,")"} ,{0," ",1,""}};
extern FmCB UNNEST_TABLE_FORMAT  {&UNNEST_TABLE_FORMAT_C, &UNNEST_TABLE_FORMAT_C};
extern Form COLLECT_VAL_FORMAT_C {{0,"ARRAY(",0,")"} };
extern FmCB COLLECT_VAL_FORMAT  {&COLLECT_VAL_FORMAT_C, &COLLECT_VAL_FORMAT_C};
extern Form AS_DERIVED_PART_FORMAT_C {{0,"",0," "},{0,"",1,""},{0,"(",2,")"} };
extern FmCB AS_DERIVED_PART_FORMAT  {&AS_DERIVED_PART_FORMAT_C, &AS_DERIVED_PART_FORMAT_C};

extern Form AGG_ORDER_SERIALIZE_FORMAT_C {{0,"ORDER BY ",0," "}};
extern FmCB AGG_ORDER_SERIALIZE_FORMAT {&AGG_ORDER_SERIALIZE_FORMAT_C,&AGG_ORDER_SERIALIZE_FORMAT_C};

extern Form REPLACE_SERIALIZE_FORMAT_C     {{0,"REPLACE ",0," "},{0,"",1," "},{0,"",2," "}, {0,"",3," "}, {0,"",4," "}};
extern Form REPLACE_SERIALIZE_FORMAT_B     {{0,"REPLACE ",0," "},{0,"",1,""},{0,"",2,"\n"}, {0,"",3," "}, {0,"",4," "}};
extern FmCB REPLACE_SERIALIZE_FORMAT       {&REPLACE_SERIALIZE_FORMAT_C,&REPLACE_SERIALIZE_FORMAT_B};
extern Form UPSERT_SERIALIZE_FORMAT_C     {{0,"UPSERT ",0," "},{0,"",1,""},{0,"",2," "}, {0,"",3," "}, {0,"",4," "}, {0,"",5," "}};
extern Form UPSERT_SERIALIZE_FORMAT_B     {{0,"UPSERT ",0," "},{0,"",1," "},{0,"",2,"\n"}, {0,"",3," "}, {0,"",4," "}, {0,"",5," "}};
extern FmCB UPSERT_SERIALIZE_FORMAT       {&UPSERT_SERIALIZE_FORMAT_C,&UPSERT_SERIALIZE_FORMAT_B};

// select ...from  ...  ? AS "t_00" ("C_0" NVARCHAR(3), "C_1" NVARCHAR(25), "C_2" NVARCHAR(2), "C_3" NVARCHAR(3))
extern Form CONSTRUCT_FROM_TABLE_C {{0," ",0," AS"},{0," ",1," "},{0,"(",2,")"}};
extern Form CONSTRUCT_FROM_TABLE_B {{0," ",0," AS"},{0," ",1," \n"},{0,"(",2,")"}};
extern FmCB CONSTRUCT_FROM_TABLE_SERIALIZE_FORMAT  {&CONSTRUCT_FROM_TABLE_C, &CONSTRUCT_FROM_TABLE_B};

extern Form CONSTRUCT_COL_C {{0," ",0," "},{0,"",1," "}};
extern Form CONSTRUCT_COL_B {{0," ",0," "},{0,"",1," "}};
extern FmCB CONSTRUCT_COL_SERIALIZE_FORMAT  {&CONSTRUCT_COL_C, &CONSTRUCT_COL_B};

extern Form HINT_CLAUSE_FORMAT_C            {{0,"WITH HINT(",0,")"}};
extern Form HINT_CLAUSE_FORMAT_B            {{0,"WITH HINT(",0,")"}};
extern FmCB HINT_CLAUSE_FORMAT              {&HINT_CLAUSE_FORMAT_C, &HINT_CLAUSE_FORMAT_B};

extern Form FOR_UPDATE_FORMAT_C            {{0," ",0," "},{0,"",1," "},{0,"OF ",2," "}, {0,"",3," "}};
extern Form FOR_UPDATE_FORMAT_B            {{0," ",0," "},{0,"",1," "},{0,"OF ",2," "}, {0,"",3," "}};
extern FmCB FOR_UPDATE_FORMAT              {&FOR_UPDATE_FORMAT_C, &FOR_UPDATE_FORMAT_B};

extern Form FOR_UPDATE_FORMAT_HANA_B            {{1," FOR UPDATE "},{0,"",0," "},{0,"",1," "},{0,"",2," "}};
extern FmCB FOR_UPDATE_FORMAT_HANA              {&FOR_UPDATE_FORMAT_HANA_B, &FOR_UPDATE_FORMAT_HANA_B};

extern Form WAIT_TIME_FORMAT_C            {{0,"WAIT ",0," "}};
extern Form WAIT_TIME_FORMAT_B            {{0,"WAIT ",0," "}};
extern FmCB WAIT_TIME_FORMAT              {&WAIT_TIME_FORMAT_C, &WAIT_TIME_FORMAT_B};

extern Form TOP_CLAUSE_FORMAT_C            {{0,"TOP ",0," "}};
extern Form TOP_CLAUSE_FORMAT_B            {{0,"TOP ",0," "}};
extern FmCB TOP_CLAUSE_FORMAT              {&TOP_CLAUSE_FORMAT_C, &TOP_CLAUSE_FORMAT_B};

extern Form LIMIT_NUM_FORMAT_HANA_B            {{0,"LIMIT ",0," "}, {0,"",1," "}, {0,"",2," "}};
extern FmCB LIMIT_NUM_FORMAT_HANA              {&LIMIT_NUM_FORMAT_HANA_B, &LIMIT_NUM_FORMAT_HANA_B};



extern Form SELECT_DIRECT_SERIALIZE_FORMAT_HANA_C         {{0,"",0," "}, {0,"",1," "}, {0,"",2," "}, {0,"",3," "}, {0,"",4," "}};
extern Form SELECT_DIRECT_SERIALIZE_FORMAT_HANA_B         {{0,"",0,"\n"}, {0,"",1," "}, {0,"\n",2," "}, {0,"\n",3," "}, {0,"\n",4," "}};
extern FmCB SELECT_DIRECT_SERIALIZE_FORMAT_HANA           {&SELECT_DIRECT_SERIALIZE_FORMAT_HANA_C, &SELECT_DIRECT_SERIALIZE_FORMAT_HANA_B};


extern Form SELECT_EXCEPT_SERIALIZE_FORMAT_HANA_C         {{0,"",8,""}, {0," ",6," "}, {0,"",7," "}, {0,"",9,""}};
extern Form SELECT_EXCEPT_SERIALIZE_FORMAT_HANA_B         {{0,"",8,""}, {0,"\n",6," "}, {0,"",7," "}, {0,"\n",9,""}};
extern FmCB SELECT_EXCEPT_SERIALIZE_FORMAT_HANA           {&SELECT_EXCEPT_SERIALIZE_FORMAT_HANA_C, &SELECT_EXCEPT_SERIALIZE_FORMAT_HANA_B};


extern Form OP_MEMBER_SERIALIZE_FORMAT_C             {{0,"",0," "}, {1," MEMBER OF "},{0,"",1," "}};
extern Form OP_NOT_MEMBER_SERIALIZE_FORMAT_C         {{0,"",0," "}, {1," NOT MEMBER OF "},{0,"",1," "}};
extern FmCB OP_MEMBER_SERIALIZE_FORMAT               {&OP_MEMBER_SERIALIZE_FORMAT_C,     &OP_MEMBER_SERIALIZE_FORMAT_C};
extern FmCB OP_NOT_MEMBER_SERIALIZE_FORMAT           {&OP_NOT_MEMBER_SERIALIZE_FORMAT_C, &OP_NOT_MEMBER_SERIALIZE_FORMAT_C};

extern Form OP_LIKE_REGEXPR_SERIALIZE_FORMAT_C         {{0,"",0," "}, {1," LIKE_REGEXPR "},{0,"",1," "},{0," FLAG ",2," "}};
extern FmCB OP_LIKE_REGEXPR_SERIALIZE_FORMAT           {&OP_LIKE_REGEXPR_SERIALIZE_FORMAT_C, &OP_LIKE_REGEXPR_SERIALIZE_FORMAT_C};

extern Form OP_AND_SPC_SERIALIZE_FORMAT_C         {{0,"",0," "}, {1," & "},{0,"",1," "}};
extern FmCB OP_AND_SPC_SERIALIZE_FORMAT           {&OP_AND_SPC_SERIALIZE_FORMAT_C, &OP_AND_SPC_SERIALIZE_FORMAT_C};

extern Form FUN_CALL_314_SERIALIZE_FORMAT_C     {{0,"",0,""}, {1,"("}, {0,"",3," "}, {0,"",1,""}, {0," ",4,""}, {1,")"}, {0,"",2," "}};
/* FUN_CALL_DIS_OVER_SERIALIZE_FORMAT_B */
extern FmCB FUN_CALL_314_SERIALIZE_FORMAT       {&FUN_CALL_314_SERIALIZE_FORMAT_C, &FUN_CALL_314_SERIALIZE_FORMAT_C};

//extern Form OP_NAME_FIELD_SERIALIZE_FORMAT_4_C       {{0,"",4,""}, {1,"."}, {0,"",3,""}, {1,"."}, {0,"",2,""}, {1,"."}, {0,"",1,""}, {1,"."}, {0,"",0,""}};
//extern FmCB OP_NAME_FIELD_SERIALIZE_FORMAT_4         {&OP_NAME_FIELD_SERIALIZE_FORMAT_4_C, &OP_NAME_FIELD_SERIALIZE_FORMAT_4_C};


extern Form DELETE_S_FORMAT_C2                {{0,"",0," "}, {1, "DELETE HISTORY "}, {0,"",1," "}, {0,"FROM ",2," "}, {0,"",3," "}
        , {0,"",4," "}, {0,"",5," "}, {0,"",6," "}, {0,"",7," "}};
extern Form DELETE_S_FORMAT_B2                {{0,"",0," "}, {1, "DELETE HISTORY "}, {0,"",1," "}, {0,"FROM ",2," "}, {0,"",3," "}
        , {0,"",4," "}, {0,"",5," "}, {0,"\n",6," "}, {0,"",7," "}};
extern FmCB DELETE_S_FORMAT                  {&DELETE_S_FORMAT_C2, &DELETE_S_FORMAT_B2};

extern Form SET_S_FORMAT_C     {{0,"SET ",0," "},{0,"",1,""}};
extern FmCB SET_S_FORMAT_HANA  {&SET_S_FORMAT_C,&SET_S_FORMAT_C};
