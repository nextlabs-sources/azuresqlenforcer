/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 2

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Substitute the type names.  */
#define YYSTYPE         SQL2003_STYPE
#define YYLTYPE         SQL2003_LTYPE
/* Substitute the variable and function names.  */
#define yyparse         sql2003_parse
#define yylex           sql2003_lex
#define yyerror         sql2003_error
#define yydebug         sql2003_debug
#define yynerrs         sql2003_nerrs

/* First part of user prologue.  */
#line 1 "sqlparser_sql2003.y"

/**
 * This Grammar is designed for sql2003.
 * https://github.com/Raphael2017/SQL/blob/master/sql-2003-2.bnf
 * sqlparser.y
 * defines sqlparser_sql2003_bison.h
 * outputs sqlparser_sql2003_bison.cpp
 *
 * Bison Grammar File Spec: http://dinosaur.compilertools.net/bison/bison_6.html
 *
 */
/*********************************
 ** Section 1: C Declarations
 *********************************/

#include "sqlparser_sql2003_bison.h"
#include "sqlparser_sql2003_flex.h"
#include "serialize_format.h"

#include <stdio.h>
#include <string.h>
//#include <strings.h>

/*
 * We provide parse error includes error message, first line, first column of error lex for debug
 */
int yyerror(YYLTYPE* llocp, ParseResult* result, yyscan_t scanner, const char *msg) {
    result->accept = false;
    result->errFirstLine = llocp->first_line;
    result->errFirstColumn = llocp->first_column;
    result->errDetail = msg;
	return 0;
}


#line 114 "sqlparser_sql2003_bison.cpp"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "sqlparser_sql2003_bison.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_NAME = 3,                       /* NAME  */
  YYSYMBOL_STRING = 4,                     /* STRING  */
  YYSYMBOL_INTNUM = 5,                     /* INTNUM  */
  YYSYMBOL_BOOL = 6,                       /* BOOL  */
  YYSYMBOL_APPROXNUM = 7,                  /* APPROXNUM  */
  YYSYMBOL_NULLX = 8,                      /* NULLX  */
  YYSYMBOL_UNKNOWN = 9,                    /* UNKNOWN  */
  YYSYMBOL_QUESTIONMARK = 10,              /* QUESTIONMARK  */
  YYSYMBOL_PARAM = 11,                     /* PARAM  */
  YYSYMBOL_CREATE_TABLE = 12,              /* CREATE_TABLE  */
  YYSYMBOL_CREATE_TABLE_REST = 13,         /* CREATE_TABLE_REST  */
  YYSYMBOL_UMINUS = 14,                    /* UMINUS  */
  YYSYMBOL_15_ = 15,                       /* '('  */
  YYSYMBOL_16_ = 16,                       /* ')'  */
  YYSYMBOL_17_ = 17,                       /* '.'  */
  YYSYMBOL_ALL = 18,                       /* ALL  */
  YYSYMBOL_AND = 19,                       /* AND  */
  YYSYMBOL_ANY = 20,                       /* ANY  */
  YYSYMBOL_ARRAY = 21,                     /* ARRAY  */
  YYSYMBOL_AS = 22,                        /* AS  */
  YYSYMBOL_ASC = 23,                       /* ASC  */
  YYSYMBOL_AVG = 24,                       /* AVG  */
  YYSYMBOL_BETWEEN = 25,                   /* BETWEEN  */
  YYSYMBOL_BIGINT = 26,                    /* BIGINT  */
  YYSYMBOL_BINARY = 27,                    /* BINARY  */
  YYSYMBOL_BLOB = 28,                      /* BLOB  */
  YYSYMBOL_BOOLEAN = 29,                   /* BOOLEAN  */
  YYSYMBOL_BY = 30,                        /* BY  */
  YYSYMBOL_CALL = 31,                      /* CALL  */
  YYSYMBOL_CASE = 32,                      /* CASE  */
  YYSYMBOL_CAST = 33,                      /* CAST  */
  YYSYMBOL_CHAR = 34,                      /* CHAR  */
  YYSYMBOL_CHARACTER = 35,                 /* CHARACTER  */
  YYSYMBOL_CHARACTERS = 36,                /* CHARACTERS  */
  YYSYMBOL_CLOB = 37,                      /* CLOB  */
  YYSYMBOL_CNNOP = 38,                     /* CNNOP  */
  YYSYMBOL_COALESCE = 39,                  /* COALESCE  */
  YYSYMBOL_CODE_UNITS = 40,                /* CODE_UNITS  */
  YYSYMBOL_COLLATE = 41,                   /* COLLATE  */
  YYSYMBOL_COMP_EQ = 42,                   /* COMP_EQ  */
  YYSYMBOL_COMP_GE = 43,                   /* COMP_GE  */
  YYSYMBOL_COMP_GT = 44,                   /* COMP_GT  */
  YYSYMBOL_COMP_LE = 45,                   /* COMP_LE  */
  YYSYMBOL_COMP_LT = 46,                   /* COMP_LT  */
  YYSYMBOL_COMP_NE = 47,                   /* COMP_NE  */
  YYSYMBOL_CONVERT = 48,                   /* CONVERT  */
  YYSYMBOL_CORRESPONDING = 49,             /* CORRESPONDING  */
  YYSYMBOL_COUNT = 50,                     /* COUNT  */
  YYSYMBOL_CREATE = 51,                    /* CREATE  */
  YYSYMBOL_CROSS = 52,                     /* CROSS  */
  YYSYMBOL_CUME_DIST = 53,                 /* CUME_DIST  */
  YYSYMBOL_CURRENT = 54,                   /* CURRENT  */
  YYSYMBOL_CURRENT_TIMESTAMP = 55,         /* CURRENT_TIMESTAMP  */
  YYSYMBOL_CURRENT_USER = 56,              /* CURRENT_USER  */
  YYSYMBOL_DATE = 57,                      /* DATE  */
  YYSYMBOL_DAY = 58,                       /* DAY  */
  YYSYMBOL_DEC = 59,                       /* DEC  */
  YYSYMBOL_DECIMAL = 60,                   /* DECIMAL  */
  YYSYMBOL_DEFAULT = 61,                   /* DEFAULT  */
  YYSYMBOL_DELETE = 62,                    /* DELETE  */
  YYSYMBOL_DENSE_RANK = 63,                /* DENSE_RANK  */
  YYSYMBOL_DESC = 64,                      /* DESC  */
  YYSYMBOL_DISTINCT = 65,                  /* DISTINCT  */
  YYSYMBOL_DOUBLE = 66,                    /* DOUBLE  */
  YYSYMBOL_DROP = 67,                      /* DROP  */
  YYSYMBOL_ELSE = 68,                      /* ELSE  */
  YYSYMBOL_END = 69,                       /* END  */
  YYSYMBOL_END_P = 70,                     /* END_P  */
  YYSYMBOL_ESCAPE = 71,                    /* ESCAPE  */
  YYSYMBOL_ERROR = 72,                     /* ERROR  */
  YYSYMBOL_EXCEPT = 73,                    /* EXCEPT  */
  YYSYMBOL_EXCLUDE = 74,                   /* EXCLUDE  */
  YYSYMBOL_EXEC = 75,                      /* EXEC  */
  YYSYMBOL_EXECUTE = 76,                   /* EXECUTE  */
  YYSYMBOL_EXISTS = 77,                    /* EXISTS  */
  YYSYMBOL_FILE_TABLE = 78,                /* FILE_TABLE  */
  YYSYMBOL_FLOAT = 79,                     /* FLOAT  */
  YYSYMBOL_FOLLOWING = 80,                 /* FOLLOWING  */
  YYSYMBOL_FOR = 81,                       /* FOR  */
  YYSYMBOL_FROM = 82,                      /* FROM  */
  YYSYMBOL_FULL = 83,                      /* FULL  */
  YYSYMBOL_G = 84,                         /* G  */
  YYSYMBOL_GROUP = 85,                     /* GROUP  */
  YYSYMBOL_GROUPING = 86,                  /* GROUPING  */
  YYSYMBOL_HAVING = 87,                    /* HAVING  */
  YYSYMBOL_HOUR = 88,                      /* HOUR  */
  YYSYMBOL_IF = 89,                        /* IF  */
  YYSYMBOL_IN = 90,                        /* IN  */
  YYSYMBOL_INNER = 91,                     /* INNER  */
  YYSYMBOL_INSERT = 92,                    /* INSERT  */
  YYSYMBOL_INT = 93,                       /* INT  */
  YYSYMBOL_INTEGER = 94,                   /* INTEGER  */
  YYSYMBOL_INTERSECT = 95,                 /* INTERSECT  */
  YYSYMBOL_INTERVAL = 96,                  /* INTERVAL  */
  YYSYMBOL_INTO = 97,                      /* INTO  */
  YYSYMBOL_IS = 98,                        /* IS  */
  YYSYMBOL_JOIN = 99,                      /* JOIN  */
  YYSYMBOL_K = 100,                        /* K  */
  YYSYMBOL_LARGE = 101,                    /* LARGE  */
  YYSYMBOL_LEFT = 102,                     /* LEFT  */
  YYSYMBOL_LIKE = 103,                     /* LIKE  */
  YYSYMBOL_M = 104,                        /* M  */
  YYSYMBOL_MAX = 105,                      /* MAX  */
  YYSYMBOL_MIN = 106,                      /* MIN  */
  YYSYMBOL_MINUTE = 107,                   /* MINUTE  */
  YYSYMBOL_MOD = 108,                      /* MOD  */
  YYSYMBOL_MONTH = 109,                    /* MONTH  */
  YYSYMBOL_MULTISET = 110,                 /* MULTISET  */
  YYSYMBOL_NATIONAL = 111,                 /* NATIONAL  */
  YYSYMBOL_NATURAL = 112,                  /* NATURAL  */
  YYSYMBOL_NCHAR = 113,                    /* NCHAR  */
  YYSYMBOL_NCLOB = 114,                    /* NCLOB  */
  YYSYMBOL_NO = 115,                       /* NO  */
  YYSYMBOL_NOT = 116,                      /* NOT  */
  YYSYMBOL_NULLIF = 117,                   /* NULLIF  */
  YYSYMBOL_NUMERIC = 118,                  /* NUMERIC  */
  YYSYMBOL_OBJECT = 119,                   /* OBJECT  */
  YYSYMBOL_OCTETS = 120,                   /* OCTETS  */
  YYSYMBOL_OF = 121,                       /* OF  */
  YYSYMBOL_ON = 122,                       /* ON  */
  YYSYMBOL_ONLY = 123,                     /* ONLY  */
  YYSYMBOL_OR = 124,                       /* OR  */
  YYSYMBOL_ORDER = 125,                    /* ORDER  */
  YYSYMBOL_OTHERS = 126,                   /* OTHERS  */
  YYSYMBOL_OUTER = 127,                    /* OUTER  */
  YYSYMBOL_OVER = 128,                     /* OVER  */
  YYSYMBOL_PARTITION = 129,                /* PARTITION  */
  YYSYMBOL_PERCENT_RANK = 130,             /* PERCENT_RANK  */
  YYSYMBOL_PRECEDING = 131,                /* PRECEDING  */
  YYSYMBOL_PRECISION = 132,                /* PRECISION  */
  YYSYMBOL_RANGE = 133,                    /* RANGE  */
  YYSYMBOL_RANK = 134,                     /* RANK  */
  YYSYMBOL_READ = 135,                     /* READ  */
  YYSYMBOL_REAL = 136,                     /* REAL  */
  YYSYMBOL_RECURSIVE = 137,                /* RECURSIVE  */
  YYSYMBOL_REF = 138,                      /* REF  */
  YYSYMBOL_RIGHT = 139,                    /* RIGHT  */
  YYSYMBOL_ROW = 140,                      /* ROW  */
  YYSYMBOL_ROWS = 141,                     /* ROWS  */
  YYSYMBOL_ROW_NUMBER = 142,               /* ROW_NUMBER  */
  YYSYMBOL_SCOPE = 143,                    /* SCOPE  */
  YYSYMBOL_SECOND = 144,                   /* SECOND  */
  YYSYMBOL_SELECT = 145,                   /* SELECT  */
  YYSYMBOL_SESSION_USER = 146,             /* SESSION_USER  */
  YYSYMBOL_SET = 147,                      /* SET  */
  YYSYMBOL_SETS = 148,                     /* SETS  */
  YYSYMBOL_SMALLINT = 149,                 /* SMALLINT  */
  YYSYMBOL_SOME = 150,                     /* SOME  */
  YYSYMBOL_SP_EXECUTESQL = 151,            /* SP_EXECUTESQL  */
  YYSYMBOL_STDDEV_POP = 152,               /* STDDEV_POP  */
  YYSYMBOL_STDDEV_SAMP = 153,              /* STDDEV_SAMP  */
  YYSYMBOL_SUM = 154,                      /* SUM  */
  YYSYMBOL_SYSTEM_USER = 155,              /* SYSTEM_USER  */
  YYSYMBOL_TABLE = 156,                    /* TABLE  */
  YYSYMBOL_THEN = 157,                     /* THEN  */
  YYSYMBOL_TIES = 158,                     /* TIES  */
  YYSYMBOL_TIME = 159,                     /* TIME  */
  YYSYMBOL_TIMESTAMP = 160,                /* TIMESTAMP  */
  YYSYMBOL_TO = 161,                       /* TO  */
  YYSYMBOL_UNBOUNDED = 162,                /* UNBOUNDED  */
  YYSYMBOL_UNION = 163,                    /* UNION  */
  YYSYMBOL_UPDATE = 164,                   /* UPDATE  */
  YYSYMBOL_USING = 165,                    /* USING  */
  YYSYMBOL_VALUES = 166,                   /* VALUES  */
  YYSYMBOL_VARCHAR = 167,                  /* VARCHAR  */
  YYSYMBOL_VARYING = 168,                  /* VARYING  */
  YYSYMBOL_VAR_POP = 169,                  /* VAR_POP  */
  YYSYMBOL_VAR_SAMP = 170,                 /* VAR_SAMP  */
  YYSYMBOL_WHEN = 171,                     /* WHEN  */
  YYSYMBOL_WHERE = 172,                    /* WHERE  */
  YYSYMBOL_WITH = 173,                     /* WITH  */
  YYSYMBOL_WITHOUT = 174,                  /* WITHOUT  */
  YYSYMBOL_YEAR = 175,                     /* YEAR  */
  YYSYMBOL_ZONE = 176,                     /* ZONE  */
  YYSYMBOL_TOP = 177,                      /* TOP  */
  YYSYMBOL_NVARCHAR = 178,                 /* NVARCHAR  */
  YYSYMBOL_USE = 179,                      /* USE  */
  YYSYMBOL_TRIM = 180,                     /* TRIM  */
  YYSYMBOL_UNPIVOT = 181,                  /* UNPIVOT  */
  YYSYMBOL_PIVOT = 182,                    /* PIVOT  */
  YYSYMBOL_DECLARE = 183,                  /* DECLARE  */
  YYSYMBOL_184_ = 184,                     /* ';'  */
  YYSYMBOL_185_ = 185,                     /* '{'  */
  YYSYMBOL_186_ = 186,                     /* '}'  */
  YYSYMBOL_187_ = 187,                     /* ','  */
  YYSYMBOL_188_ = 188,                     /* '*'  */
  YYSYMBOL_189_ = 189,                     /* '^'  */
  YYSYMBOL_190_ = 190,                     /* '+'  */
  YYSYMBOL_191_ = 191,                     /* '-'  */
  YYSYMBOL_192_ = 192,                     /* '!'  */
  YYSYMBOL_193_ = 193,                     /* '|'  */
  YYSYMBOL_194_ = 194,                     /* '/'  */
  YYSYMBOL_195_ = 195,                     /* '%'  */
  YYSYMBOL_196_ = 196,                     /* '&'  */
  YYSYMBOL_YYACCEPT = 197,                 /* $accept  */
  YYSYMBOL_sql_stmt = 198,                 /* sql_stmt  */
  YYSYMBOL_stmt_list = 199,                /* stmt_list  */
  YYSYMBOL_stmt = 200,                     /* stmt  */
  YYSYMBOL_special_stmt = 201,             /* special_stmt  */
  YYSYMBOL_sp_executesql_stmt = 202,       /* sp_executesql_stmt  */
  YYSYMBOL_call_stmt = 203,                /* call_stmt  */
  YYSYMBOL_sql_argument_list = 204,        /* sql_argument_list  */
  YYSYMBOL_sql_argument = 205,             /* sql_argument  */
  YYSYMBOL_value_expression = 206,         /* value_expression  */
  YYSYMBOL_sp_name = 207,                  /* sp_name  */
  YYSYMBOL_ddl_stmt = 208,                 /* ddl_stmt  */
  YYSYMBOL_dql_stmt = 209,                 /* dql_stmt  */
  YYSYMBOL_dml_stmt = 210,                 /* dml_stmt  */
  YYSYMBOL_create_stmt = 211,              /* create_stmt  */
  YYSYMBOL_create_table_stmt = 212,        /* create_table_stmt  */
  YYSYMBOL_insert_stmt = 213,              /* insert_stmt  */
  YYSYMBOL_insert_columns_and_source = 214, /* insert_columns_and_source  */
  YYSYMBOL_from_constructor = 215,         /* from_constructor  */
  YYSYMBOL_delete_stmt = 216,              /* delete_stmt  */
  YYSYMBOL_drop_stmt = 217,                /* drop_stmt  */
  YYSYMBOL_drop_table_stmt = 218,          /* drop_table_stmt  */
  YYSYMBOL_update_stmt = 219,              /* update_stmt  */
  YYSYMBOL_update_elem_list = 220,         /* update_elem_list  */
  YYSYMBOL_update_elem = 221,              /* update_elem  */
  YYSYMBOL_select_stmt = 222,              /* select_stmt  */
  YYSYMBOL_query_expression = 223,         /* query_expression  */
  YYSYMBOL_query_expression_body = 224,    /* query_expression_body  */
  YYSYMBOL_query_term = 225,               /* query_term  */
  YYSYMBOL_query_primary = 226,            /* query_primary  */
  YYSYMBOL_select_with_parens = 227,       /* select_with_parens  */
  YYSYMBOL_subquery = 228,                 /* subquery  */
  YYSYMBOL_table_subquery = 229,           /* table_subquery  */
  YYSYMBOL_row_subquery = 230,             /* row_subquery  */
  YYSYMBOL_simple_table = 231,             /* simple_table  */
  YYSYMBOL_opt_where = 232,                /* opt_where  */
  YYSYMBOL_opt_from_clause = 233,          /* opt_from_clause  */
  YYSYMBOL_opt_groupby = 234,              /* opt_groupby  */
  YYSYMBOL_grouping_element_list = 235,    /* grouping_element_list  */
  YYSYMBOL_grouping_element = 236,         /* grouping_element  */
  YYSYMBOL_opt_order_by = 237,             /* opt_order_by  */
  YYSYMBOL_order_by = 238,                 /* order_by  */
  YYSYMBOL_sort_list = 239,                /* sort_list  */
  YYSYMBOL_sort_key = 240,                 /* sort_key  */
  YYSYMBOL_opt_asc_desc = 241,             /* opt_asc_desc  */
  YYSYMBOL_opt_having = 242,               /* opt_having  */
  YYSYMBOL_with_clause = 243,              /* with_clause  */
  YYSYMBOL_with_list = 244,                /* with_list  */
  YYSYMBOL_common_table_expr = 245,        /* common_table_expr  */
  YYSYMBOL_opt_derived_column_list = 246,  /* opt_derived_column_list  */
  YYSYMBOL_simple_ident_list_with_parens = 247, /* simple_ident_list_with_parens  */
  YYSYMBOL_simple_ident_list = 248,        /* simple_ident_list  */
  YYSYMBOL_opt_distinct = 249,             /* opt_distinct  */
  YYSYMBOL_select_expr_list = 250,         /* select_expr_list  */
  YYSYMBOL_projection = 251,               /* projection  */
  YYSYMBOL_asterisk_expr = 252,            /* asterisk_expr  */
  YYSYMBOL_from_list = 253,                /* from_list  */
  YYSYMBOL_table_reference = 254,          /* table_reference  */
  YYSYMBOL_table_primary = 255,            /* table_primary  */
  YYSYMBOL_table_primary_non_join = 256,   /* table_primary_non_join  */
  YYSYMBOL_unpivot_clause = 257,           /* unpivot_clause  */
  YYSYMBOL_pivot_clause = 258,             /* pivot_clause  */
  YYSYMBOL_opt_simple_ident_list_with_parens = 259, /* opt_simple_ident_list_with_parens  */
  YYSYMBOL_column_ref = 260,               /* column_ref  */
  YYSYMBOL_relation_factor_list = 261,     /* relation_factor_list  */
  YYSYMBOL_relation_factor = 262,          /* relation_factor  */
  YYSYMBOL_joined_table = 263,             /* joined_table  */
  YYSYMBOL_qualified_join = 264,           /* qualified_join  */
  YYSYMBOL_cross_join = 265,               /* cross_join  */
  YYSYMBOL_join_type = 266,                /* join_type  */
  YYSYMBOL_join_outer = 267,               /* join_outer  */
  YYSYMBOL_search_condition = 268,         /* search_condition  */
  YYSYMBOL_boolean_term = 269,             /* boolean_term  */
  YYSYMBOL_boolean_factor = 270,           /* boolean_factor  */
  YYSYMBOL_boolean_test = 271,             /* boolean_test  */
  YYSYMBOL_boolean_primary = 272,          /* boolean_primary  */
  YYSYMBOL_predicate = 273,                /* predicate  */
  YYSYMBOL_comparison_predicate = 274,     /* comparison_predicate  */
  YYSYMBOL_quantified_comparison_predicate = 275, /* quantified_comparison_predicate  */
  YYSYMBOL_between_predicate = 276,        /* between_predicate  */
  YYSYMBOL_like_predicate = 277,           /* like_predicate  */
  YYSYMBOL_in_predicate = 278,             /* in_predicate  */
  YYSYMBOL_null_predicate = 279,           /* null_predicate  */
  YYSYMBOL_exists_predicate = 280,         /* exists_predicate  */
  YYSYMBOL_row_expr = 281,                 /* row_expr  */
  YYSYMBOL_factor0 = 282,                  /* factor0  */
  YYSYMBOL_factor1 = 283,                  /* factor1  */
  YYSYMBOL_factor2 = 284,                  /* factor2  */
  YYSYMBOL_factor3 = 285,                  /* factor3  */
  YYSYMBOL_factor4 = 286,                  /* factor4  */
  YYSYMBOL_row_expr_list = 287,            /* row_expr_list  */
  YYSYMBOL_in_expr = 288,                  /* in_expr  */
  YYSYMBOL_truth_value = 289,              /* truth_value  */
  YYSYMBOL_comp_op = 290,                  /* comp_op  */
  YYSYMBOL_cnn_op = 291,                   /* cnn_op  */
  YYSYMBOL_comp_all_some_any_op = 292,     /* comp_all_some_any_op  */
  YYSYMBOL_plus_minus_op = 293,            /* plus_minus_op  */
  YYSYMBOL_star_div_percent_mod_op = 294,  /* star_div_percent_mod_op  */
  YYSYMBOL_expr_const = 295,               /* expr_const  */
  YYSYMBOL_case_expr = 296,                /* case_expr  */
  YYSYMBOL_case_arg = 297,                 /* case_arg  */
  YYSYMBOL_when_clause_list = 298,         /* when_clause_list  */
  YYSYMBOL_when_clause = 299,              /* when_clause  */
  YYSYMBOL_case_default = 300,             /* case_default  */
  YYSYMBOL_func_expr = 301,                /* func_expr  */
  YYSYMBOL_aggregate_windowed_function = 302, /* aggregate_windowed_function  */
  YYSYMBOL_aggregate_function_name = 303,  /* aggregate_function_name  */
  YYSYMBOL_ranking_windowed_function = 304, /* ranking_windowed_function  */
  YYSYMBOL_ranking_function_name = 305,    /* ranking_function_name  */
  YYSYMBOL_over_clause = 306,              /* over_clause  */
  YYSYMBOL_window_specification = 307,     /* window_specification  */
  YYSYMBOL_window_name = 308,              /* window_name  */
  YYSYMBOL_window_specification_details = 309, /* window_specification_details  */
  YYSYMBOL_opt_existing_window_name = 310, /* opt_existing_window_name  */
  YYSYMBOL_opt_window_partition_clause = 311, /* opt_window_partition_clause  */
  YYSYMBOL_opt_window_frame_clause = 312,  /* opt_window_frame_clause  */
  YYSYMBOL_window_frame_units = 313,       /* window_frame_units  */
  YYSYMBOL_window_frame_extent = 314,      /* window_frame_extent  */
  YYSYMBOL_window_frame_start = 315,       /* window_frame_start  */
  YYSYMBOL_window_frame_preceding = 316,   /* window_frame_preceding  */
  YYSYMBOL_window_frame_between = 317,     /* window_frame_between  */
  YYSYMBOL_window_frame_bound = 318,       /* window_frame_bound  */
  YYSYMBOL_window_frame_following = 319,   /* window_frame_following  */
  YYSYMBOL_opt_window_frame_exclusion = 320, /* opt_window_frame_exclusion  */
  YYSYMBOL_scalar_function = 321,          /* scalar_function  */
  YYSYMBOL_function_call_keyword = 322,    /* function_call_keyword  */
  YYSYMBOL_data_type = 323,                /* data_type  */
  YYSYMBOL_user_defined_type_name = 324,   /* user_defined_type_name  */
  YYSYMBOL_relation_factor_type = 325,     /* relation_factor_type  */
  YYSYMBOL_relation_factor_func = 326,     /* relation_factor_func  */
  YYSYMBOL_reference_type = 327,           /* reference_type  */
  YYSYMBOL_collection_type = 328,          /* collection_type  */
  YYSYMBOL_predefined_type = 329,          /* predefined_type  */
  YYSYMBOL_interval_type = 330,            /* interval_type  */
  YYSYMBOL_interval_qualifier = 331,       /* interval_qualifier  */
  YYSYMBOL_start_field = 332,              /* start_field  */
  YYSYMBOL_end_field = 333,                /* end_field  */
  YYSYMBOL_single_datetime_field = 334,    /* single_datetime_field  */
  YYSYMBOL_non_second_primary_datetime_field = 335, /* non_second_primary_datetime_field  */
  YYSYMBOL_boolean_type = 336,             /* boolean_type  */
  YYSYMBOL_datetime_type = 337,            /* datetime_type  */
  YYSYMBOL_numeric_type = 338,             /* numeric_type  */
  YYSYMBOL_exact_numeric_type = 339,       /* exact_numeric_type  */
  YYSYMBOL_approximate_numeric_type = 340, /* approximate_numeric_type  */
  YYSYMBOL_character_string_type = 341,    /* character_string_type  */
  YYSYMBOL_data_type_len = 342,            /* data_type_len  */
  YYSYMBOL_binary_large_object_string_type = 343, /* binary_large_object_string_type  */
  YYSYMBOL_national_character_string_type = 344, /* national_character_string_type  */
  YYSYMBOL_large_object_length = 345,      /* large_object_length  */
  YYSYMBOL_char_length_units = 346,        /* char_length_units  */
  YYSYMBOL_multiplier = 347,               /* multiplier  */
  YYSYMBOL_distinct_or_all = 348,          /* distinct_or_all  */
  YYSYMBOL_all_some_any = 349,             /* all_some_any  */
  YYSYMBOL_opt_as_label = 350,             /* opt_as_label  */
  YYSYMBOL_as_label = 351,                 /* as_label  */
  YYSYMBOL_label = 352,                    /* label  */
  YYSYMBOL_collate_clause = 353,           /* collate_clause  */
  YYSYMBOL_name_r = 354,                   /* name_r  */
  YYSYMBOL_name_type = 355,                /* name_type  */
  YYSYMBOL_name_f = 356,                   /* name_f  */
  YYSYMBOL_reserved_type = 357,            /* reserved_type  */
  YYSYMBOL_reserved_func = 358,            /* reserved_func  */
  YYSYMBOL_reserved_other = 359,           /* reserved_other  */
  YYSYMBOL_reserved = 360,                 /* reserved  */
  YYSYMBOL_opt_if_exists_clause = 361,     /* opt_if_exists_clause  */
  YYSYMBOL_opt_top_clause = 362,           /* opt_top_clause  */
  YYSYMBOL_intnum_parens = 363             /* intnum_parens  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int16 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if 1

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* 1 */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined SQL2003_LTYPE_IS_TRIVIAL && SQL2003_LTYPE_IS_TRIVIAL \
             && defined SQL2003_STYPE_IS_TRIVIAL && SQL2003_STYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE) \
             + YYSIZEOF (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  149
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   7141

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  197
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  167
/* YYNRULES -- Number of rules.  */
#define YYNRULES  529
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  901

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   435


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   192,     2,     2,     2,   195,   196,     2,
      15,    16,   188,   190,   187,   191,    17,   194,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,   184,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,   189,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   185,   193,   186,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183
};

#if SQL2003_DEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   240,   240,   250,   251,   259,   260,   261,   262,   263,
     264,   268,   273,   284,   295,   299,   303,   310,   318,   323,
     328,   333,   341,   342,   350,   355,   363,   364,   368,   372,
     373,   377,   381,   382,   383,   387,   391,   399,   407,   412,
     417,   422,   427,   436,   445,   461,   465,   476,   493,   494,
     502,   511,   519,   524,   531,   532,   559,   589,   590,   620,
     621,   636,   643,   645,   647,   650,   679,   680,   688,   689,
     697,   698,   706,   707,   715,   716,   717,   727,   728,   732,
     740,   741,   749,   758,   761,   765,   772,   773,   783,   788,
     796,   797,   806,   814,   815,   819,   827,   828,   836,   837,
     841,   848,   849,   857,   871,   875,   879,   885,   891,   900,
     901,   909,   910,   914,   915,   927,   932,   937,   990,   995,
    1048,  1056,  1063,  1071,  1072,  1077,  1083,  1089,  1095,  1101,
    1164,  1165,  1174,  1179,  1184,  1189,  1194,  1199,  1204,  1209,
    1218,  1219,  1224,  1229,  1250,  1275,  1286,  1297,  1308,  1315,
    1316,  1321,  1322,  1330,  1331,  1339,  1340,  1348,  1349,  1354,
    1362,  1363,  1381,  1382,  1383,  1384,  1385,  1386,  1387,  1391,
    1399,  1407,  1412,  1420,  1425,  1430,  1435,  1443,  1448,  1456,
    1461,  1469,  1478,  1479,  1484,  1492,  1493,  1501,  1502,  1510,
    1511,  1519,  1520,  1525,  1533,  1534,  1535,  1536,  1541,  1542,
    1546,  1547,  1555,  1556,  1563,  1563,  1567,  1568,  1569,  1570,
    1571,  1572,  1573,  1574,  1575,  1576,  1580,  1581,  1586,  1593,
    1594,  1598,  1599,  1600,  1601,  1602,  1606,  1607,  1608,  1609,
    1610,  1611,  1612,  1613,  1618,  1626,  1627,  1631,  1632,  1640,
    1645,  1653,  1654,  1663,  1664,  1665,  1666,  1671,  1677,  1683,
    1689,  1695,  1707,  1722,  1723,  1724,  1725,  1726,  1727,  1728,
    1729,  1730,  1731,  1735,  1744,  1745,  1746,  1747,  1748,  1752,
    1756,  1765,  1770,  1774,  1782,  1783,  1787,  1788,  1793,  1794,
    1803,  1804,  1808,  1809,  1813,  1814,  1815,  1819,  1823,  1828,
    1829,  1830,  1834,  1838,  1839,  1840,  1841,  1842,  1846,  1852,
    1864,  1870,  1877,  1884,  1891,  1902,  1911,  1918,  1925,  1932,
    1939,  1948,  1955,  1962,  1970,  1984,  1985,  1986,  1987,  1991,
    1998,  2003,  2008,  2013,  2021,  2026,  2031,  2036,  2044,  2052,
    2058,  2063,  2071,  2076,  2081,  2086,  2087,  2092,  2093,  2094,
    2095,  2096,  2097,  2101,  2109,  2114,  2118,  2124,  2128,  2129,
    2134,  2141,  2146,  2147,  2152,  2157,  2164,  2168,  2172,  2176,
    2180,  2187,  2194,  2198,  2203,  2208,  2213,  2217,  2221,  2226,
    2231,  2236,  2240,  2247,  2248,  2252,  2257,  2262,  2267,  2272,
    2277,  2282,  2286,  2290,  2294,  2298,  2302,  2306,  2313,  2318,
    2322,  2326,  2333,  2338,  2343,  2348,  2353,  2358,  2362,  2366,
    2371,  2376,  2380,  2385,  2389,  2394,  2398,  2402,  2409,  2410,
    2414,  2419,  2423,  2428,  2435,  2440,  2444,  2449,  2453,  2458,
    2463,  2468,  2473,  2478,  2482,  2487,  2491,  2496,  2500,  2507,
    2512,  2517,  2522,  2530,  2534,  2538,  2545,  2549,  2553,  2560,
    2564,  2571,  2572,  2573,  2577,  2578,  2582,  2583,  2587,  2588,
    2593,  2603,  2604,  2607,  2608,  2609,  2610,  2613,  2614,  2615,
    2618,  2619,  2620,  2621,  2622,  2623,  2624,  2625,  2626,  2627,
    2628,  2629,  2630,  2631,  2632,  2633,  2634,  2635,  2636,  2640,
    2641,  2642,  2643,  2644,  2647,  2648,  2649,  2650,  2651,  2652,
    2653,  2654,  2655,  2656,  2657,  2658,  2659,  2660,  2661,  2662,
    2663,  2664,  2665,  2666,  2667,  2668,  2669,  2670,  2671,  2672,
    2673,  2674,  2675,  2676,  2677,  2678,  2679,  2680,  2681,  2685,
    2686,  2687,  2688,  2692,  2693,  2696,  2697,  2704,  2705,  2706
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if 1
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "NAME", "STRING",
  "INTNUM", "BOOL", "APPROXNUM", "NULLX", "UNKNOWN", "QUESTIONMARK",
  "PARAM", "CREATE_TABLE", "CREATE_TABLE_REST", "UMINUS", "'('", "')'",
  "'.'", "ALL", "AND", "ANY", "ARRAY", "AS", "ASC", "AVG", "BETWEEN",
  "BIGINT", "BINARY", "BLOB", "BOOLEAN", "BY", "CALL", "CASE", "CAST",
  "CHAR", "CHARACTER", "CHARACTERS", "CLOB", "CNNOP", "COALESCE",
  "CODE_UNITS", "COLLATE", "COMP_EQ", "COMP_GE", "COMP_GT", "COMP_LE",
  "COMP_LT", "COMP_NE", "CONVERT", "CORRESPONDING", "COUNT", "CREATE",
  "CROSS", "CUME_DIST", "CURRENT", "CURRENT_TIMESTAMP", "CURRENT_USER",
  "DATE", "DAY", "DEC", "DECIMAL", "DEFAULT", "DELETE", "DENSE_RANK",
  "DESC", "DISTINCT", "DOUBLE", "DROP", "ELSE", "END", "END_P", "ESCAPE",
  "ERROR", "EXCEPT", "EXCLUDE", "EXEC", "EXECUTE", "EXISTS", "FILE_TABLE",
  "FLOAT", "FOLLOWING", "FOR", "FROM", "FULL", "G", "GROUP", "GROUPING",
  "HAVING", "HOUR", "IF", "IN", "INNER", "INSERT", "INT", "INTEGER",
  "INTERSECT", "INTERVAL", "INTO", "IS", "JOIN", "K", "LARGE", "LEFT",
  "LIKE", "M", "MAX", "MIN", "MINUTE", "MOD", "MONTH", "MULTISET",
  "NATIONAL", "NATURAL", "NCHAR", "NCLOB", "NO", "NOT", "NULLIF",
  "NUMERIC", "OBJECT", "OCTETS", "OF", "ON", "ONLY", "OR", "ORDER",
  "OTHERS", "OUTER", "OVER", "PARTITION", "PERCENT_RANK", "PRECEDING",
  "PRECISION", "RANGE", "RANK", "READ", "REAL", "RECURSIVE", "REF",
  "RIGHT", "ROW", "ROWS", "ROW_NUMBER", "SCOPE", "SECOND", "SELECT",
  "SESSION_USER", "SET", "SETS", "SMALLINT", "SOME", "SP_EXECUTESQL",
  "STDDEV_POP", "STDDEV_SAMP", "SUM", "SYSTEM_USER", "TABLE", "THEN",
  "TIES", "TIME", "TIMESTAMP", "TO", "UNBOUNDED", "UNION", "UPDATE",
  "USING", "VALUES", "VARCHAR", "VARYING", "VAR_POP", "VAR_SAMP", "WHEN",
  "WHERE", "WITH", "WITHOUT", "YEAR", "ZONE", "TOP", "NVARCHAR", "USE",
  "TRIM", "UNPIVOT", "PIVOT", "DECLARE", "';'", "'{'", "'}'", "','", "'*'",
  "'^'", "'+'", "'-'", "'!'", "'|'", "'/'", "'%'", "'&'", "$accept",
  "sql_stmt", "stmt_list", "stmt", "special_stmt", "sp_executesql_stmt",
  "call_stmt", "sql_argument_list", "sql_argument", "value_expression",
  "sp_name", "ddl_stmt", "dql_stmt", "dml_stmt", "create_stmt",
  "create_table_stmt", "insert_stmt", "insert_columns_and_source",
  "from_constructor", "delete_stmt", "drop_stmt", "drop_table_stmt",
  "update_stmt", "update_elem_list", "update_elem", "select_stmt",
  "query_expression", "query_expression_body", "query_term",
  "query_primary", "select_with_parens", "subquery", "table_subquery",
  "row_subquery", "simple_table", "opt_where", "opt_from_clause",
  "opt_groupby", "grouping_element_list", "grouping_element",
  "opt_order_by", "order_by", "sort_list", "sort_key", "opt_asc_desc",
  "opt_having", "with_clause", "with_list", "common_table_expr",
  "opt_derived_column_list", "simple_ident_list_with_parens",
  "simple_ident_list", "opt_distinct", "select_expr_list", "projection",
  "asterisk_expr", "from_list", "table_reference", "table_primary",
  "table_primary_non_join", "unpivot_clause", "pivot_clause",
  "opt_simple_ident_list_with_parens", "column_ref",
  "relation_factor_list", "relation_factor", "joined_table",
  "qualified_join", "cross_join", "join_type", "join_outer",
  "search_condition", "boolean_term", "boolean_factor", "boolean_test",
  "boolean_primary", "predicate", "comparison_predicate",
  "quantified_comparison_predicate", "between_predicate", "like_predicate",
  "in_predicate", "null_predicate", "exists_predicate", "row_expr",
  "factor0", "factor1", "factor2", "factor3", "factor4", "row_expr_list",
  "in_expr", "truth_value", "comp_op", "cnn_op", "comp_all_some_any_op",
  "plus_minus_op", "star_div_percent_mod_op", "expr_const", "case_expr",
  "case_arg", "when_clause_list", "when_clause", "case_default",
  "func_expr", "aggregate_windowed_function", "aggregate_function_name",
  "ranking_windowed_function", "ranking_function_name", "over_clause",
  "window_specification", "window_name", "window_specification_details",
  "opt_existing_window_name", "opt_window_partition_clause",
  "opt_window_frame_clause", "window_frame_units", "window_frame_extent",
  "window_frame_start", "window_frame_preceding", "window_frame_between",
  "window_frame_bound", "window_frame_following",
  "opt_window_frame_exclusion", "scalar_function", "function_call_keyword",
  "data_type", "user_defined_type_name", "relation_factor_type",
  "relation_factor_func", "reference_type", "collection_type",
  "predefined_type", "interval_type", "interval_qualifier", "start_field",
  "end_field", "single_datetime_field",
  "non_second_primary_datetime_field", "boolean_type", "datetime_type",
  "numeric_type", "exact_numeric_type", "approximate_numeric_type",
  "character_string_type", "data_type_len",
  "binary_large_object_string_type", "national_character_string_type",
  "large_object_length", "char_length_units", "multiplier",
  "distinct_or_all", "all_some_any", "opt_as_label", "as_label", "label",
  "collate_clause", "name_r", "name_type", "name_f", "reserved_type",
  "reserved_func", "reserved_other", "reserved", "opt_if_exists_clause",
  "opt_top_clause", "intnum_parens", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-565)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-460)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     806,    94,  6494,  -118,  -107,   -23,   -15,   -15,  -107,    47,
     164,   147,  -107,  6652,   135,   169,   215,   186,   221,   119,
    -565,  -565,  -565,  -565,  -565,  -565,  -565,  -565,  -565,  -565,
    -565,  -565,  -565,  -565,   182,    48,   217,  -565,  -565,    92,
     294,  -565,  -565,  -565,  -565,  -565,  -565,  -565,  -565,  -565,
    -565,  -565,  -565,  -565,  -565,  -565,  -565,  -565,  -565,  -565,
    -565,  -565,  -565,  -565,  -565,  -565,  -565,  -565,  -565,  -565,
    -565,  -565,  -565,  -565,  -565,  -565,  -565,  -565,  -565,  -565,
    -565,  -565,  -565,  -565,  -565,  -565,  -565,  -565,  -565,  -565,
    -565,  -565,  -565,  -565,  -565,  -565,  -565,  -565,  -565,  -565,
    -565,  -565,  -565,  -565,  -565,  -565,  -565,  -565,  -565,  -565,
    -565,  -565,  -565,  -565,  -565,  -565,  -565,  -565,  -565,   299,
    -565,  -565,  -565,  -565,   312,  -565,  -565,  -565,  -565,  6494,
     283,   250,   254,  -565,  -565,   249,  -565,  -565,  -107,   307,
    -565,  6494,  6494,   174,  -565,   351,  -565,  4869,  6494,  -565,
    -565,   806,   343,  -565,  -565,    47,    47,    47,    48,  -565,
    2344,  5529,   365,  -565,  -565,   283,  -565,  6494,   303,  6494,
    6494,  3667,   653,   236,   174,  6494,  6494,   362,  -565,  -565,
    -565,   285,   375,  -565,   127,   132,   379,  -565,   381,   384,
     269,   388,  -565,  -565,   238,    60,   140,   389,   390,  -565,
     391,  -565,   -12,    17,   392,   401,   211,  -565,  -565,  -565,
    -565,  -565,  -565,  -565,  -565,  -565,  -565,  -565,  -565,    88,
    -565,   376,   402,  -565,  -565,   405,  -565,  4045,    92,    92,
      92,   406,  -565,  -565,  -565,  -565,  -565,  -565,  -565,  2155,
    -565,  4045,   407,   409,   414,   416,  -565,  -565,   418,   421,
    3289,   426,   427,  -565,  -565,   428,  4045,  4045,    13,  -565,
     417,  -565,  -565,  -565,  -565,   322,   432,  -565,  -565,   361,
    -565,  -565,  -565,  -565,  -565,  -565,  -565,  -565,   486,   133,
     214,   271,  -565,  -565,  -565,  -565,  -565,  -565,   448,  -565,
     449,  -565,  -565,   451,   452,   454,  -565,   453,   457,  5694,
     458,  -565,   461,   297,  -565,   287,  -565,   109,  3478,  -565,
       7,  -565,  -565,  1974,   462,  -565,  4045,  -565,    18,  -565,
     418,   359,   475,   176,   363,   466,   176,   370,   477,   489,
     490,   491,  -565,   492,   484,  -565,   340,  -565,    28,   101,
     141,   176,   387,   488,   489,   502,  6494,   507,   349,   354,
     510,   358,   360,   176,   176,   505,  -565,   374,  6494,  -565,
    -565,  6958,  2533,   331,  -565,    73,   217,   217,  -565,  2155,
     506,    84,   486,    23,    79,   352,  4045,  4045,  4869,   520,
      94,  -565,  -565,  4045,  -565,   486,  4045,  4045,  4045,  -565,
    -565,  -565,  3289,  4869,  3289,  3289,   104,  4045,  -565,  -565,
    -565,   494,  -565,   234,  -565,   519,    90,  4045,    76,   495,
     347,  2722,  4045,   418,  -565,  -565,  -565,  4045,  -565,  -565,
    -565,  -565,  -565,  4045,  4045,  2911,   522,  3856,  4045,  6810,
    6494,   525,  5859,  -565,  3289,  -565,  6494,  5027,   378,  4045,
    -565,  -565,  -565,   117,  3478,    79,  6017,  3667,   297,  -565,
    6178,  -565,  -565,  -565,  -565,  4395,   297,   373,    64,  -565,
    6494,  -565,   530,   531,  -565,  -565,   532,   535,   176,   536,
     548,   176,   503,   554,    29,    53,   557,   541,   270,   559,
     562,   560,   569,   467,   566,   576,   580,   176,   581,    57,
     592,   593,   420,   434,   597,   438,   439,   600,   602,   614,
    6494,  -565,   603,   436,    61,  4045,  -565,  -565,  -565,   611,
    -565,  -565,  -565,  4045,  3289,    51,  -565,    33,    65,    78,
     612,    66,     6,    68,    11,  -565,   211,   432,  -565,  -565,
    -565,   143,  -565,    21,  -565,  -565,  -565,  3478,  -565,  -565,
    -565,   621,    77,  4045,   519,  4045,  -565,  -565,  -565,  -565,
    -565,    79,  -565,   133,  -565,   214,   271,  -565,  -565,  -565,
     615,    19,  4045,   504,  -565,    69,    20,   613,  -565,  -565,
    6494,  6494,   619,   322,  -565,  -565,   450,  -565,  -565,  5196,
    5364,   455,   552,  -565,  -565,  5364,  -565,  -565,  -565,  -565,
     555,  -565,  -565,   624,  -565,  4045,  4045,  -565,   639,  -565,
    -565,   489,   630,  -565,   489,   631,  -565,  -565,  -565,  -565,
    -565,  -565,  -565,   207,  -565,  -565,   643,  -565,   644,  -565,
      71,   635,  -565,  -565,   636,   646,   650,   649,   651,   662,
    -565,   489,   652,  -565,  -565,   664,   527,   144,  -565,  -565,
     161,  -565,  -565,  -565,  -565,   655,   376,  6958,  -565,   487,
    -565,    62,    79,   149,   295,  4045,  -565,   605,  4869,  -565,
    4045,  -565,  -565,  4045,  -565,  -565,  4045,  -565,  4045,    72,
    -565,  4045,    45,  -565,    87,   504,   504,    25,  6336,  -565,
    -565,  -565,  6810,  -565,  -565,  6494,  5196,   552,   656,    82,
    6017,   578,   558,  -565,  6017,   558,   558,   582,   173,   645,
     595,  4553,  -565,    79,   670,   673,  -565,   674,  -565,  -565,
     679,   680,  -565,   689,   692,   539,  -565,   687,  -565,   489,
     688,   690,  -565,   691,  6494,   546,   549,   550,   556,  -565,
    -565,   693,  -565,  4045,  4045,    79,  -565,   278,    10,    34,
      38,    79,  -565,    79,  4045,  4045,  -565,  -565,   504,  6494,
    -565,  -565,  -565,   694,  -565,  -565,  -565,   701,   702,  -565,
    -565,  -565,  -565,   552,  6017,  -565,  -565,   371,  -565,  -565,
    6017,  -565,  -565,    47,  3289,  -565,  -565,   703,  -565,  -565,
    -565,  -565,  -565,   705,   708,  -565,   709,  -565,  -565,  -565,
    -565,   542,   553,   565,   567,  6958,    79,    79,  -565,  -565,
     722,  -565,  -565,    79,    79,  -565,  -565,   714,   606,  6810,
    6494,   678,  -565,  3289,   499,  4234,   322,  4711,  -565,  -565,
    -565,  -565,  -565,  -565,  -565,  -565,   715,  -565,   704,   182,
     721,  -565,   661,   668,   448,   322,  3289,  3100,   608,   563,
    -565,    79,  -565,  -565,  4045,   -11,  6494,  6494,  6494,   322,
    -565,   736,  4234,   450,  -565,  -565,  -565,   364,  -565,   663,
     667,  4234,  -565,   551,   618,   623,   628,   686,  -565,  -565,
    -565,   351,   351,    74,   155,   170,  -565,   742,  -565,  -565,
    -565,  -565,    49,  -565,   747,   750,  -565,  -565,  -565,   551,
     627,  -565,   660,  -565,  5364,  5364,  -565,  -565,  -565,  -565,
    -565
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       5,     0,     0,     0,   525,     0,     0,     0,   525,    98,
       0,     0,   525,     0,     0,     0,     0,     0,     0,     3,
      10,    14,     9,     6,     7,     8,    29,    35,    34,    33,
      30,    45,    32,    31,    77,    52,    54,    57,    59,     0,
       0,   451,   461,   514,   253,   474,   462,   483,   468,   470,
     465,   515,   487,   482,   488,   261,   267,   466,   358,   472,
     265,   509,   513,   518,   467,   489,   506,   486,   262,   359,
     460,   475,   490,   484,   491,   485,   254,   255,   360,   357,
     492,   469,   516,   471,   493,   494,   507,   495,   512,   266,
     496,   497,   264,   508,   473,   498,   499,   500,   268,   501,
     502,   511,   476,   257,   258,   256,   510,   464,   463,   503,
     477,   259,   260,   504,   356,   505,   478,   481,   517,     0,
      28,   479,   480,   521,   132,   520,   519,   522,   452,     0,
       0,     0,   523,    15,    16,     0,    99,   100,   525,     0,
      17,     0,   498,    88,    90,    93,    11,     0,     0,     1,
       2,     5,     0,    51,    78,    98,    98,    98,    53,    60,
       0,     0,     0,   527,   528,     0,   526,     0,     0,     0,
       0,     0,     0,     0,    89,     0,     0,     0,    94,   453,
     385,     0,   413,   361,   406,   407,   405,   362,   381,   387,
       0,   390,   384,   383,   490,     0,   428,   427,   386,   391,
     499,   382,     0,     0,   397,   398,    13,   316,   319,   317,
     318,   315,   342,   455,   340,   341,   339,   373,   374,   335,
     338,   337,   320,   454,   456,     0,     4,     0,     0,     0,
       0,   451,   226,   229,   227,   230,   228,   231,   232,     0,
      19,   235,   483,   515,   482,   307,   309,   233,     0,     0,
       0,   516,     0,   311,   312,   481,     0,     0,     0,    22,
      24,    62,    64,   196,   194,    26,   151,   153,   155,   157,
     160,   162,   167,   163,   165,   164,   166,   168,    27,   182,
     185,   187,   189,   191,   195,   198,   199,   246,   479,   245,
     480,   244,   243,     0,   521,   125,   324,   520,   522,     0,
     133,    36,     0,    66,   524,    46,   130,     0,     0,   105,
      68,   101,   104,   444,   125,    12,     0,    91,     0,    96,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   389,     0,   355,   343,     0,   345,   352,   417,
     415,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   330,   331,     0,     0,   334,
     336,     0,     0,    79,    80,    83,    56,    55,    58,     0,
       0,     0,   200,     0,   236,     0,     0,     0,     0,     0,
       0,    63,   181,     0,   156,     0,     0,     0,     0,   192,
     193,    18,     0,     0,     0,     0,     0,     0,   216,   210,
     208,   209,   206,   207,   211,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   183,   219,   220,     0,   224,   221,
     222,   223,   225,     0,     0,     0,     0,     0,     0,     0,
       0,   135,     0,   529,     0,    44,     0,     0,     0,     0,
      37,    41,    38,     0,     0,   200,     0,     0,    66,   449,
       0,   103,   445,   447,   448,     0,    66,    48,     0,    95,
       0,    92,   411,     0,   408,   409,     0,   403,     0,     0,
     401,     0,   432,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   425,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   450,   321,     0,     0,     0,    84,    85,    82,     0,
      61,   161,   197,     0,     0,   241,   237,     0,     0,     0,
       0,     0,     0,     0,     0,    23,    25,   152,   154,   204,
     205,     0,   158,     0,   212,   213,   214,     0,   202,   177,
     179,     0,   173,     0,     0,     0,   215,   217,   441,   443,
     442,   169,   218,   184,   170,   186,   188,   190,   439,   440,
       0,     0,     0,     0,   298,     0,     0,   126,   325,   139,
       0,     0,   134,    67,   131,    42,    43,    40,    39,     0,
     444,    69,   109,   111,   113,   444,   112,   140,   141,   102,
      70,   446,   106,   126,    47,     0,     0,    97,     0,   412,
     393,     0,     0,   392,     0,     0,   433,   434,   438,   436,
     437,   435,   431,   430,   404,   380,     0,   378,     0,   388,
       0,   350,   344,   348,     0,     0,     0,     0,   423,     0,
     418,     0,     0,   426,   376,     0,     0,   365,   366,   367,
     370,   371,   372,   396,   399,     0,   333,     0,    21,     0,
      81,    61,   201,     0,     0,     0,   238,     0,     0,   306,
       0,   308,   302,     0,   301,   313,     0,   159,     0,     0,
     180,     0,     0,   178,   175,   252,   248,     0,     0,   263,
     299,   300,     0,   137,   138,     0,     0,     0,   112,   123,
       0,     0,   149,   148,     0,   149,   149,     0,   115,     0,
      86,     0,    49,    50,     0,     0,   395,     0,   394,   429,
       0,     0,   354,     0,     0,   351,   416,     0,   414,     0,
       0,     0,   421,     0,     0,     0,     0,     0,     0,   329,
     332,   322,    20,     0,     0,   242,   234,     0,     0,     0,
       0,   171,   203,   174,     0,     0,   251,   247,   250,   274,
     270,   269,   272,   127,   326,   136,   114,     0,     0,   124,
     118,   120,   116,   110,     0,   150,   145,     0,   146,   147,
       0,   117,   119,    98,     0,    65,   107,   127,   410,   402,
     400,   379,   377,     0,     0,   420,     0,   419,   424,   375,
     328,     0,     0,     0,     0,     0,   239,   240,   303,   305,
       0,   310,   314,   172,   176,   249,   275,     0,   276,     0,
       0,     0,   144,     0,     0,     0,    87,     0,   353,   349,
     422,   363,   364,   368,   369,   323,     0,   271,     0,    77,
     128,   327,     0,     0,     0,   143,     0,     0,   262,    71,
      72,    75,   108,   304,     0,   278,     0,     0,     0,   142,
      74,     0,     0,   277,   281,   280,   273,     0,   129,     0,
       0,     0,    73,     0,     0,     0,     0,   293,   282,   286,
     283,     0,     0,     0,     0,     0,   289,     0,   291,   285,
     284,   287,     0,   279,     0,     0,    76,   290,   292,     0,
       0,   295,     0,   296,   444,   444,   288,   294,   297,   121,
     122
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -565,  -565,   625,  -565,  -565,   345,  -565,   410,   377,  -565,
     629,  -565,  -565,  -565,  -565,  -565,  -565,  -565,   338,  -565,
    -565,  -565,  -565,   193,  -565,  -261,    12,   751,   134,   561,
    -565,  -229,  -228,  -565,  -565,  -143,  -565,  -565,   -72,   -60,
     -35,  -565,  -565,   290,  -565,  -565,  -565,   654,   622,  -565,
    -292,  -565,  -147,  -565,   353,  -565,  -565,  -445,    35,  -565,
     103,   106,  -565,  -565,  -565,     2,  -526,  -565,  -565,  -565,
    -331,  -225,   411,  -208,  -565,  -565,  -565,  -565,  -565,  -565,
    -565,  -565,  -565,  -565,  1132,   394,   393,   386,  -226,  -565,
    -359,   267,   281,  -565,  -565,  -565,  -565,  -565,  -170,  -565,
    -565,  -565,   298,  -565,  -565,     3,    54,  -565,   424,  -391,
    -565,    67,  -565,  -565,  -565,  -565,  -565,  -565,   -38,  -565,
    -565,   -69,  -565,  -565,  -565,  -565,  -356,  -565,  -565,  -565,
    -565,  -565,  -565,  -565,  -565,  -565,  -565,  -565,    -2,  -565,
    -565,  -565,  -565,  -565,  -565,  -200,  -565,  -565,  -327,   209,
    -565,  -565,  -565,  -564,  -565,   383,  -196,   604,  -355,  -422,
    1126,  -142,   785,  -565,  -565,    96,   658
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,    17,    18,    19,    20,    21,    22,   258,   259,   260,
     119,    23,    24,    25,    26,    27,    28,   440,   441,    29,
      30,    31,    32,   456,   457,    33,   370,    35,    36,    37,
     261,   262,   580,   263,    38,   435,   448,   700,   839,   840,
     153,   154,   363,   364,   508,   775,    39,   143,   144,   177,
     178,   318,   138,   310,   311,   312,   581,   687,   583,   584,
     760,   761,   762,   264,   305,   585,   586,   587,   588,   697,
     766,   265,   266,   267,   268,   269,   270,   271,   272,   273,
     274,   275,   276,   277,   445,   279,   280,   281,   282,   283,
     373,   539,   532,   411,   412,   413,   417,   423,   284,   285,
     375,   515,   516,   657,   286,   287,   288,   289,   290,   679,
     750,   751,   807,   808,   829,   856,   857,   867,   876,   869,
     870,   877,   878,   883,   291,   292,   206,   207,   208,   293,
     209,   210,   211,   212,   335,   336,   622,   337,   294,   214,
     215,   216,   217,   218,   219,   466,   220,   221,   473,   612,
     613,   562,   552,   451,   452,   453,   414,   295,   222,   296,
     297,   126,   298,   128,   169,   131,   166
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     123,   582,   315,   347,   120,   223,   502,   568,   228,   229,
     230,   123,    34,    40,   371,   443,   689,   488,   518,   381,
     382,   698,   519,   359,   521,   360,   799,   665,   523,   391,
     389,   390,   350,   568,   459,   676,   681,   526,   129,   512,
     668,   748,   384,   479,   398,   615,   442,   358,   398,   398,
     801,   358,   358,   688,   802,   658,   121,   398,   398,   398,
     358,   358,   358,   398,   744,   136,   358,   121,   565,   617,
     130,   398,   398,   634,   358,   358,   398,   649,   -60,   358,
     576,   659,   662,   398,   664,   680,   358,   712,   742,   446,
     886,   381,   461,   666,   339,   340,   506,   176,   540,   355,
     511,   543,   398,   890,   135,   358,   596,     1,   141,     1,
     529,   398,   137,   530,   358,   398,   480,   398,   358,   655,
     358,   155,   854,   357,   437,   398,   469,   123,   358,   358,
     855,   162,     1,   132,   891,   -60,    11,   507,   146,   123,
     123,   485,   323,   173,   371,   213,   123,   326,   671,   529,
     120,   140,   530,   497,   498,   341,   482,   -60,   745,   123,
     688,   348,   349,    34,   892,   123,   544,   123,   123,   303,
     438,   306,   307,   123,   123,   139,   381,   538,   669,   545,
     147,   464,   578,   121,   381,   554,   149,   528,   356,  -347,
     351,   352,   338,   663,   447,   121,   121,   800,   557,   410,
     392,   121,   121,   410,   410,   460,   541,   893,   394,   573,
     513,   156,   410,   410,   410,   121,   616,   381,   410,   223,
     531,   121,   514,   121,   121,   -60,   410,   410,   324,   121,
     121,   410,   355,   327,   171,   887,   223,     9,   410,     9,
     618,   342,   483,   606,   635,   763,   148,   607,   392,   767,
     888,   223,   513,   513,     9,   513,   513,   410,   713,   513,
     754,   852,     9,   757,   758,   660,   410,    13,   602,   481,
     410,   605,   410,   394,   705,   439,   535,   707,   536,   754,
     410,   465,    13,   439,   746,   747,   880,   632,   163,   653,
      13,   150,   731,   164,   798,   325,    58,   123,   165,   355,
     328,   881,   737,   151,   721,   590,   733,   152,   343,   484,
     159,   123,   157,   594,   160,   381,   538,   725,   726,    34,
     397,   356,   418,   415,   416,   814,    69,   611,    58,   161,
     899,   900,   167,   398,   727,   728,   358,   399,   400,   401,
     402,   403,   404,   168,   123,    78,   170,    79,   490,   172,
     381,   133,   134,   121,   757,   758,   123,   805,    69,   213,
     501,   175,   366,   367,   768,   769,   176,   121,   232,   233,
     234,   235,   236,   227,   237,   238,   213,    78,   301,    79,
     304,   509,   334,   316,   320,   405,   321,   831,   356,   863,
     322,   213,   786,   406,   329,   831,   330,   759,   407,   331,
     121,   332,   419,   333,   344,   345,   346,   353,   420,   421,
     422,   408,   121,   114,   621,   121,   354,   358,   864,   361,
     362,  -457,   376,   691,   377,   247,   122,   123,   123,   378,
     123,   379,   121,   380,   123,   123,   383,   122,   574,   393,
     825,   386,   387,   388,   123,   114,   394,   121,   123,    40,
     730,   395,   734,   123,   692,    34,   509,   381,   123,   396,
     424,   381,   693,   425,   426,   381,   427,   428,  -458,   434,
     694,   429,  -459,   695,   436,   432,   623,   433,   462,   455,
     463,   468,   467,   121,   121,   853,   121,   409,   410,   470,
     121,   121,   471,   813,   472,   474,   475,   476,   123,   477,
     121,   478,   646,   487,   121,   223,   486,   489,   492,   121,
     696,   397,   491,   493,   121,   494,   223,   495,   505,   496,
     499,   500,   510,   514,   398,   520,   865,   358,   399,   400,
     401,   402,   403,   404,   537,   381,   534,   546,   563,   606,
     547,   381,   570,   607,   575,   598,   620,   599,   600,   816,
     601,   691,   603,   122,   121,   232,   233,   234,   235,   236,
     595,   237,   238,   604,   624,   122,   122,   625,   123,   123,
     614,   122,   122,   619,   627,   626,   405,   123,   123,   884,
     885,   629,   692,   123,   406,   122,   628,   608,   835,   407,
     693,   122,   630,   122,   122,   631,   638,   633,   694,   122,
     122,   695,   408,   609,   691,   864,   124,   610,   636,   637,
     639,   849,   247,   640,   641,   642,   643,   145,   644,   645,
     647,   836,   648,   611,   121,   121,   815,   651,   661,   670,
     682,   675,   678,   121,   121,   692,   685,   513,   696,   121,
     699,   701,   690,   693,   704,   213,   706,   708,   710,   711,
     714,   694,   715,   223,   695,   717,   213,   232,   233,   234,
     235,   236,   716,   237,   238,   718,   719,   720,   722,   723,
     724,   729,   756,   732,   736,   773,   123,   764,   409,   410,
     123,   770,   774,   123,   123,   765,   778,   866,   123,   779,
     780,   696,   123,   875,   783,   781,   782,   784,   509,   123,
    -346,   121,    44,   785,   787,   791,   788,   789,   792,   793,
     795,   809,   121,   874,   247,   794,   810,   811,   821,   875,
     817,   818,   123,   122,   819,   820,   790,   826,    55,   822,
     827,   843,   121,   124,   844,   828,   121,   122,   846,   121,
     121,   823,   847,   824,   121,   124,   145,   123,   121,   848,
     852,   861,   124,   871,   880,   121,   851,   872,   879,   881,
     882,   889,   123,   894,    68,   300,   895,   897,   123,   525,
     122,   124,   504,   124,   124,   314,   226,   225,   121,   145,
     319,   577,   122,    76,    77,   122,   898,   127,   702,   873,
     158,   368,   862,   213,   845,   650,   174,   317,   127,   812,
     589,   771,   122,   121,   772,   527,   553,   123,   123,   556,
     555,   673,   667,   656,   833,   123,   806,   122,   121,   868,
     896,     1,   709,   302,   121,     0,     0,     0,     0,     0,
     103,   104,   105,   591,     0,     0,     0,     2,     0,     0,
       0,     0,     0,     0,   123,   123,   123,   111,   112,   121,
       0,     0,     0,   122,   122,     0,   122,     3,     0,     0,
     122,   122,     0,   121,   121,   834,     0,     0,     4,     0,
     122,   121,     0,     5,   122,     0,     0,     0,     0,   122,
       0,     6,     7,     0,   122,     0,     0,     0,     0,     0,
       0,     0,   123,   123,     0,     0,     0,     0,     8,     0,
     121,   121,   121,   431,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   127,     0,     0,   454,     0,     0,
       0,     0,     0,     0,   122,     0,   127,   127,     0,     0,
       0,     0,   224,   127,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   127,     0,   121,   121,
     124,     9,   127,    10,   127,   127,     0,    11,     0,     0,
     127,   127,   124,     0,     0,     0,     0,     0,     0,     0,
      12,     0,     0,     0,     0,     0,     0,     0,     0,    13,
       0,     0,     0,     0,     0,    14,     0,     0,     0,    15,
       0,    16,     0,     0,   122,   122,     0,     0,     0,     0,
       0,     0,     0,   122,   122,     0,     0,     0,     0,   122,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   567,   569,     0,   572,     0,     0,     0,
     124,   319,     0,     0,     0,     0,     0,     0,     0,     0,
     124,   314,     0,     0,   454,     0,     0,     0,     0,   593,
       0,     0,     0,     0,   597,     0,     0,     0,     0,     0,
       0,   122,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   122,     0,   127,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   127,     0,
       0,     0,   122,     0,   124,     0,   122,     0,     0,   122,
     122,     0,     0,     0,   122,     0,     0,     0,   122,     0,
       0,     0,     0,     0,     0,   122,     0,     0,   125,     0,
       0,   127,     0,     0,     0,     0,     0,     0,     0,   125,
       0,     0,     0,   127,     0,     0,   224,     0,   122,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   224,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   122,   683,   684,     0,     0,   224,     0,
       0,     0,     0,   124,   454,     0,     0,     0,   122,   454,
       0,     0,     0,     0,   122,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   127,     0,   127,     0,   122,
       0,   127,   127,     0,     0,     0,     0,     0,     0,     0,
       0,   127,     0,   122,   122,   127,     0,     0,     0,     0,
       0,   122,     0,     0,     0,   127,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   125,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   125,   125,     0,
     122,   122,   122,     0,   125,     0,     0,     0,     0,     0,
       0,     0,   752,     0,     0,   127,   753,   125,     0,   755,
     124,     0,   278,   125,   124,   125,   125,     0,   124,     0,
       0,   125,   125,   313,     0,   777,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   122,   122,
       0,     0,     0,     0,     0,     0,     0,     0,   124,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   752,     0,   127,   127,     0,     0,   365,
       0,     0,     0,     0,   127,   127,     0,     0,   124,     0,
     127,   372,     0,   374,   124,     0,     0,     0,     0,     0,
       0,     0,   385,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   830,   832,     0,     0,     0,     0,     0,
       0,   830,     0,     0,     0,   125,     0,     0,     0,     0,
       0,     0,   224,     0,     0,     0,     0,     0,     0,   125,
       0,     0,     0,   224,     0,     0,     0,     0,   458,     0,
     858,   859,   860,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   127,     0,     0,     0,     0,     0,     0,
     127,   127,   125,     0,     0,   127,     0,     0,     0,   127,
       0,     0,     0,     0,   125,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   278,     0,     0,     0,   454,   454,
       0,   372,     0,     0,     0,     0,     0,     0,   517,   127,
       0,     0,     0,     0,     0,     0,     0,     0,   522,     0,
     524,     0,     0,     0,   278,     0,   385,   385,     0,   533,
       0,     0,     0,     0,   127,     0,     0,     0,     0,   542,
       0,     0,     0,   551,     0,     0,     0,     0,     0,   127,
       0,     0,     0,     0,     0,   127,   125,   561,   125,     0,
     566,     0,   125,   125,     0,     0,   385,     0,     0,     0,
       0,     0,   125,     0,     0,     0,   125,     0,     0,   313,
     224,     0,     0,     0,     0,     0,   125,     0,     0,     0,
       0,     0,     0,     0,     0,   127,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   125,     0,     0,     0,
       0,   127,   127,   127,     0,     0,     0,   365,     0,     0,
       0,     0,     0,     0,     0,   652,   654,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   672,     0,   674,     0,   127,
     127,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   677,     0,   125,   125,     0,     0,
       0,     0,     0,     0,     0,   125,   125,     0,     0,     0,
       0,   125,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   458,   703,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   735,     0,     0,
       0,     0,   738,     0,     0,   739,     0,     0,   740,     0,
     741,     0,     0,   743,   125,     0,     0,     0,     0,     0,
       0,   125,   125,     0,     0,     0,   125,     0,     0,     0,
     125,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     125,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   796,   797,     0,     0,     0,
       0,     0,     0,     0,     0,   125,   803,   804,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     125,     0,     0,     0,     0,     0,   125,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   385,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   125,     0,     0,     0,
       0,     0,     0,     0,     0,   385,     0,   841,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   385,     0,
       0,     0,   125,   125,   125,     0,     0,    41,   449,     0,
       0,     0,     0,     0,   841,     0,     0,     0,     0,     0,
       0,     0,     0,   841,     0,    42,   450,    43,    44,     0,
      45,    46,     0,     0,     0,     0,     0,    47,    48,    49,
      50,     0,   398,    51,    52,   358,     0,     0,     0,     0,
     125,   125,    53,    54,    55,     0,     0,    56,     0,     0,
       0,    57,    58,     0,    59,     0,     0,    60,    61,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    62,     0,
       0,     0,    63,    64,    65,    66,     0,     0,    67,     0,
      68,     0,    69,     0,     0,     0,     0,    70,    71,     0,
      72,     0,     0,     0,    73,    74,     0,     0,    75,    76,
      77,    78,     0,    79,    80,     0,     0,    81,     0,     0,
       0,    82,    83,    84,    85,    86,     0,    87,     0,     0,
      88,     0,     0,     0,    89,    90,    91,     0,    92,    93,
      94,    95,    96,     0,    97,     0,    98,    99,   100,     0,
       0,     0,   101,   102,     0,     0,   103,   104,   105,     0,
       0,     0,   106,   107,   108,     0,   109,     0,     0,     0,
       0,   110,     0,   111,   112,     0,     0,     0,   113,   114,
     115,     0,   116,     0,   117,     0,     0,   118,   231,   232,
     233,   234,   235,   236,     0,   237,   238,   410,     0,     0,
     369,     0,     0,     0,     0,     0,    42,     0,    43,    44,
       0,    45,    46,     0,     0,     0,     0,   241,   242,    48,
      49,    50,     0,     0,   243,    52,     0,     0,     0,     0,
       0,     0,     0,   244,    54,    55,     0,     0,    56,     0,
     245,   246,    57,    58,     0,    59,   247,     0,    60,    61,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    62,
       0,     0,   248,    63,    64,    65,    66,     0,     0,    67,
       0,    68,     0,    69,     0,     0,     0,     0,    70,    71,
       0,    72,     0,     0,     0,    73,    74,   249,     0,    75,
      76,    77,    78,     0,    79,    80,     0,     0,    81,     0,
       0,   250,   251,    83,    84,    85,    86,     0,    87,     0,
       0,    88,     0,     0,     0,    89,    90,    91,     0,    92,
      93,    94,    95,    96,   252,    97,     0,    98,    99,   100,
       9,   253,     0,   101,   102,     0,     0,   103,   104,   105,
     254,     0,     0,   106,   107,   108,     0,   109,     0,     0,
       0,     0,   110,     0,   111,   112,     0,     0,    13,   113,
     114,   115,     0,   116,     0,   255,     0,     0,   118,     0,
       0,     0,     0,     0,     0,   256,   257,   231,   232,   233,
     234,   235,   236,     0,   237,   238,     0,     0,     0,   239,
     240,     0,     0,     0,     0,    42,     0,    43,    44,     0,
      45,    46,     0,     0,     0,     0,   241,   242,    48,    49,
      50,     0,     0,   243,    52,     0,     0,     0,     0,     0,
       0,     0,   244,    54,    55,     0,     0,    56,     0,   245,
     246,    57,    58,     0,    59,   247,     0,    60,    61,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    62,     0,
       0,   248,    63,    64,    65,    66,     0,     0,    67,     0,
      68,     0,    69,     0,     0,     0,     0,    70,    71,     0,
      72,     0,     0,     0,    73,    74,   249,     0,    75,    76,
      77,    78,     0,    79,    80,     0,     0,    81,     0,     0,
     250,   251,    83,    84,    85,    86,     0,    87,     0,     0,
      88,     0,     0,     0,    89,    90,    91,     0,    92,    93,
      94,    95,    96,   252,    97,     0,    98,    99,   100,     0,
     253,     0,   101,   102,     0,     0,   103,   104,   105,   254,
       0,     0,   106,   107,   108,     0,   109,     0,     0,     0,
       0,   110,     0,   111,   112,     0,     0,     0,   113,   114,
     115,     0,   116,     0,   255,     0,     0,   118,     0,     0,
       0,     0,     0,     0,   256,   257,   231,   232,   233,   234,
     235,   236,     0,   237,   238,     0,     0,     0,   239,   503,
       0,     0,     0,     0,    42,     0,    43,    44,     0,    45,
      46,     0,     0,     0,     0,   241,   242,    48,    49,    50,
       0,     0,   243,    52,     0,     0,     0,     0,     0,     0,
       0,   244,    54,    55,     0,     0,    56,     0,   245,   246,
      57,    58,     0,    59,   247,     0,    60,    61,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    62,     0,     0,
     248,    63,    64,    65,    66,     0,     0,    67,     0,    68,
       0,    69,     0,     0,     0,     0,    70,    71,     0,    72,
       0,     0,     0,    73,    74,   249,     0,    75,    76,    77,
      78,     0,    79,    80,     0,     0,    81,     0,     0,   250,
     251,    83,    84,    85,    86,     0,    87,     0,     0,    88,
       0,     0,     0,    89,    90,    91,     0,    92,    93,    94,
      95,    96,   252,    97,     0,    98,    99,   100,     0,   253,
       0,   101,   102,     0,     0,   103,   104,   105,   254,     0,
       0,   106,   107,   108,     0,   109,     0,     0,     0,     0,
     110,     0,   111,   112,     0,     0,     0,   113,   114,   115,
       0,   116,     0,   255,     0,     0,   118,     0,     0,     0,
       0,     0,     0,   256,   257,   231,   232,   233,   234,   235,
     236,     0,   237,   238,     0,     0,     0,   308,     0,     0,
     548,     0,   549,    42,     0,    43,    44,     0,    45,    46,
       0,     0,     0,     0,   241,   242,    48,    49,    50,     0,
       0,   243,    52,     0,     0,     0,     0,     0,     0,     0,
     244,    54,    55,     0,     0,    56,     0,   245,   246,    57,
      58,     0,    59,   247,     0,    60,    61,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    62,     0,     0,     0,
      63,    64,    65,    66,     0,     0,    67,     0,    68,     0,
      69,     0,     0,     0,     0,    70,    71,     0,    72,     0,
       0,     0,    73,    74,   249,     0,    75,    76,    77,    78,
       0,    79,    80,     0,     0,    81,     0,     0,     0,   251,
      83,    84,    85,    86,     0,    87,     0,     0,    88,     0,
       0,     0,    89,    90,    91,     0,    92,    93,    94,    95,
      96,   252,    97,     0,    98,    99,   100,     0,   253,     0,
     101,   102,   550,     0,   103,   104,   105,   254,     0,     0,
     106,   107,   108,     0,   109,     0,     0,     0,     0,   110,
       0,   111,   112,     0,     0,     0,   113,   114,   115,     0,
     116,     0,   255,     0,     0,   118,     0,     0,     0,     0,
       0,     0,   256,   257,   231,   232,   233,   234,   235,   236,
       0,   237,   238,     0,     0,     0,   308,     0,     0,   558,
       0,     0,    42,     0,    43,    44,     0,    45,    46,     0,
       0,     0,     0,   241,   242,    48,    49,    50,     0,     0,
     243,    52,     0,     0,     0,     0,     0,     0,     0,   244,
      54,    55,     0,     0,    56,     0,   245,   246,    57,    58,
       0,    59,   247,     0,    60,    61,   559,     0,     0,     0,
       0,     0,     0,     0,     0,    62,     0,     0,     0,    63,
      64,    65,    66,     0,     0,    67,     0,    68,     0,    69,
       0,     0,     0,     0,    70,    71,     0,    72,     0,     0,
       0,    73,    74,   249,     0,    75,    76,    77,    78,     0,
      79,    80,     0,     0,    81,     0,     0,     0,   251,    83,
      84,    85,    86,     0,    87,     0,     0,    88,     0,     0,
       0,    89,    90,    91,     0,    92,    93,    94,    95,    96,
     252,    97,     0,    98,    99,   100,     0,   253,     0,   101,
     102,     0,     0,   103,   104,   105,   254,     0,     0,   106,
     107,   108,     0,   109,     0,     0,     0,     0,   110,     0,
     111,   112,     0,     0,     0,   113,   114,   115,     0,   116,
       0,   255,     0,     0,   118,     0,     0,     0,     0,   560,
       0,   256,   257,   231,   232,   233,   234,   235,   236,     0,
     237,   238,     0,     0,     0,   444,   850,     0,     0,     0,
       0,    42,     0,    43,    44,     0,    45,    46,     0,     0,
       0,     0,   241,   242,    48,    49,    50,     0,     0,   243,
      52,     0,     0,     0,     0,     0,     0,     0,   244,    54,
      55,     0,     0,    56,     0,   245,   246,    57,    58,     0,
      59,   247,     0,    60,    61,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    62,     0,     0,     0,    63,    64,
      65,    66,     0,     0,    67,     0,    68,     0,    69,     0,
       0,     0,     0,    70,    71,     0,    72,     0,     0,     0,
      73,    74,   249,     0,    75,    76,    77,    78,     0,    79,
      80,     0,     0,    81,     0,     0,     0,   251,    83,    84,
      85,    86,     0,    87,     0,     0,    88,     0,     0,     0,
      89,    90,    91,     0,    92,    93,    94,    95,    96,   252,
      97,     0,    98,    99,   100,     9,   253,     0,   101,   102,
       0,     0,   103,   104,   105,   254,     0,     0,   106,   107,
     108,     0,   109,     0,     0,     0,     0,   110,     0,   111,
     112,     0,     0,    13,   113,   114,   115,     0,   116,     0,
     255,     0,     0,   118,     0,     0,     0,     0,     0,     0,
     256,   257,   231,   232,   233,   234,   235,   236,     0,   237,
     238,     0,     0,     0,   239,     0,     0,     0,     0,     0,
      42,     0,    43,    44,     0,    45,    46,     0,     0,     0,
       0,   241,   242,    48,    49,    50,     0,     0,   243,    52,
       0,     0,     0,     0,     0,     0,     0,   244,    54,    55,
       0,     0,    56,     0,   245,   246,    57,    58,     0,    59,
     247,     0,    60,    61,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,   248,    63,    64,    65,
      66,     0,     0,    67,     0,    68,     0,    69,     0,     0,
       0,     0,    70,    71,     0,    72,     0,     0,     0,    73,
      74,   249,     0,    75,    76,    77,    78,     0,    79,    80,
       0,     0,    81,     0,     0,   250,   251,    83,    84,    85,
      86,     0,    87,     0,     0,    88,     0,     0,     0,    89,
      90,    91,     0,    92,    93,    94,    95,    96,   252,    97,
       0,    98,    99,   100,     0,   253,     0,   101,   102,     0,
       0,   103,   104,   105,   254,     0,     0,   106,   107,   108,
       0,   109,     0,     0,     0,     0,   110,     0,   111,   112,
       0,     0,     0,   113,   114,   115,     0,   116,     0,   255,
       0,     0,   118,     0,     0,     0,     0,     0,     0,   256,
     257,   231,   232,   233,   234,   235,   236,     0,   237,   238,
       0,     0,     0,   444,     0,     0,     0,     0,     0,    42,
       0,    43,    44,     0,    45,    46,     0,     0,     0,     0,
     241,   242,    48,    49,    50,     0,     0,   243,    52,     0,
       0,     0,     0,     0,     0,     0,   244,    54,    55,     0,
       0,    56,     0,   245,   246,    57,    58,     0,    59,   247,
       0,    60,    61,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    62,     0,     0,     0,    63,    64,    65,    66,
       0,     0,    67,     0,    68,     0,    69,     0,     0,     0,
       0,    70,    71,     0,    72,     0,     0,     0,    73,    74,
     249,     0,    75,    76,    77,    78,     0,    79,    80,     0,
       0,    81,     0,     0,     0,   251,    83,    84,    85,    86,
       0,    87,     0,     0,    88,     0,     0,     0,    89,    90,
      91,     0,    92,    93,    94,    95,    96,   252,    97,     0,
      98,    99,   100,     9,   253,     0,   101,   102,     0,     0,
     103,   104,   105,   254,     0,     0,   106,   107,   108,     0,
     109,     0,     0,     0,     0,   110,     0,   111,   112,     0,
       0,    13,   113,   114,   115,     0,   116,     0,   255,     0,
       0,   118,     0,     0,     0,     0,     0,     0,   256,   257,
     231,   232,   233,   234,   235,   236,     0,   237,   238,     0,
       0,     0,   308,     0,     0,     0,     0,     0,    42,     0,
      43,    44,     0,    45,    46,     0,     0,     0,     0,   241,
     242,    48,    49,    50,     0,     0,   243,    52,     0,     0,
       0,     0,     0,     0,     0,   244,    54,    55,     0,     0,
      56,     0,   245,   246,    57,    58,     0,    59,   247,     0,
      60,    61,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    62,     0,     0,     0,    63,    64,    65,    66,     0,
       0,    67,     0,    68,     0,    69,     0,     0,     0,     0,
      70,    71,     0,    72,     0,     0,     0,    73,    74,   249,
       0,    75,    76,    77,    78,     0,    79,    80,     0,     0,
      81,     0,     0,     0,   251,    83,    84,    85,    86,     0,
      87,     0,     0,    88,     0,     0,     0,    89,    90,    91,
       0,    92,    93,    94,    95,    96,   252,    97,     0,    98,
      99,   100,     0,   253,     0,   101,   102,     0,     0,   103,
     104,   105,   254,     0,     0,   106,   107,   108,     0,   109,
       0,     0,     0,     0,   110,     0,   111,   112,     0,     0,
       0,   113,   114,   115,     0,   116,     0,   255,     0,     0,
     118,     0,     0,     0,     0,   309,     0,   256,   257,   231,
     232,   233,   234,   235,   236,     0,   237,   238,     0,     0,
       0,   308,   564,     0,     0,     0,     0,    42,     0,    43,
      44,     0,    45,    46,     0,     0,     0,     0,   241,   242,
      48,    49,    50,     0,     0,   243,    52,     0,     0,     0,
       0,     0,     0,     0,   244,    54,    55,     0,     0,    56,
       0,   245,   246,    57,    58,     0,    59,   247,     0,    60,
      61,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,    63,    64,    65,    66,     0,     0,
      67,     0,    68,     0,    69,     0,     0,     0,     0,    70,
      71,     0,    72,     0,     0,     0,    73,    74,   249,     0,
      75,    76,    77,    78,     0,    79,    80,     0,     0,    81,
       0,     0,     0,   251,    83,    84,    85,    86,     0,    87,
       0,     0,    88,     0,     0,     0,    89,    90,    91,     0,
      92,    93,    94,    95,    96,   252,    97,     0,    98,    99,
     100,     0,   253,     0,   101,   102,     0,     0,   103,   104,
     105,   254,     0,     0,   106,   107,   108,     0,   109,     0,
       0,     0,     0,   110,     0,   111,   112,     0,     0,     0,
     113,   114,   115,     0,   116,     0,   255,     0,     0,   118,
       0,     0,     0,     0,     0,     0,   256,   257,   231,   232,
     233,   234,   235,   236,     0,   237,   238,     0,     0,     0,
     308,     0,     0,     0,     0,     0,    42,     0,    43,    44,
       0,    45,    46,     0,     0,     0,     0,   241,   242,    48,
      49,    50,     0,     0,   243,    52,     0,     0,     0,     0,
       0,     0,     0,   244,    54,    55,     0,     0,    56,     0,
     245,   246,    57,    58,     0,    59,   247,     0,    60,    61,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    62,
       0,     0,     0,    63,    64,    65,    66,     0,     0,    67,
       0,    68,     0,    69,     0,     0,     0,     0,    70,    71,
       0,    72,     0,     0,     0,    73,    74,   249,     0,    75,
      76,    77,    78,     0,    79,    80,     0,     0,    81,     0,
       0,     0,   251,    83,    84,    85,    86,     0,    87,     0,
       0,    88,     0,     0,     0,    89,    90,    91,     0,    92,
      93,    94,    95,    96,   252,    97,     0,    98,    99,   100,
       0,   253,     0,   101,   102,     0,     0,   103,   104,   105,
     254,     0,     0,   106,   107,   108,     0,   109,     0,     0,
       0,     0,   110,     0,   111,   112,     0,     0,     0,   113,
     114,   115,     0,   116,     0,   255,     0,     0,   118,     0,
       0,     0,     0,     0,     0,   256,   257,   231,   232,   233,
     234,   235,   236,     0,   237,   238,     0,     0,     0,   837,
       0,     0,     0,     0,     0,    42,     0,    43,    44,     0,
      45,    46,     0,     0,     0,     0,   241,   242,    48,    49,
      50,     0,     0,   243,    52,     0,     0,     0,     0,     0,
       0,     0,   244,    54,    55,     0,     0,    56,     0,   245,
     246,    57,    58,     0,    59,   247,     0,    60,    61,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    62,     0,
       0,     0,    63,    64,    65,    66,     0,     0,    67,     0,
     838,     0,    69,     0,     0,     0,     0,    70,    71,     0,
      72,     0,     0,     0,    73,    74,   249,     0,    75,    76,
      77,    78,     0,    79,    80,     0,     0,    81,     0,     0,
       0,   251,    83,    84,    85,    86,     0,    87,     0,     0,
      88,     0,     0,     0,    89,    90,    91,     0,    92,    93,
      94,    95,    96,   252,    97,     0,    98,    99,   100,     0,
     253,     0,   101,   102,     0,     0,   103,   104,   105,   254,
       0,     0,   106,   107,   108,     0,   109,     0,   231,     0,
       0,   110,     0,   111,   112,     0,     0,     0,   113,   114,
     115,     0,   116,     0,   255,     0,    42,   118,    43,    44,
       0,    45,    46,     0,   256,   257,     0,     0,    47,    48,
      49,    50,     0,     0,    51,    52,     0,     0,     0,     0,
       0,     0,     0,    53,    54,    55,     0,     0,    56,     0,
       0,     0,    57,    58,     0,    59,     0,     0,    60,    61,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    62,
       0,     0,     0,    63,    64,    65,    66,     0,     0,    67,
       0,    68,     0,    69,     0,     0,     0,     0,    70,    71,
       0,    72,     0,     0,     0,    73,    74,     0,     0,    75,
      76,    77,    78,     0,    79,    80,     0,     0,    81,     0,
       0,     0,    82,    83,    84,    85,    86,     0,    87,     0,
       0,    88,     0,     0,     0,    89,    90,    91,     0,    92,
      93,    94,    95,    96,     0,    97,     0,    98,    99,   100,
       0,     0,     0,   101,   102,     0,     0,   103,   104,   105,
       0,     0,     0,   106,   107,   108,   231,   109,     0,     0,
       0,     0,   110,     0,   111,   112,     0,     0,     0,   113,
     114,   115,     0,   116,    42,   117,    43,    44,   118,    45,
      46,     0,     0,   592,     0,     0,    47,    48,    49,    50,
       0,     0,    51,    52,     0,     0,     0,     0,     0,     0,
       0,    53,    54,    55,     0,     0,    56,     0,     0,     0,
      57,    58,     0,    59,     0,     0,    60,    61,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    62,     0,     0,
       0,    63,    64,    65,    66,     0,     0,    67,     0,    68,
       0,    69,     0,     0,     0,     0,    70,    71,     0,    72,
       0,     0,     0,    73,    74,     0,     0,    75,    76,    77,
      78,     0,    79,    80,     0,     0,    81,     0,     0,     0,
      82,    83,    84,    85,    86,     0,    87,     0,     0,    88,
       0,     0,     0,    89,    90,    91,     0,    92,    93,    94,
      95,    96,     0,    97,     0,    98,    99,   100,     0,     0,
       0,   101,   102,     0,     0,   103,   104,   105,     0,     0,
       0,   106,   107,   108,   231,   109,     0,     0,     0,     0,
     110,     0,   111,   112,     0,     0,     0,   113,   114,   115,
       0,   116,    42,   117,    43,    44,   118,    45,    46,     0,
       0,   776,     0,     0,    47,    48,    49,    50,     0,     0,
      51,    52,     0,     0,     0,     0,     0,     0,     0,    53,
      54,    55,     0,     0,    56,     0,     0,     0,    57,    58,
       0,    59,     0,     0,    60,    61,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    62,     0,     0,     0,    63,
      64,    65,    66,     0,     0,    67,     0,    68,     0,    69,
       0,     0,     0,     0,    70,    71,     0,    72,     0,     0,
       0,    73,    74,     0,     0,    75,    76,    77,    78,     0,
      79,    80,     0,     0,    81,     0,     0,     0,    82,    83,
      84,    85,    86,     0,    87,     0,     0,    88,     0,     0,
       0,    89,    90,    91,     0,    92,    93,    94,    95,    96,
       0,    97,     0,    98,    99,   100,     0,     0,     0,   101,
     102,     0,     0,   103,   104,   105,     0,     0,     0,   106,
     107,   108,   179,   109,     0,     0,     0,     0,   110,     0,
     111,   112,     0,     0,     0,   113,   114,   115,     0,   116,
       0,   117,    43,    44,   118,   180,   181,   182,   183,   842,
       0,     0,    47,   184,   185,     0,   186,     0,    51,    52,
       0,     0,     0,     0,     0,     0,     0,    53,    54,    55,
       0,     0,    56,     0,     0,     0,   187,    58,   188,   189,
       0,     0,    60,    61,     0,   190,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,    63,   191,    65,
      66,     0,     0,    67,     0,    68,     0,    69,     0,     0,
       0,     0,   192,   193,     0,   194,     0,     0,     0,    73,
      74,     0,     0,    75,    76,    77,    78,     0,    79,    80,
     195,     0,   196,   197,     0,     0,    82,   198,    84,    85,
      86,     0,    87,     0,     0,    88,     0,     0,     0,    89,
      90,    91,     0,    92,    93,   199,    95,   200,     0,    97,
       0,    98,    99,   100,     0,     0,     0,   101,   201,     0,
       0,   103,   104,   105,     0,     0,     0,   106,   202,   203,
      41,   109,     0,     0,     0,     0,   204,     0,   111,   112,
       0,     0,     1,   113,   114,   115,     0,   205,    42,   117,
      43,    44,   118,    45,    46,     0,     0,     0,     0,     0,
      47,    48,    49,    50,     0,     0,    51,    52,     0,     0,
       0,     0,     0,     0,     0,    53,    54,    55,     0,     0,
      56,     0,     0,     0,    57,    58,     0,    59,     0,     0,
      60,    61,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    62,     0,     0,     0,    63,    64,    65,    66,     0,
       0,    67,     0,    68,     0,    69,     0,     0,     0,     0,
      70,    71,     0,    72,     0,     0,     0,    73,    74,     0,
       0,    75,    76,    77,    78,     0,    79,    80,     0,     0,
      81,     0,     0,     0,    82,    83,    84,    85,    86,     0,
      87,     0,     0,    88,     0,     0,     0,    89,    90,    91,
       0,    92,    93,    94,    95,    96,     0,    97,     0,    98,
      99,   100,     9,     0,     0,   101,   102,     0,     0,   103,
     104,   105,     0,     0,     0,   106,   107,   108,     0,   109,
       0,     0,     0,     0,   110,     0,   111,   112,     0,    41,
      13,   113,   114,   115,     0,   116,     0,   117,     0,     0,
     118,   686,     0,     0,     0,     0,     0,    42,     0,    43,
      44,     0,    45,    46,     0,     0,     0,     0,     0,    47,
      48,    49,    50,     0,     0,    51,    52,     0,     0,     0,
       0,     0,     0,     0,    53,    54,    55,     0,     0,    56,
       0,     0,     0,    57,    58,     0,    59,     0,     0,    60,
      61,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,    63,    64,    65,    66,     0,     0,
      67,     0,    68,     0,    69,     0,     0,     0,     0,    70,
      71,     0,    72,     0,     0,     0,    73,    74,     0,     0,
      75,    76,    77,    78,     0,    79,    80,     0,     0,    81,
       0,     0,     0,    82,    83,    84,    85,    86,     0,    87,
       0,     0,    88,     0,     0,     0,    89,    90,    91,     0,
      92,    93,    94,    95,    96,     0,    97,     0,    98,    99,
     100,     9,     0,     0,   101,   102,     0,     0,   103,   104,
     105,     0,     0,     0,   106,   107,   108,     0,   109,     0,
       0,     0,     0,   110,     0,   111,   112,    41,   449,    13,
     113,   114,   115,     0,   116,     0,   117,     0,     0,   118,
       0,     0,     0,     0,     0,    42,   450,    43,    44,     0,
      45,    46,     0,     0,     0,     0,     0,    47,    48,    49,
      50,     0,     0,    51,    52,     0,     0,     0,     0,     0,
       0,     0,    53,    54,    55,     0,     0,    56,     0,     0,
       0,    57,    58,     0,    59,     0,     0,    60,    61,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    62,     0,
       0,     0,    63,    64,    65,    66,     0,     0,    67,     0,
      68,     0,    69,     0,     0,     0,     0,    70,    71,     0,
      72,     0,     0,     0,    73,    74,     0,     0,    75,    76,
      77,    78,     0,    79,    80,     0,     0,    81,     0,     0,
       0,    82,    83,    84,    85,    86,     0,    87,     0,     0,
      88,     0,     0,     0,    89,    90,    91,     0,    92,    93,
      94,    95,    96,     0,    97,     0,    98,    99,   100,     0,
       0,     0,   101,   102,     0,     0,   103,   104,   105,     0,
       0,     0,   106,   107,   108,     0,   109,     0,     0,     0,
       0,   110,    41,   111,   112,     0,     0,     0,   113,   114,
     115,     0,   116,     0,   117,     0,   299,   118,     0,     0,
      42,     0,    43,    44,     0,    45,    46,     0,     0,     0,
       0,     0,    47,    48,    49,    50,     0,     0,    51,    52,
       0,     0,     0,     0,     0,     0,     0,    53,    54,    55,
       0,     0,    56,     0,     0,     0,    57,    58,     0,    59,
       0,     0,    60,    61,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,    63,    64,    65,
      66,     0,     0,    67,     0,    68,     0,    69,     0,     0,
       0,     0,    70,    71,     0,    72,     0,     0,     0,    73,
      74,     0,     0,    75,    76,    77,    78,     0,    79,    80,
       0,     0,    81,     0,     0,     0,    82,    83,    84,    85,
      86,     0,    87,     0,     0,    88,     0,     0,     0,    89,
      90,    91,     0,    92,    93,    94,    95,    96,     0,    97,
       0,    98,    99,   100,     0,     0,     0,   101,   102,     0,
       0,   103,   104,   105,     0,     0,     0,   106,   107,   108,
       0,   109,     0,     0,     0,     0,   110,    41,   111,   112,
       0,     0,     0,   113,   114,   115,     0,   116,     0,   117,
       0,   430,   118,     0,     0,    42,     0,    43,    44,     0,
      45,    46,     0,     0,     0,     0,     0,    47,    48,    49,
      50,     0,     0,    51,    52,     0,     0,     0,     0,     0,
       0,     0,    53,    54,    55,     0,     0,    56,     0,     0,
       0,    57,    58,     0,    59,     0,     0,    60,    61,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    62,     0,
       0,     0,    63,    64,    65,    66,     0,     0,    67,     0,
      68,     0,    69,     0,     0,     0,     0,    70,    71,     0,
      72,     0,     0,     0,    73,    74,     0,     0,    75,    76,
      77,    78,     0,    79,    80,     0,     0,    81,     0,     0,
       0,    82,    83,    84,    85,    86,     0,    87,     0,     0,
      88,     0,     0,     0,    89,    90,    91,     0,    92,    93,
      94,    95,    96,     0,    97,     0,    98,    99,   100,     0,
       0,     0,   101,   102,     0,     0,   103,   104,   105,     0,
       0,     0,   106,   107,   108,     0,   109,     0,     0,     0,
       0,   110,    41,   111,   112,     0,     0,     0,   113,   114,
     115,     0,   116,     0,   117,     0,   571,   118,     0,     0,
      42,     0,    43,    44,     0,    45,    46,     0,     0,     0,
       0,     0,    47,    48,    49,    50,     0,     0,    51,    52,
       0,     0,     0,     0,     0,     0,     0,    53,    54,    55,
       0,     0,    56,     0,     0,     0,    57,    58,     0,    59,
       0,     0,    60,    61,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,    63,    64,    65,
      66,     0,     0,    67,     0,    68,     0,    69,     0,     0,
       0,     0,    70,    71,     0,    72,     0,     0,     0,    73,
      74,     0,     0,    75,    76,    77,    78,     0,    79,    80,
       0,     0,    81,     0,     0,     0,    82,    83,    84,    85,
      86,     0,    87,     0,     0,    88,     0,     0,     0,    89,
      90,    91,     0,    92,    93,    94,    95,    96,     0,    97,
       0,    98,    99,   100,     0,     0,     0,   101,   102,     0,
       0,   103,   104,   105,     0,     0,     0,   106,   107,   108,
      41,   109,     0,     0,     0,     0,   110,     0,   111,   112,
       0,     0,   579,   113,   114,   115,     0,   116,    42,   117,
      43,    44,   118,    45,    46,     0,     0,     0,     0,     0,
      47,    48,    49,    50,     0,     0,    51,    52,     0,     0,
       0,     0,     0,     0,     0,    53,    54,    55,     0,     0,
      56,     0,     0,     0,    57,    58,     0,    59,     0,     0,
      60,    61,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    62,     0,     0,     0,    63,    64,    65,    66,     0,
       0,    67,     0,    68,     0,    69,     0,     0,     0,     0,
      70,    71,     0,    72,     0,     0,     0,    73,    74,     0,
       0,    75,    76,    77,    78,     0,    79,    80,     0,     0,
      81,     0,     0,     0,    82,    83,    84,    85,    86,     0,
      87,     0,     0,    88,     0,     0,     0,    89,    90,    91,
       0,    92,    93,    94,    95,    96,     0,    97,     0,    98,
      99,   100,     0,     0,     0,   101,   102,     0,     0,   103,
     104,   105,     0,     0,     0,   106,   107,   108,     0,   109,
       0,    41,   449,     0,   110,     0,   111,   112,     0,     0,
       0,   113,   114,   115,     0,   116,     0,   117,     0,    42,
     118,    43,    44,     0,    45,    46,     0,     0,     0,     0,
       0,    47,    48,    49,    50,     0,     0,    51,    52,     0,
       0,     0,     0,     0,     0,     0,    53,    54,    55,     0,
       0,    56,     0,     0,     0,    57,    58,     0,    59,     0,
       0,    60,    61,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    62,     0,     0,     0,    63,    64,    65,    66,
       0,     0,    67,     0,    68,     0,    69,     0,     0,     0,
       0,    70,    71,     0,    72,     0,     0,     0,    73,    74,
       0,     0,    75,    76,    77,    78,     0,    79,    80,     0,
       0,    81,     0,     0,     0,    82,    83,    84,    85,    86,
       0,    87,     0,     0,    88,     0,     0,     0,    89,    90,
      91,     0,    92,    93,    94,    95,    96,     0,    97,     0,
      98,    99,   100,     0,     0,     0,   101,   102,     0,     0,
     103,   104,   105,     0,     0,     0,   106,   107,   108,    41,
     109,     0,     0,     0,     0,   110,     0,   111,   112,     0,
       0,   749,   113,   114,   115,     0,   116,    42,   117,    43,
      44,   118,    45,    46,     0,     0,     0,     0,     0,    47,
      48,    49,    50,     0,     0,    51,    52,     0,     0,     0,
       0,     0,     0,     0,    53,    54,    55,     0,     0,    56,
       0,     0,     0,    57,    58,     0,    59,     0,     0,    60,
      61,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,    63,    64,    65,    66,     0,     0,
      67,     0,    68,     0,    69,     0,     0,     0,     0,    70,
      71,     0,    72,     0,     0,     0,    73,    74,     0,     0,
      75,    76,    77,    78,     0,    79,    80,     0,     0,    81,
       0,     0,     0,    82,    83,    84,    85,    86,     0,    87,
       0,     0,    88,     0,     0,     0,    89,    90,    91,     0,
      92,    93,    94,    95,    96,     0,    97,     0,    98,    99,
     100,     0,     0,     0,   101,   102,     0,     0,   103,   104,
     105,     0,     0,     0,   106,   107,   108,    41,   109,     0,
       0,     0,     0,   110,     0,   111,   112,     0,     0,     0,
     113,   114,   115,     0,   116,    42,   117,    43,    44,   118,
      45,    46,     0,     0,     0,     0,     0,    47,    48,    49,
      50,     0,     0,    51,    52,     0,     0,     0,     0,     0,
       0,     0,    53,    54,    55,     0,     0,    56,     0,     0,
       0,    57,    58,     0,    59,     0,     0,    60,    61,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    62,     0,
       0,     0,    63,    64,    65,    66,     0,     0,    67,     0,
      68,     0,    69,     0,     0,     0,     0,    70,    71,     0,
      72,     0,     0,     0,    73,    74,     0,     0,    75,    76,
      77,    78,     0,    79,    80,     0,     0,    81,     0,     0,
       0,    82,    83,    84,    85,    86,     0,    87,     0,     0,
      88,     0,     0,     0,    89,    90,    91,     0,    92,    93,
      94,    95,    96,     0,    97,     0,    98,    99,   100,     0,
       0,     0,   101,   102,     0,     0,   103,   104,   105,     0,
       0,     0,   106,   107,   108,    41,   109,     0,     0,     0,
       0,   110,     0,   111,   112,     0,     0,     0,   113,   114,
     115,     0,   116,    42,   117,    43,    44,   118,    45,    46,
       0,     0,     0,     0,     0,    47,    48,    49,    50,     0,
       0,    51,    52,     0,     0,     0,     0,     0,     0,     0,
      53,    54,    55,     0,     0,    56,     0,     0,     0,    57,
      58,     0,    59,     0,     0,    60,    61,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    62,     0,     0,     0,
      63,    64,    65,    66,     0,     0,    67,     0,    68,     0,
      69,     0,     0,     0,     0,    70,    71,     0,    72,     0,
       0,     0,    73,    74,     0,     0,    75,    76,    77,    78,
       0,    79,    80,     0,     0,    81,     0,     0,     0,    82,
      83,    84,    85,    86,     0,    87,     0,     0,    88,     0,
       0,     0,    89,    90,    91,     0,    92,    93,    94,   142,
      96,     0,    97,     0,    98,    99,   100,     0,     0,     0,
     101,   102,     0,     0,   103,   104,   105,     0,     0,     0,
     106,   107,   108,   231,   109,     0,     0,     0,     0,   110,
       0,   111,   112,     0,     0,     0,   113,   114,   115,     0,
     116,    42,   117,    43,    44,   118,    45,    46,     0,     0,
       0,     0,     0,    47,    48,    49,    50,     0,     0,    51,
      52,     0,     0,     0,     0,     0,     0,     0,    53,    54,
      55,     0,     0,    56,     0,     0,     0,    57,    58,     0,
      59,     0,     0,    60,    61,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    62,     0,     0,     0,    63,    64,
      65,    66,     0,     0,    67,     0,    68,     0,    69,     0,
       0,     0,     0,    70,    71,     0,    72,     0,     0,     0,
      73,    74,     0,     0,    75,    76,    77,    78,     0,    79,
      80,     0,     0,    81,     0,     0,     0,    82,    83,    84,
      85,    86,     0,    87,     0,     0,    88,     0,     0,     0,
      89,    90,    91,     0,    92,    93,    94,    95,    96,     0,
      97,     0,    98,    99,   100,     0,     0,     0,   101,   102,
       0,   179,   103,   104,   105,     0,     0,     0,   106,   107,
     108,     0,   109,     0,     0,     0,     0,   110,     0,   111,
     112,    43,    44,     0,   113,   114,   115,     0,   116,     0,
     117,    47,     0,   118,     0,     0,     0,    51,    52,     0,
       0,     0,     0,     0,     0,     0,    53,    54,    55,     0,
       0,    56,     0,     0,     0,     0,    58,     0,     0,     0,
       0,    60,    61,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    62,     0,     0,     0,    63,     0,    65,    66,
       0,     0,    67,     0,    68,     0,    69,     0,     0,     0,
       0,     0,     0,     0,    72,     0,     0,     0,    73,    74,
       0,     0,    75,    76,    77,    78,     0,    79,    80,     0,
       0,     0,     0,     0,     0,    82,     0,    84,    85,    86,
       0,    87,     0,     0,    88,     0,     0,     0,    89,    90,
      91,     0,    92,    93,     0,    95,    96,     0,    97,     0,
      98,    99,   100,     0,     0,     0,   101,     0,     0,     0,
     103,   104,   105,     0,     0,     0,   106,     0,     0,     0,
     109,     0,     0,     0,     0,     0,     0,   111,   112,     0,
       0,     0,   113,   114,   115,     0,     0,     0,   117,     0,
       0,   118
};

static const yytype_int16 yycheck[] =
{
       2,   446,   172,    15,     2,   147,   361,   429,   155,   156,
     157,    13,     0,     1,   239,   307,   580,   344,   377,   248,
     248,   585,   378,   219,   383,   221,    16,    16,   387,    16,
     256,   257,    15,   455,    16,    16,    16,   393,   156,    16,
      19,    16,   250,    15,    38,    16,   307,    41,    38,    38,
      16,    41,    41,   579,    16,    22,     2,    38,    38,    38,
      41,    41,    41,    38,    19,    18,    41,    13,   427,    16,
     177,    38,    38,    16,    41,    41,    38,    16,    16,    41,
     439,    16,    16,    38,    16,    16,    41,    16,    16,    82,
      16,   320,   320,    82,    34,    35,    23,    15,     8,    21,
      16,    25,    38,    54,     8,    41,    42,    15,    12,    15,
       6,    38,    65,     9,    41,    38,    15,    38,    41,    68,
      41,    73,   133,    35,    15,    38,   326,   129,    41,    41,
     141,   129,    15,   156,    85,    73,   151,    64,     3,   141,
     142,   341,    15,   141,   369,   147,   148,    15,    71,     6,
     148,     4,     9,   353,   354,    15,    15,    95,    71,   161,
     686,   173,   174,   151,   115,   167,    90,   169,   170,   167,
      61,   169,   170,   175,   176,    11,   405,   405,   537,   103,
      11,     5,   443,   129,   413,   413,     0,   395,   110,   161,
     173,   174,   194,   187,   187,   141,   142,   187,   424,   193,
     187,   147,   148,   193,   193,   187,   116,   158,   124,   434,
     187,   163,   193,   193,   193,   161,   187,   446,   193,   361,
     116,   167,   171,   169,   170,   163,   193,   193,   101,   175,
     176,   193,    21,   101,   138,    80,   378,   145,   193,   145,
     187,   101,   101,    36,   187,   690,    31,    40,   187,   694,
      80,   393,   187,   187,   145,   187,   187,   193,   187,   187,
     682,   187,   145,   181,   182,   187,   193,   173,   468,   168,
     193,   471,   193,   124,   601,   166,    42,   604,    44,   701,
     193,   105,   173,   166,   675,   676,   131,   487,     5,   514,
     173,    70,   647,    10,    16,   168,    58,   299,    15,    21,
     168,   131,   658,   184,   631,   448,   157,   125,   168,   168,
      16,   313,    95,   456,    15,   544,   544,   173,   174,   307,
      25,   110,   108,   190,   191,   770,    88,   120,    58,    17,
     894,   895,    82,    38,   173,   174,    41,    42,    43,    44,
      45,    46,    47,    89,   346,   107,    97,   109,   346,    42,
     579,     6,     7,   299,   181,   182,   358,   748,    88,   361,
     358,   187,   228,   229,   695,   696,    15,   313,     4,     5,
       6,     7,     8,    30,    10,    11,   378,   107,    13,   109,
      77,   369,   144,   147,    22,    90,   101,   809,   110,    25,
      15,   393,   719,    98,    15,   817,    15,   689,   103,    15,
     346,   132,   188,    15,    15,    15,    15,    15,   194,   195,
     196,   116,   358,   175,   144,   361,    15,    41,    54,    17,
      15,    15,    15,    52,    15,    61,     2,   429,   430,    15,
     432,    15,   378,    15,   436,   437,    15,    13,   436,    22,
     795,    15,    15,    15,   446,   175,   124,   393,   450,   437,
     646,    19,   157,   455,    83,   443,   444,   686,   460,    98,
     189,   690,    91,    15,    15,   694,    15,    15,    15,   172,
      99,    17,    15,   102,   187,    17,   478,    16,   119,    17,
       5,    15,   119,   429,   430,   844,   432,   192,   193,   119,
     436,   437,    15,   122,     5,     5,     5,     5,   500,    15,
     446,   161,   500,    15,   450,   647,   119,     5,   159,   455,
     139,    25,     5,   159,   460,     5,   658,   159,   187,   159,
      15,   147,    16,   171,    38,     5,   162,    41,    42,    43,
      44,    45,    46,    47,    15,   764,    42,    42,    16,    36,
     193,   770,    17,    40,   166,    15,     5,    16,    16,   774,
      15,    52,    16,   129,   500,     4,     5,     6,     7,     8,
     187,    10,    11,    15,     5,   141,   142,     5,   570,   571,
      16,   147,   148,    16,     5,    15,    90,   579,   580,   871,
     872,    15,    83,   585,    98,   161,   119,    84,   813,   103,
      91,   167,    16,   169,   170,    15,   176,    16,    99,   175,
     176,   102,   116,   100,    52,    54,     2,   104,    16,    16,
     176,   836,    61,    16,   176,   176,    16,    13,    16,     5,
      17,   122,   186,   120,   570,   571,   773,    16,    16,     8,
      17,    16,   128,   579,   580,    83,    17,   187,   139,   585,
      85,    17,   187,    91,     5,   647,    16,    16,     5,     5,
      15,    99,    16,   795,   102,     5,   658,     4,     5,     6,
       7,     8,    16,    10,    11,    16,    15,     5,    16,     5,
     143,    16,    16,   186,    69,    30,   678,    99,   192,   193,
     682,    99,    87,   685,   686,   127,    16,   857,   690,    16,
      16,   139,   694,   863,     5,    16,    16,     5,   686,   701,
     161,   647,    24,    16,    16,   159,    16,    16,   159,   159,
      17,    17,   658,   162,    61,   159,    15,    15,   176,   889,
      17,    16,   724,   299,    16,    16,   724,     5,    50,   176,
      16,    16,   678,   129,    30,   129,   682,   313,    17,   685,
     686,   176,    81,   176,   690,   141,   142,   749,   694,    81,
     187,    15,   148,    90,   131,   701,   148,    90,   140,   131,
      74,    19,   764,    16,    86,   161,    16,   140,   770,   392,
     346,   167,   362,   169,   170,   171,   151,   148,   724,   175,
     176,   443,   358,   105,   106,   361,   126,     2,   595,   861,
      39,   230,   852,   795,   829,   505,   142,   175,    13,   764,
     447,   698,   378,   749,   698,   394,   412,   809,   810,   423,
     417,   544,   531,   515,   811,   817,   749,   393,   764,   857,
     889,    15,   613,   165,   770,    -1,    -1,    -1,    -1,    -1,
     152,   153,   154,   450,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,   846,   847,   848,   169,   170,   795,
      -1,    -1,    -1,   429,   430,    -1,   432,    51,    -1,    -1,
     436,   437,    -1,   809,   810,   811,    -1,    -1,    62,    -1,
     446,   817,    -1,    67,   450,    -1,    -1,    -1,    -1,   455,
      -1,    75,    76,    -1,   460,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   894,   895,    -1,    -1,    -1,    -1,    92,    -1,
     846,   847,   848,   299,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   129,    -1,    -1,   313,    -1,    -1,
      -1,    -1,    -1,    -1,   500,    -1,   141,   142,    -1,    -1,
      -1,    -1,   147,   148,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   161,    -1,   894,   895,
     346,   145,   167,   147,   169,   170,    -1,   151,    -1,    -1,
     175,   176,   358,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     164,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   173,
      -1,    -1,    -1,    -1,    -1,   179,    -1,    -1,    -1,   183,
      -1,   185,    -1,    -1,   570,   571,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   579,   580,    -1,    -1,    -1,    -1,   585,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   429,   430,    -1,   432,    -1,    -1,    -1,
     436,   437,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     446,   447,    -1,    -1,   450,    -1,    -1,    -1,    -1,   455,
      -1,    -1,    -1,    -1,   460,    -1,    -1,    -1,    -1,    -1,
      -1,   647,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   658,    -1,   299,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   313,    -1,
      -1,    -1,   678,    -1,   500,    -1,   682,    -1,    -1,   685,
     686,    -1,    -1,    -1,   690,    -1,    -1,    -1,   694,    -1,
      -1,    -1,    -1,    -1,    -1,   701,    -1,    -1,     2,    -1,
      -1,   346,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    13,
      -1,    -1,    -1,   358,    -1,    -1,   361,    -1,   724,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   378,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   749,   570,   571,    -1,    -1,   393,    -1,
      -1,    -1,    -1,   579,   580,    -1,    -1,    -1,   764,   585,
      -1,    -1,    -1,    -1,   770,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   430,    -1,   432,    -1,   795,
      -1,   436,   437,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   446,    -1,   809,   810,   450,    -1,    -1,    -1,    -1,
      -1,   817,    -1,    -1,    -1,   460,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   129,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,    -1,
     846,   847,   848,    -1,   148,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   678,    -1,    -1,   500,   682,   161,    -1,   685,
     686,    -1,   160,   167,   690,   169,   170,    -1,   694,    -1,
      -1,   175,   176,   171,    -1,   701,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   894,   895,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   724,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   749,    -1,   570,   571,    -1,    -1,   227,
      -1,    -1,    -1,    -1,   579,   580,    -1,    -1,   764,    -1,
     585,   239,    -1,   241,   770,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   250,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   809,   810,    -1,    -1,    -1,    -1,    -1,
      -1,   817,    -1,    -1,    -1,   299,    -1,    -1,    -1,    -1,
      -1,    -1,   647,    -1,    -1,    -1,    -1,    -1,    -1,   313,
      -1,    -1,    -1,   658,    -1,    -1,    -1,    -1,   316,    -1,
     846,   847,   848,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   678,    -1,    -1,    -1,    -1,    -1,    -1,
     685,   686,   346,    -1,    -1,   690,    -1,    -1,    -1,   694,
      -1,    -1,    -1,    -1,   358,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   362,    -1,    -1,    -1,   894,   895,
      -1,   369,    -1,    -1,    -1,    -1,    -1,    -1,   376,   724,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   386,    -1,
     388,    -1,    -1,    -1,   392,    -1,   394,   395,    -1,   397,
      -1,    -1,    -1,    -1,   749,    -1,    -1,    -1,    -1,   407,
      -1,    -1,    -1,   411,    -1,    -1,    -1,    -1,    -1,   764,
      -1,    -1,    -1,    -1,    -1,   770,   430,   425,   432,    -1,
     428,    -1,   436,   437,    -1,    -1,   434,    -1,    -1,    -1,
      -1,    -1,   446,    -1,    -1,    -1,   450,    -1,    -1,   447,
     795,    -1,    -1,    -1,    -1,    -1,   460,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   810,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   500,    -1,    -1,    -1,
      -1,   846,   847,   848,    -1,    -1,    -1,   505,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   513,   514,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   543,    -1,   545,    -1,   894,
     895,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   562,    -1,   570,   571,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   579,   580,    -1,    -1,    -1,
      -1,   585,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   595,   596,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   655,    -1,    -1,
      -1,    -1,   660,    -1,    -1,   663,    -1,    -1,   666,    -1,
     668,    -1,    -1,   671,   678,    -1,    -1,    -1,    -1,    -1,
      -1,   685,   686,    -1,    -1,    -1,   690,    -1,    -1,    -1,
     694,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     724,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   733,   734,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   749,   744,   745,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     764,    -1,    -1,    -1,    -1,    -1,   770,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   774,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   810,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   813,    -1,   815,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   836,    -1,
      -1,    -1,   846,   847,   848,    -1,    -1,     3,     4,    -1,
      -1,    -1,    -1,    -1,   852,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   861,    -1,    21,    22,    23,    24,    -1,
      26,    27,    -1,    -1,    -1,    -1,    -1,    33,    34,    35,
      36,    -1,    38,    39,    40,    41,    -1,    -1,    -1,    -1,
     894,   895,    48,    49,    50,    -1,    -1,    53,    -1,    -1,
      -1,    57,    58,    -1,    60,    -1,    -1,    63,    64,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    74,    -1,
      -1,    -1,    78,    79,    80,    81,    -1,    -1,    84,    -1,
      86,    -1,    88,    -1,    -1,    -1,    -1,    93,    94,    -1,
      96,    -1,    -1,    -1,   100,   101,    -1,    -1,   104,   105,
     106,   107,    -1,   109,   110,    -1,    -1,   113,    -1,    -1,
      -1,   117,   118,   119,   120,   121,    -1,   123,    -1,    -1,
     126,    -1,    -1,    -1,   130,   131,   132,    -1,   134,   135,
     136,   137,   138,    -1,   140,    -1,   142,   143,   144,    -1,
      -1,    -1,   148,   149,    -1,    -1,   152,   153,   154,    -1,
      -1,    -1,   158,   159,   160,    -1,   162,    -1,    -1,    -1,
      -1,   167,    -1,   169,   170,    -1,    -1,    -1,   174,   175,
     176,    -1,   178,    -1,   180,    -1,    -1,   183,     3,     4,
       5,     6,     7,     8,    -1,    10,    11,   193,    -1,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    21,    -1,    23,    24,
      -1,    26,    27,    -1,    -1,    -1,    -1,    32,    33,    34,
      35,    36,    -1,    -1,    39,    40,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    48,    49,    50,    -1,    -1,    53,    -1,
      55,    56,    57,    58,    -1,    60,    61,    -1,    63,    64,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    74,
      -1,    -1,    77,    78,    79,    80,    81,    -1,    -1,    84,
      -1,    86,    -1,    88,    -1,    -1,    -1,    -1,    93,    94,
      -1,    96,    -1,    -1,    -1,   100,   101,   102,    -1,   104,
     105,   106,   107,    -1,   109,   110,    -1,    -1,   113,    -1,
      -1,   116,   117,   118,   119,   120,   121,    -1,   123,    -1,
      -1,   126,    -1,    -1,    -1,   130,   131,   132,    -1,   134,
     135,   136,   137,   138,   139,   140,    -1,   142,   143,   144,
     145,   146,    -1,   148,   149,    -1,    -1,   152,   153,   154,
     155,    -1,    -1,   158,   159,   160,    -1,   162,    -1,    -1,
      -1,    -1,   167,    -1,   169,   170,    -1,    -1,   173,   174,
     175,   176,    -1,   178,    -1,   180,    -1,    -1,   183,    -1,
      -1,    -1,    -1,    -1,    -1,   190,   191,     3,     4,     5,
       6,     7,     8,    -1,    10,    11,    -1,    -1,    -1,    15,
      16,    -1,    -1,    -1,    -1,    21,    -1,    23,    24,    -1,
      26,    27,    -1,    -1,    -1,    -1,    32,    33,    34,    35,
      36,    -1,    -1,    39,    40,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    48,    49,    50,    -1,    -1,    53,    -1,    55,
      56,    57,    58,    -1,    60,    61,    -1,    63,    64,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    74,    -1,
      -1,    77,    78,    79,    80,    81,    -1,    -1,    84,    -1,
      86,    -1,    88,    -1,    -1,    -1,    -1,    93,    94,    -1,
      96,    -1,    -1,    -1,   100,   101,   102,    -1,   104,   105,
     106,   107,    -1,   109,   110,    -1,    -1,   113,    -1,    -1,
     116,   117,   118,   119,   120,   121,    -1,   123,    -1,    -1,
     126,    -1,    -1,    -1,   130,   131,   132,    -1,   134,   135,
     136,   137,   138,   139,   140,    -1,   142,   143,   144,    -1,
     146,    -1,   148,   149,    -1,    -1,   152,   153,   154,   155,
      -1,    -1,   158,   159,   160,    -1,   162,    -1,    -1,    -1,
      -1,   167,    -1,   169,   170,    -1,    -1,    -1,   174,   175,
     176,    -1,   178,    -1,   180,    -1,    -1,   183,    -1,    -1,
      -1,    -1,    -1,    -1,   190,   191,     3,     4,     5,     6,
       7,     8,    -1,    10,    11,    -1,    -1,    -1,    15,    16,
      -1,    -1,    -1,    -1,    21,    -1,    23,    24,    -1,    26,
      27,    -1,    -1,    -1,    -1,    32,    33,    34,    35,    36,
      -1,    -1,    39,    40,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    48,    49,    50,    -1,    -1,    53,    -1,    55,    56,
      57,    58,    -1,    60,    61,    -1,    63,    64,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    74,    -1,    -1,
      77,    78,    79,    80,    81,    -1,    -1,    84,    -1,    86,
      -1,    88,    -1,    -1,    -1,    -1,    93,    94,    -1,    96,
      -1,    -1,    -1,   100,   101,   102,    -1,   104,   105,   106,
     107,    -1,   109,   110,    -1,    -1,   113,    -1,    -1,   116,
     117,   118,   119,   120,   121,    -1,   123,    -1,    -1,   126,
      -1,    -1,    -1,   130,   131,   132,    -1,   134,   135,   136,
     137,   138,   139,   140,    -1,   142,   143,   144,    -1,   146,
      -1,   148,   149,    -1,    -1,   152,   153,   154,   155,    -1,
      -1,   158,   159,   160,    -1,   162,    -1,    -1,    -1,    -1,
     167,    -1,   169,   170,    -1,    -1,    -1,   174,   175,   176,
      -1,   178,    -1,   180,    -1,    -1,   183,    -1,    -1,    -1,
      -1,    -1,    -1,   190,   191,     3,     4,     5,     6,     7,
       8,    -1,    10,    11,    -1,    -1,    -1,    15,    -1,    -1,
      18,    -1,    20,    21,    -1,    23,    24,    -1,    26,    27,
      -1,    -1,    -1,    -1,    32,    33,    34,    35,    36,    -1,
      -1,    39,    40,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      48,    49,    50,    -1,    -1,    53,    -1,    55,    56,    57,
      58,    -1,    60,    61,    -1,    63,    64,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    74,    -1,    -1,    -1,
      78,    79,    80,    81,    -1,    -1,    84,    -1,    86,    -1,
      88,    -1,    -1,    -1,    -1,    93,    94,    -1,    96,    -1,
      -1,    -1,   100,   101,   102,    -1,   104,   105,   106,   107,
      -1,   109,   110,    -1,    -1,   113,    -1,    -1,    -1,   117,
     118,   119,   120,   121,    -1,   123,    -1,    -1,   126,    -1,
      -1,    -1,   130,   131,   132,    -1,   134,   135,   136,   137,
     138,   139,   140,    -1,   142,   143,   144,    -1,   146,    -1,
     148,   149,   150,    -1,   152,   153,   154,   155,    -1,    -1,
     158,   159,   160,    -1,   162,    -1,    -1,    -1,    -1,   167,
      -1,   169,   170,    -1,    -1,    -1,   174,   175,   176,    -1,
     178,    -1,   180,    -1,    -1,   183,    -1,    -1,    -1,    -1,
      -1,    -1,   190,   191,     3,     4,     5,     6,     7,     8,
      -1,    10,    11,    -1,    -1,    -1,    15,    -1,    -1,    18,
      -1,    -1,    21,    -1,    23,    24,    -1,    26,    27,    -1,
      -1,    -1,    -1,    32,    33,    34,    35,    36,    -1,    -1,
      39,    40,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    48,
      49,    50,    -1,    -1,    53,    -1,    55,    56,    57,    58,
      -1,    60,    61,    -1,    63,    64,    65,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    74,    -1,    -1,    -1,    78,
      79,    80,    81,    -1,    -1,    84,    -1,    86,    -1,    88,
      -1,    -1,    -1,    -1,    93,    94,    -1,    96,    -1,    -1,
      -1,   100,   101,   102,    -1,   104,   105,   106,   107,    -1,
     109,   110,    -1,    -1,   113,    -1,    -1,    -1,   117,   118,
     119,   120,   121,    -1,   123,    -1,    -1,   126,    -1,    -1,
      -1,   130,   131,   132,    -1,   134,   135,   136,   137,   138,
     139,   140,    -1,   142,   143,   144,    -1,   146,    -1,   148,
     149,    -1,    -1,   152,   153,   154,   155,    -1,    -1,   158,
     159,   160,    -1,   162,    -1,    -1,    -1,    -1,   167,    -1,
     169,   170,    -1,    -1,    -1,   174,   175,   176,    -1,   178,
      -1,   180,    -1,    -1,   183,    -1,    -1,    -1,    -1,   188,
      -1,   190,   191,     3,     4,     5,     6,     7,     8,    -1,
      10,    11,    -1,    -1,    -1,    15,    16,    -1,    -1,    -1,
      -1,    21,    -1,    23,    24,    -1,    26,    27,    -1,    -1,
      -1,    -1,    32,    33,    34,    35,    36,    -1,    -1,    39,
      40,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    48,    49,
      50,    -1,    -1,    53,    -1,    55,    56,    57,    58,    -1,
      60,    61,    -1,    63,    64,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    74,    -1,    -1,    -1,    78,    79,
      80,    81,    -1,    -1,    84,    -1,    86,    -1,    88,    -1,
      -1,    -1,    -1,    93,    94,    -1,    96,    -1,    -1,    -1,
     100,   101,   102,    -1,   104,   105,   106,   107,    -1,   109,
     110,    -1,    -1,   113,    -1,    -1,    -1,   117,   118,   119,
     120,   121,    -1,   123,    -1,    -1,   126,    -1,    -1,    -1,
     130,   131,   132,    -1,   134,   135,   136,   137,   138,   139,
     140,    -1,   142,   143,   144,   145,   146,    -1,   148,   149,
      -1,    -1,   152,   153,   154,   155,    -1,    -1,   158,   159,
     160,    -1,   162,    -1,    -1,    -1,    -1,   167,    -1,   169,
     170,    -1,    -1,   173,   174,   175,   176,    -1,   178,    -1,
     180,    -1,    -1,   183,    -1,    -1,    -1,    -1,    -1,    -1,
     190,   191,     3,     4,     5,     6,     7,     8,    -1,    10,
      11,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      21,    -1,    23,    24,    -1,    26,    27,    -1,    -1,    -1,
      -1,    32,    33,    34,    35,    36,    -1,    -1,    39,    40,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    48,    49,    50,
      -1,    -1,    53,    -1,    55,    56,    57,    58,    -1,    60,
      61,    -1,    63,    64,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    74,    -1,    -1,    77,    78,    79,    80,
      81,    -1,    -1,    84,    -1,    86,    -1,    88,    -1,    -1,
      -1,    -1,    93,    94,    -1,    96,    -1,    -1,    -1,   100,
     101,   102,    -1,   104,   105,   106,   107,    -1,   109,   110,
      -1,    -1,   113,    -1,    -1,   116,   117,   118,   119,   120,
     121,    -1,   123,    -1,    -1,   126,    -1,    -1,    -1,   130,
     131,   132,    -1,   134,   135,   136,   137,   138,   139,   140,
      -1,   142,   143,   144,    -1,   146,    -1,   148,   149,    -1,
      -1,   152,   153,   154,   155,    -1,    -1,   158,   159,   160,
      -1,   162,    -1,    -1,    -1,    -1,   167,    -1,   169,   170,
      -1,    -1,    -1,   174,   175,   176,    -1,   178,    -1,   180,
      -1,    -1,   183,    -1,    -1,    -1,    -1,    -1,    -1,   190,
     191,     3,     4,     5,     6,     7,     8,    -1,    10,    11,
      -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    21,
      -1,    23,    24,    -1,    26,    27,    -1,    -1,    -1,    -1,
      32,    33,    34,    35,    36,    -1,    -1,    39,    40,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    48,    49,    50,    -1,
      -1,    53,    -1,    55,    56,    57,    58,    -1,    60,    61,
      -1,    63,    64,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    74,    -1,    -1,    -1,    78,    79,    80,    81,
      -1,    -1,    84,    -1,    86,    -1,    88,    -1,    -1,    -1,
      -1,    93,    94,    -1,    96,    -1,    -1,    -1,   100,   101,
     102,    -1,   104,   105,   106,   107,    -1,   109,   110,    -1,
      -1,   113,    -1,    -1,    -1,   117,   118,   119,   120,   121,
      -1,   123,    -1,    -1,   126,    -1,    -1,    -1,   130,   131,
     132,    -1,   134,   135,   136,   137,   138,   139,   140,    -1,
     142,   143,   144,   145,   146,    -1,   148,   149,    -1,    -1,
     152,   153,   154,   155,    -1,    -1,   158,   159,   160,    -1,
     162,    -1,    -1,    -1,    -1,   167,    -1,   169,   170,    -1,
      -1,   173,   174,   175,   176,    -1,   178,    -1,   180,    -1,
      -1,   183,    -1,    -1,    -1,    -1,    -1,    -1,   190,   191,
       3,     4,     5,     6,     7,     8,    -1,    10,    11,    -1,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    21,    -1,
      23,    24,    -1,    26,    27,    -1,    -1,    -1,    -1,    32,
      33,    34,    35,    36,    -1,    -1,    39,    40,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    48,    49,    50,    -1,    -1,
      53,    -1,    55,    56,    57,    58,    -1,    60,    61,    -1,
      63,    64,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    74,    -1,    -1,    -1,    78,    79,    80,    81,    -1,
      -1,    84,    -1,    86,    -1,    88,    -1,    -1,    -1,    -1,
      93,    94,    -1,    96,    -1,    -1,    -1,   100,   101,   102,
      -1,   104,   105,   106,   107,    -1,   109,   110,    -1,    -1,
     113,    -1,    -1,    -1,   117,   118,   119,   120,   121,    -1,
     123,    -1,    -1,   126,    -1,    -1,    -1,   130,   131,   132,
      -1,   134,   135,   136,   137,   138,   139,   140,    -1,   142,
     143,   144,    -1,   146,    -1,   148,   149,    -1,    -1,   152,
     153,   154,   155,    -1,    -1,   158,   159,   160,    -1,   162,
      -1,    -1,    -1,    -1,   167,    -1,   169,   170,    -1,    -1,
      -1,   174,   175,   176,    -1,   178,    -1,   180,    -1,    -1,
     183,    -1,    -1,    -1,    -1,   188,    -1,   190,   191,     3,
       4,     5,     6,     7,     8,    -1,    10,    11,    -1,    -1,
      -1,    15,    16,    -1,    -1,    -1,    -1,    21,    -1,    23,
      24,    -1,    26,    27,    -1,    -1,    -1,    -1,    32,    33,
      34,    35,    36,    -1,    -1,    39,    40,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    48,    49,    50,    -1,    -1,    53,
      -1,    55,    56,    57,    58,    -1,    60,    61,    -1,    63,
      64,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      74,    -1,    -1,    -1,    78,    79,    80,    81,    -1,    -1,
      84,    -1,    86,    -1,    88,    -1,    -1,    -1,    -1,    93,
      94,    -1,    96,    -1,    -1,    -1,   100,   101,   102,    -1,
     104,   105,   106,   107,    -1,   109,   110,    -1,    -1,   113,
      -1,    -1,    -1,   117,   118,   119,   120,   121,    -1,   123,
      -1,    -1,   126,    -1,    -1,    -1,   130,   131,   132,    -1,
     134,   135,   136,   137,   138,   139,   140,    -1,   142,   143,
     144,    -1,   146,    -1,   148,   149,    -1,    -1,   152,   153,
     154,   155,    -1,    -1,   158,   159,   160,    -1,   162,    -1,
      -1,    -1,    -1,   167,    -1,   169,   170,    -1,    -1,    -1,
     174,   175,   176,    -1,   178,    -1,   180,    -1,    -1,   183,
      -1,    -1,    -1,    -1,    -1,    -1,   190,   191,     3,     4,
       5,     6,     7,     8,    -1,    10,    11,    -1,    -1,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    21,    -1,    23,    24,
      -1,    26,    27,    -1,    -1,    -1,    -1,    32,    33,    34,
      35,    36,    -1,    -1,    39,    40,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    48,    49,    50,    -1,    -1,    53,    -1,
      55,    56,    57,    58,    -1,    60,    61,    -1,    63,    64,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    74,
      -1,    -1,    -1,    78,    79,    80,    81,    -1,    -1,    84,
      -1,    86,    -1,    88,    -1,    -1,    -1,    -1,    93,    94,
      -1,    96,    -1,    -1,    -1,   100,   101,   102,    -1,   104,
     105,   106,   107,    -1,   109,   110,    -1,    -1,   113,    -1,
      -1,    -1,   117,   118,   119,   120,   121,    -1,   123,    -1,
      -1,   126,    -1,    -1,    -1,   130,   131,   132,    -1,   134,
     135,   136,   137,   138,   139,   140,    -1,   142,   143,   144,
      -1,   146,    -1,   148,   149,    -1,    -1,   152,   153,   154,
     155,    -1,    -1,   158,   159,   160,    -1,   162,    -1,    -1,
      -1,    -1,   167,    -1,   169,   170,    -1,    -1,    -1,   174,
     175,   176,    -1,   178,    -1,   180,    -1,    -1,   183,    -1,
      -1,    -1,    -1,    -1,    -1,   190,   191,     3,     4,     5,
       6,     7,     8,    -1,    10,    11,    -1,    -1,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    21,    -1,    23,    24,    -1,
      26,    27,    -1,    -1,    -1,    -1,    32,    33,    34,    35,
      36,    -1,    -1,    39,    40,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    48,    49,    50,    -1,    -1,    53,    -1,    55,
      56,    57,    58,    -1,    60,    61,    -1,    63,    64,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    74,    -1,
      -1,    -1,    78,    79,    80,    81,    -1,    -1,    84,    -1,
      86,    -1,    88,    -1,    -1,    -1,    -1,    93,    94,    -1,
      96,    -1,    -1,    -1,   100,   101,   102,    -1,   104,   105,
     106,   107,    -1,   109,   110,    -1,    -1,   113,    -1,    -1,
      -1,   117,   118,   119,   120,   121,    -1,   123,    -1,    -1,
     126,    -1,    -1,    -1,   130,   131,   132,    -1,   134,   135,
     136,   137,   138,   139,   140,    -1,   142,   143,   144,    -1,
     146,    -1,   148,   149,    -1,    -1,   152,   153,   154,   155,
      -1,    -1,   158,   159,   160,    -1,   162,    -1,     3,    -1,
      -1,   167,    -1,   169,   170,    -1,    -1,    -1,   174,   175,
     176,    -1,   178,    -1,   180,    -1,    21,   183,    23,    24,
      -1,    26,    27,    -1,   190,   191,    -1,    -1,    33,    34,
      35,    36,    -1,    -1,    39,    40,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    48,    49,    50,    -1,    -1,    53,    -1,
      -1,    -1,    57,    58,    -1,    60,    -1,    -1,    63,    64,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    74,
      -1,    -1,    -1,    78,    79,    80,    81,    -1,    -1,    84,
      -1,    86,    -1,    88,    -1,    -1,    -1,    -1,    93,    94,
      -1,    96,    -1,    -1,    -1,   100,   101,    -1,    -1,   104,
     105,   106,   107,    -1,   109,   110,    -1,    -1,   113,    -1,
      -1,    -1,   117,   118,   119,   120,   121,    -1,   123,    -1,
      -1,   126,    -1,    -1,    -1,   130,   131,   132,    -1,   134,
     135,   136,   137,   138,    -1,   140,    -1,   142,   143,   144,
      -1,    -1,    -1,   148,   149,    -1,    -1,   152,   153,   154,
      -1,    -1,    -1,   158,   159,   160,     3,   162,    -1,    -1,
      -1,    -1,   167,    -1,   169,   170,    -1,    -1,    -1,   174,
     175,   176,    -1,   178,    21,   180,    23,    24,   183,    26,
      27,    -1,    -1,   188,    -1,    -1,    33,    34,    35,    36,
      -1,    -1,    39,    40,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    48,    49,    50,    -1,    -1,    53,    -1,    -1,    -1,
      57,    58,    -1,    60,    -1,    -1,    63,    64,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    74,    -1,    -1,
      -1,    78,    79,    80,    81,    -1,    -1,    84,    -1,    86,
      -1,    88,    -1,    -1,    -1,    -1,    93,    94,    -1,    96,
      -1,    -1,    -1,   100,   101,    -1,    -1,   104,   105,   106,
     107,    -1,   109,   110,    -1,    -1,   113,    -1,    -1,    -1,
     117,   118,   119,   120,   121,    -1,   123,    -1,    -1,   126,
      -1,    -1,    -1,   130,   131,   132,    -1,   134,   135,   136,
     137,   138,    -1,   140,    -1,   142,   143,   144,    -1,    -1,
      -1,   148,   149,    -1,    -1,   152,   153,   154,    -1,    -1,
      -1,   158,   159,   160,     3,   162,    -1,    -1,    -1,    -1,
     167,    -1,   169,   170,    -1,    -1,    -1,   174,   175,   176,
      -1,   178,    21,   180,    23,    24,   183,    26,    27,    -1,
      -1,   188,    -1,    -1,    33,    34,    35,    36,    -1,    -1,
      39,    40,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    48,
      49,    50,    -1,    -1,    53,    -1,    -1,    -1,    57,    58,
      -1,    60,    -1,    -1,    63,    64,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    74,    -1,    -1,    -1,    78,
      79,    80,    81,    -1,    -1,    84,    -1,    86,    -1,    88,
      -1,    -1,    -1,    -1,    93,    94,    -1,    96,    -1,    -1,
      -1,   100,   101,    -1,    -1,   104,   105,   106,   107,    -1,
     109,   110,    -1,    -1,   113,    -1,    -1,    -1,   117,   118,
     119,   120,   121,    -1,   123,    -1,    -1,   126,    -1,    -1,
      -1,   130,   131,   132,    -1,   134,   135,   136,   137,   138,
      -1,   140,    -1,   142,   143,   144,    -1,    -1,    -1,   148,
     149,    -1,    -1,   152,   153,   154,    -1,    -1,    -1,   158,
     159,   160,     3,   162,    -1,    -1,    -1,    -1,   167,    -1,
     169,   170,    -1,    -1,    -1,   174,   175,   176,    -1,   178,
      -1,   180,    23,    24,   183,    26,    27,    28,    29,   188,
      -1,    -1,    33,    34,    35,    -1,    37,    -1,    39,    40,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    48,    49,    50,
      -1,    -1,    53,    -1,    -1,    -1,    57,    58,    59,    60,
      -1,    -1,    63,    64,    -1,    66,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    74,    -1,    -1,    -1,    78,    79,    80,
      81,    -1,    -1,    84,    -1,    86,    -1,    88,    -1,    -1,
      -1,    -1,    93,    94,    -1,    96,    -1,    -1,    -1,   100,
     101,    -1,    -1,   104,   105,   106,   107,    -1,   109,   110,
     111,    -1,   113,   114,    -1,    -1,   117,   118,   119,   120,
     121,    -1,   123,    -1,    -1,   126,    -1,    -1,    -1,   130,
     131,   132,    -1,   134,   135,   136,   137,   138,    -1,   140,
      -1,   142,   143,   144,    -1,    -1,    -1,   148,   149,    -1,
      -1,   152,   153,   154,    -1,    -1,    -1,   158,   159,   160,
       3,   162,    -1,    -1,    -1,    -1,   167,    -1,   169,   170,
      -1,    -1,    15,   174,   175,   176,    -1,   178,    21,   180,
      23,    24,   183,    26,    27,    -1,    -1,    -1,    -1,    -1,
      33,    34,    35,    36,    -1,    -1,    39,    40,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    48,    49,    50,    -1,    -1,
      53,    -1,    -1,    -1,    57,    58,    -1,    60,    -1,    -1,
      63,    64,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    74,    -1,    -1,    -1,    78,    79,    80,    81,    -1,
      -1,    84,    -1,    86,    -1,    88,    -1,    -1,    -1,    -1,
      93,    94,    -1,    96,    -1,    -1,    -1,   100,   101,    -1,
      -1,   104,   105,   106,   107,    -1,   109,   110,    -1,    -1,
     113,    -1,    -1,    -1,   117,   118,   119,   120,   121,    -1,
     123,    -1,    -1,   126,    -1,    -1,    -1,   130,   131,   132,
      -1,   134,   135,   136,   137,   138,    -1,   140,    -1,   142,
     143,   144,   145,    -1,    -1,   148,   149,    -1,    -1,   152,
     153,   154,    -1,    -1,    -1,   158,   159,   160,    -1,   162,
      -1,    -1,    -1,    -1,   167,    -1,   169,   170,    -1,     3,
     173,   174,   175,   176,    -1,   178,    -1,   180,    -1,    -1,
     183,    15,    -1,    -1,    -1,    -1,    -1,    21,    -1,    23,
      24,    -1,    26,    27,    -1,    -1,    -1,    -1,    -1,    33,
      34,    35,    36,    -1,    -1,    39,    40,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    48,    49,    50,    -1,    -1,    53,
      -1,    -1,    -1,    57,    58,    -1,    60,    -1,    -1,    63,
      64,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      74,    -1,    -1,    -1,    78,    79,    80,    81,    -1,    -1,
      84,    -1,    86,    -1,    88,    -1,    -1,    -1,    -1,    93,
      94,    -1,    96,    -1,    -1,    -1,   100,   101,    -1,    -1,
     104,   105,   106,   107,    -1,   109,   110,    -1,    -1,   113,
      -1,    -1,    -1,   117,   118,   119,   120,   121,    -1,   123,
      -1,    -1,   126,    -1,    -1,    -1,   130,   131,   132,    -1,
     134,   135,   136,   137,   138,    -1,   140,    -1,   142,   143,
     144,   145,    -1,    -1,   148,   149,    -1,    -1,   152,   153,
     154,    -1,    -1,    -1,   158,   159,   160,    -1,   162,    -1,
      -1,    -1,    -1,   167,    -1,   169,   170,     3,     4,   173,
     174,   175,   176,    -1,   178,    -1,   180,    -1,    -1,   183,
      -1,    -1,    -1,    -1,    -1,    21,    22,    23,    24,    -1,
      26,    27,    -1,    -1,    -1,    -1,    -1,    33,    34,    35,
      36,    -1,    -1,    39,    40,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    48,    49,    50,    -1,    -1,    53,    -1,    -1,
      -1,    57,    58,    -1,    60,    -1,    -1,    63,    64,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    74,    -1,
      -1,    -1,    78,    79,    80,    81,    -1,    -1,    84,    -1,
      86,    -1,    88,    -1,    -1,    -1,    -1,    93,    94,    -1,
      96,    -1,    -1,    -1,   100,   101,    -1,    -1,   104,   105,
     106,   107,    -1,   109,   110,    -1,    -1,   113,    -1,    -1,
      -1,   117,   118,   119,   120,   121,    -1,   123,    -1,    -1,
     126,    -1,    -1,    -1,   130,   131,   132,    -1,   134,   135,
     136,   137,   138,    -1,   140,    -1,   142,   143,   144,    -1,
      -1,    -1,   148,   149,    -1,    -1,   152,   153,   154,    -1,
      -1,    -1,   158,   159,   160,    -1,   162,    -1,    -1,    -1,
      -1,   167,     3,   169,   170,    -1,    -1,    -1,   174,   175,
     176,    -1,   178,    -1,   180,    -1,    17,   183,    -1,    -1,
      21,    -1,    23,    24,    -1,    26,    27,    -1,    -1,    -1,
      -1,    -1,    33,    34,    35,    36,    -1,    -1,    39,    40,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    48,    49,    50,
      -1,    -1,    53,    -1,    -1,    -1,    57,    58,    -1,    60,
      -1,    -1,    63,    64,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    74,    -1,    -1,    -1,    78,    79,    80,
      81,    -1,    -1,    84,    -1,    86,    -1,    88,    -1,    -1,
      -1,    -1,    93,    94,    -1,    96,    -1,    -1,    -1,   100,
     101,    -1,    -1,   104,   105,   106,   107,    -1,   109,   110,
      -1,    -1,   113,    -1,    -1,    -1,   117,   118,   119,   120,
     121,    -1,   123,    -1,    -1,   126,    -1,    -1,    -1,   130,
     131,   132,    -1,   134,   135,   136,   137,   138,    -1,   140,
      -1,   142,   143,   144,    -1,    -1,    -1,   148,   149,    -1,
      -1,   152,   153,   154,    -1,    -1,    -1,   158,   159,   160,
      -1,   162,    -1,    -1,    -1,    -1,   167,     3,   169,   170,
      -1,    -1,    -1,   174,   175,   176,    -1,   178,    -1,   180,
      -1,    17,   183,    -1,    -1,    21,    -1,    23,    24,    -1,
      26,    27,    -1,    -1,    -1,    -1,    -1,    33,    34,    35,
      36,    -1,    -1,    39,    40,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    48,    49,    50,    -1,    -1,    53,    -1,    -1,
      -1,    57,    58,    -1,    60,    -1,    -1,    63,    64,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    74,    -1,
      -1,    -1,    78,    79,    80,    81,    -1,    -1,    84,    -1,
      86,    -1,    88,    -1,    -1,    -1,    -1,    93,    94,    -1,
      96,    -1,    -1,    -1,   100,   101,    -1,    -1,   104,   105,
     106,   107,    -1,   109,   110,    -1,    -1,   113,    -1,    -1,
      -1,   117,   118,   119,   120,   121,    -1,   123,    -1,    -1,
     126,    -1,    -1,    -1,   130,   131,   132,    -1,   134,   135,
     136,   137,   138,    -1,   140,    -1,   142,   143,   144,    -1,
      -1,    -1,   148,   149,    -1,    -1,   152,   153,   154,    -1,
      -1,    -1,   158,   159,   160,    -1,   162,    -1,    -1,    -1,
      -1,   167,     3,   169,   170,    -1,    -1,    -1,   174,   175,
     176,    -1,   178,    -1,   180,    -1,    17,   183,    -1,    -1,
      21,    -1,    23,    24,    -1,    26,    27,    -1,    -1,    -1,
      -1,    -1,    33,    34,    35,    36,    -1,    -1,    39,    40,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    48,    49,    50,
      -1,    -1,    53,    -1,    -1,    -1,    57,    58,    -1,    60,
      -1,    -1,    63,    64,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    74,    -1,    -1,    -1,    78,    79,    80,
      81,    -1,    -1,    84,    -1,    86,    -1,    88,    -1,    -1,
      -1,    -1,    93,    94,    -1,    96,    -1,    -1,    -1,   100,
     101,    -1,    -1,   104,   105,   106,   107,    -1,   109,   110,
      -1,    -1,   113,    -1,    -1,    -1,   117,   118,   119,   120,
     121,    -1,   123,    -1,    -1,   126,    -1,    -1,    -1,   130,
     131,   132,    -1,   134,   135,   136,   137,   138,    -1,   140,
      -1,   142,   143,   144,    -1,    -1,    -1,   148,   149,    -1,
      -1,   152,   153,   154,    -1,    -1,    -1,   158,   159,   160,
       3,   162,    -1,    -1,    -1,    -1,   167,    -1,   169,   170,
      -1,    -1,    15,   174,   175,   176,    -1,   178,    21,   180,
      23,    24,   183,    26,    27,    -1,    -1,    -1,    -1,    -1,
      33,    34,    35,    36,    -1,    -1,    39,    40,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    48,    49,    50,    -1,    -1,
      53,    -1,    -1,    -1,    57,    58,    -1,    60,    -1,    -1,
      63,    64,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    74,    -1,    -1,    -1,    78,    79,    80,    81,    -1,
      -1,    84,    -1,    86,    -1,    88,    -1,    -1,    -1,    -1,
      93,    94,    -1,    96,    -1,    -1,    -1,   100,   101,    -1,
      -1,   104,   105,   106,   107,    -1,   109,   110,    -1,    -1,
     113,    -1,    -1,    -1,   117,   118,   119,   120,   121,    -1,
     123,    -1,    -1,   126,    -1,    -1,    -1,   130,   131,   132,
      -1,   134,   135,   136,   137,   138,    -1,   140,    -1,   142,
     143,   144,    -1,    -1,    -1,   148,   149,    -1,    -1,   152,
     153,   154,    -1,    -1,    -1,   158,   159,   160,    -1,   162,
      -1,     3,     4,    -1,   167,    -1,   169,   170,    -1,    -1,
      -1,   174,   175,   176,    -1,   178,    -1,   180,    -1,    21,
     183,    23,    24,    -1,    26,    27,    -1,    -1,    -1,    -1,
      -1,    33,    34,    35,    36,    -1,    -1,    39,    40,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    48,    49,    50,    -1,
      -1,    53,    -1,    -1,    -1,    57,    58,    -1,    60,    -1,
      -1,    63,    64,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    74,    -1,    -1,    -1,    78,    79,    80,    81,
      -1,    -1,    84,    -1,    86,    -1,    88,    -1,    -1,    -1,
      -1,    93,    94,    -1,    96,    -1,    -1,    -1,   100,   101,
      -1,    -1,   104,   105,   106,   107,    -1,   109,   110,    -1,
      -1,   113,    -1,    -1,    -1,   117,   118,   119,   120,   121,
      -1,   123,    -1,    -1,   126,    -1,    -1,    -1,   130,   131,
     132,    -1,   134,   135,   136,   137,   138,    -1,   140,    -1,
     142,   143,   144,    -1,    -1,    -1,   148,   149,    -1,    -1,
     152,   153,   154,    -1,    -1,    -1,   158,   159,   160,     3,
     162,    -1,    -1,    -1,    -1,   167,    -1,   169,   170,    -1,
      -1,    15,   174,   175,   176,    -1,   178,    21,   180,    23,
      24,   183,    26,    27,    -1,    -1,    -1,    -1,    -1,    33,
      34,    35,    36,    -1,    -1,    39,    40,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    48,    49,    50,    -1,    -1,    53,
      -1,    -1,    -1,    57,    58,    -1,    60,    -1,    -1,    63,
      64,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      74,    -1,    -1,    -1,    78,    79,    80,    81,    -1,    -1,
      84,    -1,    86,    -1,    88,    -1,    -1,    -1,    -1,    93,
      94,    -1,    96,    -1,    -1,    -1,   100,   101,    -1,    -1,
     104,   105,   106,   107,    -1,   109,   110,    -1,    -1,   113,
      -1,    -1,    -1,   117,   118,   119,   120,   121,    -1,   123,
      -1,    -1,   126,    -1,    -1,    -1,   130,   131,   132,    -1,
     134,   135,   136,   137,   138,    -1,   140,    -1,   142,   143,
     144,    -1,    -1,    -1,   148,   149,    -1,    -1,   152,   153,
     154,    -1,    -1,    -1,   158,   159,   160,     3,   162,    -1,
      -1,    -1,    -1,   167,    -1,   169,   170,    -1,    -1,    -1,
     174,   175,   176,    -1,   178,    21,   180,    23,    24,   183,
      26,    27,    -1,    -1,    -1,    -1,    -1,    33,    34,    35,
      36,    -1,    -1,    39,    40,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    48,    49,    50,    -1,    -1,    53,    -1,    -1,
      -1,    57,    58,    -1,    60,    -1,    -1,    63,    64,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    74,    -1,
      -1,    -1,    78,    79,    80,    81,    -1,    -1,    84,    -1,
      86,    -1,    88,    -1,    -1,    -1,    -1,    93,    94,    -1,
      96,    -1,    -1,    -1,   100,   101,    -1,    -1,   104,   105,
     106,   107,    -1,   109,   110,    -1,    -1,   113,    -1,    -1,
      -1,   117,   118,   119,   120,   121,    -1,   123,    -1,    -1,
     126,    -1,    -1,    -1,   130,   131,   132,    -1,   134,   135,
     136,   137,   138,    -1,   140,    -1,   142,   143,   144,    -1,
      -1,    -1,   148,   149,    -1,    -1,   152,   153,   154,    -1,
      -1,    -1,   158,   159,   160,     3,   162,    -1,    -1,    -1,
      -1,   167,    -1,   169,   170,    -1,    -1,    -1,   174,   175,
     176,    -1,   178,    21,   180,    23,    24,   183,    26,    27,
      -1,    -1,    -1,    -1,    -1,    33,    34,    35,    36,    -1,
      -1,    39,    40,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      48,    49,    50,    -1,    -1,    53,    -1,    -1,    -1,    57,
      58,    -1,    60,    -1,    -1,    63,    64,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    74,    -1,    -1,    -1,
      78,    79,    80,    81,    -1,    -1,    84,    -1,    86,    -1,
      88,    -1,    -1,    -1,    -1,    93,    94,    -1,    96,    -1,
      -1,    -1,   100,   101,    -1,    -1,   104,   105,   106,   107,
      -1,   109,   110,    -1,    -1,   113,    -1,    -1,    -1,   117,
     118,   119,   120,   121,    -1,   123,    -1,    -1,   126,    -1,
      -1,    -1,   130,   131,   132,    -1,   134,   135,   136,   137,
     138,    -1,   140,    -1,   142,   143,   144,    -1,    -1,    -1,
     148,   149,    -1,    -1,   152,   153,   154,    -1,    -1,    -1,
     158,   159,   160,     3,   162,    -1,    -1,    -1,    -1,   167,
      -1,   169,   170,    -1,    -1,    -1,   174,   175,   176,    -1,
     178,    21,   180,    23,    24,   183,    26,    27,    -1,    -1,
      -1,    -1,    -1,    33,    34,    35,    36,    -1,    -1,    39,
      40,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    48,    49,
      50,    -1,    -1,    53,    -1,    -1,    -1,    57,    58,    -1,
      60,    -1,    -1,    63,    64,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    74,    -1,    -1,    -1,    78,    79,
      80,    81,    -1,    -1,    84,    -1,    86,    -1,    88,    -1,
      -1,    -1,    -1,    93,    94,    -1,    96,    -1,    -1,    -1,
     100,   101,    -1,    -1,   104,   105,   106,   107,    -1,   109,
     110,    -1,    -1,   113,    -1,    -1,    -1,   117,   118,   119,
     120,   121,    -1,   123,    -1,    -1,   126,    -1,    -1,    -1,
     130,   131,   132,    -1,   134,   135,   136,   137,   138,    -1,
     140,    -1,   142,   143,   144,    -1,    -1,    -1,   148,   149,
      -1,     3,   152,   153,   154,    -1,    -1,    -1,   158,   159,
     160,    -1,   162,    -1,    -1,    -1,    -1,   167,    -1,   169,
     170,    23,    24,    -1,   174,   175,   176,    -1,   178,    -1,
     180,    33,    -1,   183,    -1,    -1,    -1,    39,    40,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    48,    49,    50,    -1,
      -1,    53,    -1,    -1,    -1,    -1,    58,    -1,    -1,    -1,
      -1,    63,    64,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    74,    -1,    -1,    -1,    78,    -1,    80,    81,
      -1,    -1,    84,    -1,    86,    -1,    88,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    96,    -1,    -1,    -1,   100,   101,
      -1,    -1,   104,   105,   106,   107,    -1,   109,   110,    -1,
      -1,    -1,    -1,    -1,    -1,   117,    -1,   119,   120,   121,
      -1,   123,    -1,    -1,   126,    -1,    -1,    -1,   130,   131,
     132,    -1,   134,   135,    -1,   137,   138,    -1,   140,    -1,
     142,   143,   144,    -1,    -1,    -1,   148,    -1,    -1,    -1,
     152,   153,   154,    -1,    -1,    -1,   158,    -1,    -1,    -1,
     162,    -1,    -1,    -1,    -1,    -1,    -1,   169,   170,    -1,
      -1,    -1,   174,   175,   176,    -1,    -1,    -1,   180,    -1,
      -1,   183
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,    15,    31,    51,    62,    67,    75,    76,    92,   145,
     147,   151,   164,   173,   179,   183,   185,   198,   199,   200,
     201,   202,   203,   208,   209,   210,   211,   212,   213,   216,
     217,   218,   219,   222,   223,   224,   225,   226,   231,   243,
     223,     3,    21,    23,    24,    26,    27,    33,    34,    35,
      36,    39,    40,    48,    49,    50,    53,    57,    58,    60,
      63,    64,    74,    78,    79,    80,    81,    84,    86,    88,
      93,    94,    96,   100,   101,   104,   105,   106,   107,   109,
     110,   113,   117,   118,   119,   120,   121,   123,   126,   130,
     131,   132,   134,   135,   136,   137,   138,   140,   142,   143,
     144,   148,   149,   152,   153,   154,   158,   159,   160,   162,
     167,   169,   170,   174,   175,   176,   178,   180,   183,   207,
     262,   303,   305,   335,   354,   357,   358,   359,   360,   156,
     177,   362,   156,   202,   202,   362,    18,    65,   249,    11,
       4,   362,   137,   244,   245,   354,     3,    11,    31,     0,
      70,   184,   125,   237,   238,    73,   163,    95,   224,    16,
      15,    17,   262,     5,    10,    15,   363,    82,    89,   361,
      97,   362,    42,   262,   244,   187,    15,   246,   247,     3,
      26,    27,    28,    29,    34,    35,    37,    57,    59,    60,
      66,    79,    93,    94,    96,   111,   113,   114,   118,   136,
     138,   149,   159,   160,   167,   178,   323,   324,   325,   327,
     328,   329,   330,   335,   336,   337,   338,   339,   340,   341,
     343,   344,   355,   358,   359,   207,   199,    30,   249,   249,
     249,     3,     4,     5,     6,     7,     8,    10,    11,    15,
      16,    32,    33,    39,    48,    55,    56,    61,    77,   102,
     116,   117,   139,   146,   155,   180,   190,   191,   204,   205,
     206,   227,   228,   230,   260,   268,   269,   270,   271,   272,
     273,   274,   275,   276,   277,   278,   279,   280,   281,   282,
     283,   284,   285,   286,   295,   296,   301,   302,   303,   304,
     305,   321,   322,   326,   335,   354,   356,   357,   359,    17,
     354,    13,   363,   262,    77,   261,   262,   262,    15,   188,
     250,   251,   252,   281,   354,   295,   147,   245,   248,   354,
      22,   101,    15,    15,   101,   168,    15,   101,   168,    15,
      15,    15,   132,    15,   144,   331,   332,   334,   335,    34,
      35,    15,   101,   168,    15,    15,    15,    15,   173,   174,
      15,   173,   174,    15,    15,    21,   110,    35,    41,   353,
     353,    17,    15,   239,   240,   281,   225,   225,   226,    15,
     223,   268,   281,   287,   281,   297,    15,    15,    15,    15,
      15,   228,   229,    15,   270,   281,    15,    15,    15,   285,
     285,    16,   187,    22,   124,    19,    98,    25,    38,    42,
      43,    44,    45,    46,    47,    90,    98,   103,   116,   192,
     193,   290,   291,   292,   353,   190,   191,   293,   108,   188,
     194,   195,   196,   294,   189,    15,    15,    15,    15,    17,
      17,   354,    17,    16,   172,   232,   187,    15,    61,   166,
     214,   215,   222,   247,    15,   281,    82,   187,   233,     4,
      22,   350,   351,   352,   354,    17,   220,   221,   281,    16,
     187,   229,   119,     5,     5,   105,   342,   119,    15,   342,
     119,    15,     5,   345,     5,     5,     5,    15,   161,    15,
      15,   168,    15,   101,   168,   342,   119,    15,   345,     5,
     262,     5,   159,   159,     5,   159,   159,   342,   342,    15,
     147,   262,   355,    16,   204,   187,    23,    64,   241,   223,
      16,    16,    16,   187,   171,   298,   299,   281,   287,   323,
       5,   287,   281,   287,   281,   205,   323,   269,   270,     6,
       9,   116,   289,   281,    42,    42,    44,    15,   229,   288,
       8,   116,   281,    25,    90,   103,    42,   193,    18,    20,
     150,   281,   349,   282,   229,   283,   284,   285,    18,    65,
     188,   281,   348,    16,    16,   287,   281,   354,   356,   354,
      17,    17,   354,   268,   262,   166,   287,   215,   222,    15,
     229,   253,   254,   255,   256,   262,   263,   264,   265,   251,
     232,   352,   188,   354,   232,   187,    42,   354,    15,    16,
      16,    15,   342,    16,    15,   342,    36,    40,    84,   100,
     104,   120,   346,   347,    16,    16,   187,    16,   187,    16,
       5,   144,   333,   335,     5,     5,    15,     5,   119,    15,
      16,    15,   342,    16,    16,   187,    16,    16,   176,   176,
      16,   176,   176,    16,    16,     5,   262,    17,   186,    16,
     240,    16,   281,   268,   281,    68,   299,   300,    22,    16,
     187,    16,    16,   187,    16,    16,    82,   289,    19,   287,
       8,    71,   281,   288,   281,    16,    16,   281,   128,   306,
      16,    16,    17,   354,   354,    17,    15,   254,   263,   350,
     187,    52,    83,    91,    99,   102,   139,   266,   350,    85,
     234,    17,   220,   281,     5,   345,    16,   345,    16,   346,
       5,     5,    16,   187,    15,    16,    16,     5,    16,    15,
       5,   345,    16,     5,   143,   173,   174,   173,   174,    16,
     353,   355,   186,   157,   157,   281,    69,   323,   281,   281,
     281,   281,    16,   281,    19,    71,   306,   306,    16,    15,
     307,   308,   354,   354,   356,   354,    16,   181,   182,   247,
     257,   258,   259,   254,    99,   127,   267,   254,   267,   267,
      99,   257,   258,    30,    87,   242,   188,   354,    16,    16,
      16,    16,    16,     5,     5,    16,   345,    16,    16,    16,
     262,   159,   159,   159,   159,    17,   281,   281,    16,    16,
     187,    16,    16,   281,   281,   306,   308,   309,   310,    17,
      15,    15,   255,   122,   254,   249,   268,    17,    16,    16,
      16,   176,   176,   176,   176,   355,     5,    16,   129,   311,
     354,   356,   354,   302,   303,   268,   122,    15,    86,   235,
     236,   281,   188,    16,    30,   237,    17,    81,    81,   268,
      16,   148,   187,   287,   133,   141,   312,   313,   354,   354,
     354,    15,   236,    25,    54,   162,   295,   314,   315,   316,
     317,    90,    90,   235,   162,   295,   315,   318,   319,   140,
     131,   131,    74,   320,   247,   247,    16,    80,    80,    19,
      54,    85,   115,   158,    16,    16,   318,   140,   126,   350,
     350
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int16 yyr1[] =
{
       0,   197,   198,   199,   199,   200,   200,   200,   200,   200,
     200,   201,   201,   201,   201,   201,   201,   202,   203,   203,
     203,   203,   204,   204,   205,   205,   206,   206,   207,   208,
     208,   209,   210,   210,   210,   211,   212,   213,   214,   214,
     214,   214,   214,   215,   216,   217,   218,   219,   220,   220,
     221,   222,   223,   223,   224,   224,   224,   225,   225,   226,
     226,   227,   228,   229,   230,   231,   232,   232,   233,   233,
     234,   234,   235,   235,   236,   236,   236,   237,   237,   238,
     239,   239,   240,   241,   241,   241,   242,   242,   243,   243,
     244,   244,   245,   246,   246,   247,   248,   248,   249,   249,
     249,   250,   250,   251,   251,   252,   252,   252,   252,   253,
     253,   254,   254,   255,   255,   256,   256,   256,   256,   256,
     256,   257,   258,   259,   259,   260,   260,   260,   260,   260,
     261,   261,   262,   262,   262,   262,   262,   262,   262,   262,
     263,   263,   264,   264,   265,   266,   266,   266,   266,   267,
     267,   268,   268,   269,   269,   270,   270,   271,   271,   271,
     272,   272,   273,   273,   273,   273,   273,   273,   273,   274,
     275,   276,   276,   277,   277,   277,   277,   278,   278,   279,
     279,   280,   281,   281,   281,   282,   282,   283,   283,   284,
     284,   285,   285,   285,   286,   286,   286,   286,   286,   286,
     287,   287,   288,   288,   289,   289,   290,   290,   290,   290,
     290,   290,   290,   290,   290,   290,   291,   291,   292,   293,
     293,   294,   294,   294,   294,   294,   295,   295,   295,   295,
     295,   295,   295,   295,   296,   297,   297,   298,   298,   299,
     299,   300,   300,   301,   301,   301,   301,   302,   302,   302,
     302,   302,   302,   303,   303,   303,   303,   303,   303,   303,
     303,   303,   303,   304,   305,   305,   305,   305,   305,   306,
     306,   307,   308,   309,   310,   310,   311,   311,   312,   312,
     313,   313,   314,   314,   315,   315,   315,   316,   317,   318,
     318,   318,   319,   320,   320,   320,   320,   320,   321,   321,
     322,   322,   322,   322,   322,   322,   322,   322,   322,   322,
     322,   322,   322,   322,   322,   323,   323,   323,   323,   324,
     325,   325,   325,   325,   326,   326,   326,   326,   327,   328,
     328,   328,   329,   329,   329,   329,   329,   329,   329,   329,
     329,   329,   329,   330,   331,   331,   332,   332,   333,   333,
     333,   334,   334,   334,   334,   334,   335,   335,   335,   335,
     335,   336,   337,   337,   337,   337,   337,   337,   337,   337,
     337,   337,   337,   338,   338,   339,   339,   339,   339,   339,
     339,   339,   339,   339,   339,   339,   339,   339,   340,   340,
     340,   340,   341,   341,   341,   341,   341,   341,   341,   341,
     341,   341,   341,   341,   341,   341,   341,   341,   342,   342,
     343,   343,   343,   343,   344,   344,   344,   344,   344,   344,
     344,   344,   344,   344,   344,   344,   344,   344,   344,   345,
     345,   345,   345,   346,   346,   346,   347,   347,   347,   348,
     348,   349,   349,   349,   350,   350,   351,   351,   352,   352,
     353,   354,   354,   355,   355,   355,   355,   356,   356,   356,
     357,   357,   357,   357,   357,   357,   357,   357,   357,   357,
     357,   357,   357,   357,   357,   357,   357,   357,   357,   358,
     358,   358,   358,   358,   359,   359,   359,   359,   359,   359,
     359,   359,   359,   359,   359,   359,   359,   359,   359,   359,
     359,   359,   359,   359,   359,   359,   359,   359,   359,   359,
     359,   359,   359,   359,   359,   359,   359,   359,   359,   360,
     360,   360,   360,   361,   361,   362,   362,   363,   363,   363
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     2,     1,     3,     0,     1,     1,     1,     1,
       1,     2,     4,     3,     1,     2,     2,     2,     5,     4,
       7,     6,     1,     3,     1,     3,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     4,     5,     1,     2,
       2,     1,     2,     2,     5,     1,     4,     6,     1,     3,
       3,     2,     1,     2,     1,     4,     4,     1,     4,     1,
       3,     3,     1,     1,     1,     8,     0,     2,     0,     2,
       0,     4,     1,     3,     2,     1,     5,     0,     1,     3,
       1,     3,     2,     0,     1,     1,     0,     2,     2,     3,
       1,     3,     4,     0,     1,     3,     1,     3,     0,     1,
       1,     1,     3,     2,     1,     1,     3,     5,     7,     1,
       3,     1,     1,     1,     3,     2,     3,     3,     3,     3,
       3,     9,     9,     0,     1,     1,     3,     5,     7,     9,
       1,     3,     1,     3,     5,     4,     7,     6,     6,     5,
       1,     1,     6,     5,     4,     2,     2,     2,     1,     0,
       1,     1,     3,     1,     3,     1,     2,     1,     3,     4,
       1,     3,     1,     1,     1,     1,     1,     1,     1,     3,
       3,     5,     6,     3,     5,     4,     6,     3,     4,     3,
       4,     2,     1,     2,     3,     1,     3,     1,     3,     1,
       3,     1,     2,     2,     1,     1,     1,     3,     1,     1,
       1,     3,     1,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     2,     2,     2,     2,     1,     2,     2,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     5,     0,     1,     1,     2,     4,
       4,     0,     2,     1,     1,     1,     1,     5,     4,     6,
       5,     5,     4,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     4,     1,     1,     1,     1,     1,     2,
       2,     3,     1,     4,     0,     1,     0,     3,     0,     3,
       1,     1,     1,     1,     2,     2,     1,     2,     4,     1,
       2,     1,     2,     0,     3,     2,     2,     3,     3,     4,
       4,     4,     4,     6,     8,     6,     4,     1,     4,     1,
       6,     1,     1,     4,     6,     1,     1,     1,     1,     1,
       1,     3,     5,     7,     1,     3,     5,     7,     6,     5,
       2,     2,     5,     4,     2,     1,     2,     1,     1,     1,
       1,     1,     1,     2,     3,     1,     4,     1,     1,     4,
       1,     4,     1,     6,     4,     1,     1,     1,     1,     1,
       1,     1,     1,     7,     7,     4,     4,     4,     7,     7,
       4,     4,     4,     1,     1,     6,     4,     6,     4,     6,
       4,     1,     1,     1,     1,     1,     1,     1,     4,     2,
       1,     1,     4,     4,     5,     5,     4,     1,     1,     4,
       6,     3,     6,     3,     4,     1,     1,     1,     1,     1,
       6,     3,     4,     1,     5,     2,     5,     2,     4,     6,
       6,     5,     7,     4,     6,     3,     4,     1,     1,     3,
       2,     2,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     0,     1,     2,     1,     1,     1,
       2,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     0,     2,     0,     2,     1,     1,     3
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = SQL2003_SQL2003_EMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == SQL2003_SQL2003_EMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (&yylloc, result, scanner, YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use SQL2003_SQL2003_error or SQL2003_SQL2003_UNDEF. */
#define YYERRCODE SQL2003_SQL2003_UNDEF

/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


/* Enable debugging if requested.  */
#if SQL2003_DEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)


/* YYLOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

# ifndef YYLOCATION_PRINT

#  if defined YY_LOCATION_PRINT

   /* Temporary convenience wrapper in case some people defined the
      undocumented and private YY_LOCATION_PRINT macros.  */
#   define YYLOCATION_PRINT(File, Loc)  YY_LOCATION_PRINT(File, *(Loc))

#  elif defined SQL2003_LTYPE_IS_TRIVIAL && SQL2003_LTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static int
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  int res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
}

#   define YYLOCATION_PRINT  yy_location_print_

    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT(File, Loc)  YYLOCATION_PRINT(File, &(Loc))

#  else

#   define YYLOCATION_PRINT(File, Loc) ((void) 0)
    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT  YYLOCATION_PRINT

#  endif
# endif /* !defined YYLOCATION_PRINT */


# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value, Location, result, scanner); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, ParseResult* result, yyscan_t scanner)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  YY_USE (yylocationp);
  YY_USE (result);
  YY_USE (scanner);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, ParseResult* result, yyscan_t scanner)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  YYLOCATION_PRINT (yyo, yylocationp);
  YYFPRINTF (yyo, ": ");
  yy_symbol_value_print (yyo, yykind, yyvaluep, yylocationp, result, scanner);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp,
                 int yyrule, ParseResult* result, yyscan_t scanner)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)],
                       &(yylsp[(yyi + 1) - (yynrhs)]), result, scanner);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule, result, scanner); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !SQL2003_DEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !SQL2003_DEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


/* Context of a parse error.  */
typedef struct
{
  yy_state_t *yyssp;
  yysymbol_kind_t yytoken;
  YYLTYPE *yylloc;
} yypcontext_t;

/* Put in YYARG at most YYARGN of the expected tokens given the
   current YYCTX, and return the number of tokens stored in YYARG.  If
   YYARG is null, return the number of expected tokens (guaranteed to
   be less than YYNTOKENS).  Return YYENOMEM on memory exhaustion.
   Return 0 if there are more than YYARGN expected tokens, yet fill
   YYARG up to YYARGN. */
static int
yypcontext_expected_tokens (const yypcontext_t *yyctx,
                            yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  int yyn = yypact[+*yyctx->yyssp];
  if (!yypact_value_is_default (yyn))
    {
      /* Start YYX at -YYN if negative to avoid negative indexes in
         YYCHECK.  In other words, skip the first -YYN actions for
         this state because they are default actions.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;
      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yyx;
      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
        if (yycheck[yyx + yyn] == yyx && yyx != YYSYMBOL_YYerror
            && !yytable_value_is_error (yytable[yyx + yyn]))
          {
            if (!yyarg)
              ++yycount;
            else if (yycount == yyargn)
              return 0;
            else
              yyarg[yycount++] = YY_CAST (yysymbol_kind_t, yyx);
          }
    }
  if (yyarg && yycount == 0 && 0 < yyargn)
    yyarg[0] = YYSYMBOL_YYEMPTY;
  return yycount;
}




#ifndef yystrlen
# if defined __GLIBC__ && defined _STRING_H
#  define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
# else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
# endif
#endif

#ifndef yystpcpy
# if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#  define yystpcpy stpcpy
# else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
# endif
#endif

#ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
      char const *yyp = yystr;
      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            else
              goto append;

          append:
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
}
#endif


static int
yy_syntax_error_arguments (const yypcontext_t *yyctx,
                           yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yyctx->yytoken != YYSYMBOL_YYEMPTY)
    {
      int yyn;
      if (yyarg)
        yyarg[yycount] = yyctx->yytoken;
      ++yycount;
      yyn = yypcontext_expected_tokens (yyctx,
                                        yyarg ? yyarg + 1 : yyarg, yyargn - 1);
      if (yyn == YYENOMEM)
        return YYENOMEM;
      else
        yycount += yyn;
    }
  return yycount;
}

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return -1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return YYENOMEM if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                const yypcontext_t *yyctx)
{
  enum { YYARGS_MAX = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  yysymbol_kind_t yyarg[YYARGS_MAX];
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* Actual size of YYARG. */
  int yycount = yy_syntax_error_arguments (yyctx, yyarg, YYARGS_MAX);
  if (yycount == YYENOMEM)
    return YYENOMEM;

  switch (yycount)
    {
#define YYCASE_(N, S)                       \
      case N:                               \
        yyformat = S;                       \
        break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
    }

  /* Compute error message size.  Don't count the "%s"s, but reserve
     room for the terminator.  */
  yysize = yystrlen (yyformat) - 2 * yycount + 1;
  {
    int yyi;
    for (yyi = 0; yyi < yycount; ++yyi)
      {
        YYPTRDIFF_T yysize1
          = yysize + yytnamerr (YY_NULLPTR, yytname[yyarg[yyi]]);
        if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
          yysize = yysize1;
        else
          return YYENOMEM;
      }
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return -1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yytname[yyarg[yyi++]]);
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep, YYLTYPE *yylocationp, ParseResult* result, yyscan_t scanner)
{
  YY_USE (yyvaluep);
  YY_USE (yylocationp);
  YY_USE (result);
  YY_USE (scanner);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  switch (yykind)
    {
    case YYSYMBOL_NAME: /* NAME  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 3531 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_STRING: /* STRING  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 3537 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_INTNUM: /* INTNUM  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 3543 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_BOOL: /* BOOL  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 3549 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_APPROXNUM: /* APPROXNUM  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 3555 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_NULLX: /* NULLX  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 3561 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_UNKNOWN: /* UNKNOWN  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 3567 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_QUESTIONMARK: /* QUESTIONMARK  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 3573 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_PARAM: /* PARAM  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 3579 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_CREATE_TABLE: /* CREATE_TABLE  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 3585 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_CREATE_TABLE_REST: /* CREATE_TABLE_REST  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 3591 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_sql_stmt: /* sql_stmt  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 3597 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_stmt_list: /* stmt_list  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 3603 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_stmt: /* stmt  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 3609 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_special_stmt: /* special_stmt  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 3615 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_sp_executesql_stmt: /* sp_executesql_stmt  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 3621 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_call_stmt: /* call_stmt  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 3627 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_sql_argument_list: /* sql_argument_list  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 3633 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_sql_argument: /* sql_argument  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 3639 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_value_expression: /* value_expression  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 3645 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_sp_name: /* sp_name  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 3651 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_ddl_stmt: /* ddl_stmt  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 3657 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_dql_stmt: /* dql_stmt  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 3663 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_dml_stmt: /* dml_stmt  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 3669 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_create_stmt: /* create_stmt  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 3675 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_create_table_stmt: /* create_table_stmt  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 3681 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_insert_stmt: /* insert_stmt  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 3687 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_insert_columns_and_source: /* insert_columns_and_source  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 3693 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_from_constructor: /* from_constructor  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 3699 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_delete_stmt: /* delete_stmt  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 3705 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_drop_stmt: /* drop_stmt  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 3711 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_drop_table_stmt: /* drop_table_stmt  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 3717 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_update_stmt: /* update_stmt  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 3723 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_update_elem_list: /* update_elem_list  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 3729 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_update_elem: /* update_elem  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 3735 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_select_stmt: /* select_stmt  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 3741 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_query_expression: /* query_expression  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 3747 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_query_expression_body: /* query_expression_body  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 3753 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_query_term: /* query_term  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 3759 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_query_primary: /* query_primary  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 3765 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_select_with_parens: /* select_with_parens  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 3771 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_subquery: /* subquery  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 3777 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_table_subquery: /* table_subquery  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 3783 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_row_subquery: /* row_subquery  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 3789 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_simple_table: /* simple_table  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 3795 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_opt_where: /* opt_where  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 3801 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_opt_from_clause: /* opt_from_clause  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 3807 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_opt_groupby: /* opt_groupby  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 3813 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_grouping_element_list: /* grouping_element_list  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 3819 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_grouping_element: /* grouping_element  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 3825 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_opt_order_by: /* opt_order_by  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 3831 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_order_by: /* order_by  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 3837 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_sort_list: /* sort_list  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 3843 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_sort_key: /* sort_key  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 3849 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_opt_asc_desc: /* opt_asc_desc  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 3855 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_opt_having: /* opt_having  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 3861 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_with_clause: /* with_clause  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 3867 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_with_list: /* with_list  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 3873 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_common_table_expr: /* common_table_expr  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 3879 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_opt_derived_column_list: /* opt_derived_column_list  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 3885 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_simple_ident_list_with_parens: /* simple_ident_list_with_parens  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 3891 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_simple_ident_list: /* simple_ident_list  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 3897 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_opt_distinct: /* opt_distinct  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 3903 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_select_expr_list: /* select_expr_list  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 3909 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_projection: /* projection  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 3915 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_asterisk_expr: /* asterisk_expr  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 3921 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_from_list: /* from_list  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 3927 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_table_reference: /* table_reference  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 3933 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_table_primary: /* table_primary  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 3939 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_table_primary_non_join: /* table_primary_non_join  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 3945 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_unpivot_clause: /* unpivot_clause  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 3951 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_pivot_clause: /* pivot_clause  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 3957 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_opt_simple_ident_list_with_parens: /* opt_simple_ident_list_with_parens  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 3963 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_column_ref: /* column_ref  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 3969 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_relation_factor_list: /* relation_factor_list  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 3975 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_relation_factor: /* relation_factor  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 3981 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_joined_table: /* joined_table  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 3987 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_qualified_join: /* qualified_join  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 3993 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_cross_join: /* cross_join  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 3999 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_join_type: /* join_type  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 4005 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_search_condition: /* search_condition  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 4011 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_boolean_term: /* boolean_term  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 4017 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_boolean_factor: /* boolean_factor  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 4023 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_boolean_test: /* boolean_test  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 4029 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_boolean_primary: /* boolean_primary  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 4035 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_predicate: /* predicate  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 4041 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_comparison_predicate: /* comparison_predicate  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 4047 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_quantified_comparison_predicate: /* quantified_comparison_predicate  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 4053 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_between_predicate: /* between_predicate  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 4059 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_like_predicate: /* like_predicate  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 4065 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_in_predicate: /* in_predicate  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 4071 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_null_predicate: /* null_predicate  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 4077 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_exists_predicate: /* exists_predicate  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 4083 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_row_expr: /* row_expr  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 4089 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_factor0: /* factor0  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 4095 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_factor1: /* factor1  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 4101 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_factor2: /* factor2  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 4107 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_factor3: /* factor3  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 4113 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_factor4: /* factor4  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 4119 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_row_expr_list: /* row_expr_list  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 4125 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_in_expr: /* in_expr  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 4131 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_truth_value: /* truth_value  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 4137 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_expr_const: /* expr_const  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 4143 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_case_expr: /* case_expr  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 4149 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_case_arg: /* case_arg  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 4155 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_when_clause_list: /* when_clause_list  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 4161 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_when_clause: /* when_clause  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 4167 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_case_default: /* case_default  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 4173 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_func_expr: /* func_expr  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 4179 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_aggregate_windowed_function: /* aggregate_windowed_function  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 4185 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_aggregate_function_name: /* aggregate_function_name  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 4191 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_ranking_windowed_function: /* ranking_windowed_function  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 4197 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_ranking_function_name: /* ranking_function_name  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 4203 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_over_clause: /* over_clause  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 4209 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_window_specification: /* window_specification  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 4215 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_window_name: /* window_name  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 4221 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_window_specification_details: /* window_specification_details  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 4227 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_opt_existing_window_name: /* opt_existing_window_name  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 4233 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_opt_window_partition_clause: /* opt_window_partition_clause  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 4239 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_opt_window_frame_clause: /* opt_window_frame_clause  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 4245 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_window_frame_units: /* window_frame_units  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 4251 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_window_frame_extent: /* window_frame_extent  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 4257 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_window_frame_start: /* window_frame_start  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 4263 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_window_frame_preceding: /* window_frame_preceding  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 4269 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_window_frame_between: /* window_frame_between  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 4275 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_window_frame_bound: /* window_frame_bound  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 4281 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_window_frame_following: /* window_frame_following  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 4287 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_opt_window_frame_exclusion: /* opt_window_frame_exclusion  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 4293 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_scalar_function: /* scalar_function  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 4299 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_function_call_keyword: /* function_call_keyword  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 4305 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_data_type: /* data_type  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 4311 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_user_defined_type_name: /* user_defined_type_name  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 4317 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_relation_factor_type: /* relation_factor_type  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 4323 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_relation_factor_func: /* relation_factor_func  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 4329 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_reference_type: /* reference_type  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 4335 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_collection_type: /* collection_type  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 4341 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_predefined_type: /* predefined_type  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 4347 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_interval_type: /* interval_type  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 4353 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_interval_qualifier: /* interval_qualifier  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 4359 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_start_field: /* start_field  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 4365 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_end_field: /* end_field  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 4371 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_single_datetime_field: /* single_datetime_field  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 4377 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_non_second_primary_datetime_field: /* non_second_primary_datetime_field  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 4383 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_boolean_type: /* boolean_type  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 4389 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_datetime_type: /* datetime_type  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 4395 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_numeric_type: /* numeric_type  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 4401 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_exact_numeric_type: /* exact_numeric_type  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 4407 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_approximate_numeric_type: /* approximate_numeric_type  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 4413 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_character_string_type: /* character_string_type  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 4419 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_data_type_len: /* data_type_len  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 4425 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_binary_large_object_string_type: /* binary_large_object_string_type  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 4431 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_national_character_string_type: /* national_character_string_type  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 4437 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_large_object_length: /* large_object_length  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 4443 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_char_length_units: /* char_length_units  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 4449 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_multiplier: /* multiplier  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 4455 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_distinct_or_all: /* distinct_or_all  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 4461 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_opt_as_label: /* opt_as_label  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 4467 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_as_label: /* as_label  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 4473 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_label: /* label  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 4479 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_collate_clause: /* collate_clause  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 4485 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_name_r: /* name_r  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 4491 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_name_type: /* name_type  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 4497 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_name_f: /* name_f  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 4503 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_reserved_type: /* reserved_type  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 4509 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_reserved_func: /* reserved_func  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 4515 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_reserved_other: /* reserved_other  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 4521 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_reserved: /* reserved  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 4527 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_opt_if_exists_clause: /* opt_if_exists_clause  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 4533 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_opt_top_clause: /* opt_top_clause  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 4539 "sqlparser_sql2003_bison.cpp"
        break;

    case YYSYMBOL_intnum_parens: /* intnum_parens  */
#line 95 "sqlparser_sql2003.y"
            { delete(((*yyvaluep).node)); }
#line 4545 "sqlparser_sql2003_bison.cpp"
        break;

      default:
        break;
    }
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}






/*----------.
| yyparse.  |
`----------*/

int
yyparse (ParseResult* result, yyscan_t scanner)
{
/* Lookahead token kind.  */
int yychar;


/* The semantic value of the lookahead symbol.  */
/* Default value used for initialization, for pacifying older GCCs
   or non-GCC compilers.  */
YY_INITIAL_VALUE (static YYSTYPE yyval_default;)
YYSTYPE yylval YY_INITIAL_VALUE (= yyval_default);

/* Location data for the lookahead symbol.  */
static YYLTYPE yyloc_default
# if defined SQL2003_LTYPE_IS_TRIVIAL && SQL2003_LTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
YYLTYPE yylloc = yyloc_default;

    /* Number of syntax errors so far.  */
    int yynerrs = 0;

    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

    /* The location stack: array, bottom, top.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls = yylsa;
    YYLTYPE *yylsp = yyls;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

  /* The locations where the error started and ended.  */
  YYLTYPE yyerror_range[3];

  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = SQL2003_SQL2003_EMPTY; /* Cause a token to be read.  */


/* User initialization code.  */
#line 64 "sqlparser_sql2003.y"
{
	// Initialize
	yylloc.first_column = 0;
	yylloc.last_column = 0;
	yylloc.first_line = 0;
	yylloc.last_line = 0;
}

#line 4651 "sqlparser_sql2003_bison.cpp"

  yylsp[0] = yylloc;
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yyls1, yysize * YYSIZEOF (*yylsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
        yyls = yyls1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
        YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == SQL2003_SQL2003_EMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex (&yylval, &yylloc, scanner);
    }

  if (yychar <= SQL2003_YYEOF)
    {
      yychar = SQL2003_YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == SQL2003_SQL2003_error)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = SQL2003_SQL2003_UNDEF;
      yytoken = YYSYMBOL_YYerror;
      yyerror_range[1] = yylloc;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END
  *++yylsp = yylloc;

  /* Discard the shifted token.  */
  yychar = SQL2003_SQL2003_EMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

  /* Default location. */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  yyerror_range[1] = yyloc;
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* sql_stmt: stmt_list END_P  */
#line 241 "sqlparser_sql2003.y"
{
    (yyval.node) = (yyvsp[-1].node);
    result->result_tree_ = (yyvsp[-1].node);
    result->accept = true;
    YYACCEPT;
}
#line 4869 "sqlparser_sql2003_bison.cpp"
    break;

  case 4: /* stmt_list: stmt ';' stmt_list  */
#line 252 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_STMT_LIST, E_LIST_PROPERTY_CNT, (yyvsp[-2].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = &SEMICOLON_LIST_SERIALIZE_FORMAT;
}
#line 4878 "sqlparser_sql2003_bison.cpp"
    break;

  case 5: /* stmt: %empty  */
#line 259 "sqlparser_sql2003.y"
                { (yyval.node) = nullptr; }
#line 4884 "sqlparser_sql2003_bison.cpp"
    break;

  case 11: /* special_stmt: USE NAME  */
#line 269 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_USE, 1, (yyvsp[0].node));
    (yyval.node)->serialize_format = &USE_SERIALIZE_FORMAT;
}
#line 4893 "sqlparser_sql2003_bison.cpp"
    break;

  case 12: /* special_stmt: SET PARAM COMP_EQ expr_const  */
#line 274 "sqlparser_sql2003.y"
{
    std::string val = "SET ";
    val += (yyvsp[-2].node)->text();
    delete((yyvsp[-2].node));
    val += " = ";
    val += (yyvsp[0].node)->text();
    delete((yyvsp[0].node));

    (yyval.node) = Node::makeTerminalNode(E_STRING, val);
}
#line 4908 "sqlparser_sql2003_bison.cpp"
    break;

  case 13: /* special_stmt: DECLARE PARAM data_type  */
#line 285 "sqlparser_sql2003.y"
{
    std::string val = "DECLARE ";
    val += (yyvsp[-1].node)->text();
    delete((yyvsp[-1].node));
    val += " ";
    val += (yyvsp[0].node)->text();
    delete((yyvsp[0].node));

    (yyval.node) = Node::makeTerminalNode(E_STRING, val);
}
#line 4923 "sqlparser_sql2003_bison.cpp"
    break;

  case 14: /* special_stmt: sp_executesql_stmt  */
#line 296 "sqlparser_sql2003.y"
{
    (yyval.node) = (yyvsp[0].node);
}
#line 4931 "sqlparser_sql2003_bison.cpp"
    break;

  case 15: /* special_stmt: EXEC sp_executesql_stmt  */
#line 300 "sqlparser_sql2003.y"
{
    (yyval.node) = (yyvsp[0].node);
}
#line 4939 "sqlparser_sql2003_bison.cpp"
    break;

  case 16: /* special_stmt: EXECUTE sp_executesql_stmt  */
#line 304 "sqlparser_sql2003.y"
{
    (yyval.node) = (yyvsp[0].node);
}
#line 4947 "sqlparser_sql2003_bison.cpp"
    break;

  case 17: /* sp_executesql_stmt: SP_EXECUTESQL STRING  */
#line 311 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_SP_EXECUTESQL, E_SP_EXECUTESQL_PROPERTY_CNT, (yyvsp[0].node));
    (yyval.node)->serialize_format = &SP_EXECUTESQL_SERIALIZE_FORMAT;
}
#line 4956 "sqlparser_sql2003_bison.cpp"
    break;

  case 18: /* call_stmt: CALL sp_name '(' sql_argument_list ')'  */
#line 319 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_CALL, E_CALL_PROPERTY_CNT, (yyvsp[-3].node), (yyvsp[-1].node));
    (yyval.node)->serialize_format = &CALL_SERIALIZE_FORMAT;
}
#line 4965 "sqlparser_sql2003_bison.cpp"
    break;

  case 19: /* call_stmt: CALL sp_name '(' ')'  */
#line 324 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_CALL, E_CALL_PROPERTY_CNT, (yyvsp[-2].node), nullptr);
    (yyval.node)->serialize_format = &CALL_SERIALIZE_FORMAT;
}
#line 4974 "sqlparser_sql2003_bison.cpp"
    break;

  case 20: /* call_stmt: '{' CALL sp_name '(' sql_argument_list ')' '}'  */
#line 329 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_CALL, E_CALL_PROPERTY_CNT, (yyvsp[-4].node), (yyvsp[-2].node));
    (yyval.node)->serialize_format = &CALL_SQL_SERVER_SERIALIZE_FORMAT;
}
#line 4983 "sqlparser_sql2003_bison.cpp"
    break;

  case 21: /* call_stmt: '{' CALL sp_name '(' ')' '}'  */
#line 334 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_CALL, E_CALL_PROPERTY_CNT, (yyvsp[-3].node), nullptr);
    (yyval.node)->serialize_format = &CALL_SQL_SERVER_SERIALIZE_FORMAT;
}
#line 4992 "sqlparser_sql2003_bison.cpp"
    break;

  case 23: /* sql_argument_list: sql_argument_list ',' sql_argument  */
#line 343 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_STMT_LIST, E_LIST_PROPERTY_CNT, (yyvsp[-2].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = &COMMA_LIST_SERIALIZE_FORMAT;
}
#line 5001 "sqlparser_sql2003_bison.cpp"
    break;

  case 24: /* sql_argument: value_expression  */
#line 351 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_SQL_ARGUMENT, E_SQL_ARGUMENT_PROPERTY_CNT, (yyvsp[0].node), nullptr);
    (yyval.node)->serialize_format = &SQL_ARGUMENT_SERIALIZE_FORMAT;
}
#line 5010 "sqlparser_sql2003_bison.cpp"
    break;

  case 25: /* sql_argument: value_expression AS data_type  */
#line 356 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_SQL_ARGUMENT, E_SQL_ARGUMENT_PROPERTY_CNT, (yyvsp[-2].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = &SQL_ARGUMENT_SERIALIZE_FORMAT;
}
#line 5019 "sqlparser_sql2003_bison.cpp"
    break;

  case 36: /* create_table_stmt: CREATE TABLE relation_factor CREATE_TABLE_REST  */
#line 392 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_CREATE_TABLE, E_CREATE_TABLE_PROPERTY_CNT, (yyvsp[-1].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = &CREATE_TABLE_SERIALIZE_FORMAT;
}
#line 5028 "sqlparser_sql2003_bison.cpp"
    break;

  case 37: /* insert_stmt: INSERT opt_top_clause INTO relation_factor insert_columns_and_source  */
#line 400 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_INSERT, 3, (yyvsp[-1].node), (yyvsp[0].node), (yyvsp[-3].node));
    (yyval.node)->serialize_format = &INSERT_SERIALIZE_FORMAT_V3;
}
#line 5037 "sqlparser_sql2003_bison.cpp"
    break;

  case 38: /* insert_columns_and_source: select_stmt  */
#line 408 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_INSERT_COLUMNS_AND_SOURCE, E_INSERT_COLUMNS_AND_SOURCE_PROPERTY_CNT, nullptr, (yyvsp[0].node));
    (yyval.node)->serialize_format = &DOUBLE_SERIALIZE_FORMAT;
}
#line 5046 "sqlparser_sql2003_bison.cpp"
    break;

  case 39: /* insert_columns_and_source: simple_ident_list_with_parens select_stmt  */
#line 413 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_INSERT_COLUMNS_AND_SOURCE, E_INSERT_COLUMNS_AND_SOURCE_PROPERTY_CNT, (yyvsp[-1].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = &DOUBLE_SERIALIZE_FORMAT;
}
#line 5055 "sqlparser_sql2003_bison.cpp"
    break;

  case 40: /* insert_columns_and_source: simple_ident_list_with_parens from_constructor  */
#line 418 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_INSERT_COLUMNS_AND_SOURCE, E_INSERT_COLUMNS_AND_SOURCE_PROPERTY_CNT, (yyvsp[-1].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = &DOUBLE_SERIALIZE_FORMAT;
}
#line 5064 "sqlparser_sql2003_bison.cpp"
    break;

  case 41: /* insert_columns_and_source: from_constructor  */
#line 423 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_INSERT_COLUMNS_AND_SOURCE, E_INSERT_COLUMNS_AND_SOURCE_PROPERTY_CNT, nullptr, (yyvsp[0].node));
    (yyval.node)->serialize_format = &DOUBLE_SERIALIZE_FORMAT;
}
#line 5073 "sqlparser_sql2003_bison.cpp"
    break;

  case 42: /* insert_columns_and_source: DEFAULT VALUES  */
#line 428 "sqlparser_sql2003.y"
{
    Node* src = Node::makeTerminalNode(E_IDENTIFIER, "DEFAULT VALUES");
    (yyval.node) = Node::makeNonTerminalNode(E_INSERT_COLUMNS_AND_SOURCE, E_INSERT_COLUMNS_AND_SOURCE_PROPERTY_CNT, nullptr, src);
    (yyval.node)->serialize_format = &DOUBLE_SERIALIZE_FORMAT;
}
#line 5083 "sqlparser_sql2003_bison.cpp"
    break;

  case 43: /* from_constructor: VALUES row_expr_list  */
#line 437 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_VALUES_CTOR, E_VALUES_CTOR_PROPERTY_CNT, (yyvsp[0].node));
    (yyval.node)->serialize_format = &TABLE_VALUE_CTOR_SERIALIZE_FORMAT;
}
#line 5092 "sqlparser_sql2003_bison.cpp"
    break;

  case 44: /* delete_stmt: DELETE opt_top_clause FROM relation_factor opt_where  */
#line 446 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_DELETE, E_DELETE_PROPERTY_CNT,
    		nullptr,	/* E_DELETE_OPT_WITH 0 */
    		(yyvsp[-3].node),	/* E_DELETE_OPT_TOP 1 */
    		(yyvsp[-1].node),		/* E_DELETE_DELETE_RELATION 2 */
    		nullptr,	/* E_DELETE_DELETE_RELATION_OPT_TABLE_HINT 3 */
    		nullptr,        /* E_DELETE_OPT_OUTPUT 4 */
    		nullptr,	/* E_DELETE_FROM_LIST 5 */
    		(yyvsp[0].node),		/* E_DELETE_OPT_WHERE 6 */
    		nullptr 	/* E_DELETE_OPT_QUERY_HINT 7 */);
    (yyval.node)->serialize_format = &DELETE_SERIALIZE_FORMAT;
}
#line 5109 "sqlparser_sql2003_bison.cpp"
    break;

  case 46: /* drop_table_stmt: DROP TABLE opt_if_exists_clause relation_factor_list  */
#line 466 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_DROP_TABLE, E_DROP_TABLE_PROPERTY_CNT,
        (yyvsp[-1].node), /* E_DROP_TABLE_OPT_IF_EXISTS */
        (yyvsp[0].node)  /* E_DROP_TABLE_TABLE_LIST */);
    (yyval.node)->serialize_format = &DROP_TABLE_SERIALIZE_FORMAT;
}
#line 5120 "sqlparser_sql2003_bison.cpp"
    break;

  case 47: /* update_stmt: UPDATE opt_top_clause relation_factor SET update_elem_list opt_where  */
#line 477 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_UPDATE, E_UPDATE_PROPERTY_CNT,
    		nullptr,	/* E_UPDATE_OPT_WITH 0 */
    		(yyvsp[-4].node),	/* E_UPDATE_OPT_TOP 1 */
    		(yyvsp[-3].node),		/* E_UPDATE_UPDATE_RELATION 2 */
    		nullptr,	/* E_UPDATE_UPDATE_RELATION_OPT_TABLE_HINT 3 */
    		(yyvsp[-1].node),		/* E_UPDATE_UPDATE_ELEM_LIST 4 */
    		nullptr,	/* E_UPDATE_OPT_OUTPUT 5 */
    		nullptr,	/* E_UPDATE_FROM_LIST 6 */
    		(yyvsp[0].node),		/* E_UPDATE_OPT_WHERE 7 */
    		nullptr		/* E_UPDATE_OPT_QUERY_HINT 8 */);
    (yyval.node)->serialize_format = &UPDATE_SERIALIZE_FORMAT;
}
#line 5138 "sqlparser_sql2003_bison.cpp"
    break;

  case 49: /* update_elem_list: update_elem ',' update_elem_list  */
#line 495 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_UPDATE_ELEM_LIST, E_LIST_PROPERTY_CNT, (yyvsp[-2].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = &COMMA_LIST_SERIALIZE_FORMAT;
}
#line 5147 "sqlparser_sql2003_bison.cpp"
    break;

  case 50: /* update_elem: row_expr COMP_EQ row_expr  */
#line 503 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_OP_EQ, E_OP_BINARY_PROPERTY_CNT, (yyvsp[-2].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = &OP_EQ_SERIALIZE_FORMAT;
}
#line 5156 "sqlparser_sql2003_bison.cpp"
    break;

  case 51: /* select_stmt: query_expression opt_order_by  */
#line 512 "sqlparser_sql2003.y"
{
    (yyval.node) = (yyvsp[-1].node);
    (yyval.node)->setChild(E_DIRECT_SELECT_ORDER, (yyvsp[0].node));
}
#line 5165 "sqlparser_sql2003_bison.cpp"
    break;

  case 52: /* query_expression: query_expression_body  */
#line 520 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_DIRECT_SELECT, E_DIRECT_SELECT_PROPERTY_CNT, nullptr, (yyvsp[0].node), nullptr, nullptr,nullptr);
    (yyval.node)->serialize_format = &SELECT_DIRECT_SERIALIZE_FORMAT;
}
#line 5174 "sqlparser_sql2003_bison.cpp"
    break;

  case 53: /* query_expression: with_clause query_expression_body  */
#line 525 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_DIRECT_SELECT, E_DIRECT_SELECT_PROPERTY_CNT, (yyvsp[-1].node), (yyvsp[0].node), nullptr, nullptr,nullptr);
    (yyval.node)->serialize_format = &SELECT_DIRECT_SERIALIZE_FORMAT;
}
#line 5183 "sqlparser_sql2003_bison.cpp"
    break;

  case 55: /* query_expression_body: query_expression_body UNION opt_distinct query_term  */
#line 533 "sqlparser_sql2003.y"
{
    Node* set_op = Node::makeTerminalNode(E_SET_UNION, "UNION");
    (yyval.node) = Node::makeNonTerminalNode(E_SELECT, E_SELECT_PROPERTY_CNT,
                        nullptr,             /* E_SELECT_DISTINCT 0 */
                        nullptr,             /* E_SELECT_SELECT_EXPR_LIST 1 */
                        nullptr,             /* E_SELECT_FROM_LIST 2 */
                        nullptr,             /* E_SELECT_OPT_WHERE 3 */
                        nullptr,             /* E_SELECT_GROUP_BY 4 */
                        nullptr,             /* E_SELECT_HAVING 5 */
                        set_op,              /* E_SELECT_SET_OPERATION 6 */
                        (yyvsp[-1].node),                  /* E_SELECT_ALL_SPECIFIED 7 */
                        (yyvsp[-3].node),                  /* E_SELECT_FORMER_SELECT_STMT 8 */
                        (yyvsp[0].node),                  /* E_SELECT_LATER_SELECT_STMT 9 */
                        nullptr,             /* E_SELECT_ORDER_BY 10 */
                        nullptr,             /* E_SELECT_LIMIT 11 */
                        nullptr,             /* E_SELECT_FOR_UPDATE 12 */
                        nullptr,             /* E_SELECT_HINTS 13 */
                        nullptr,             /* E_SELECT_WHEN 14 */
                        nullptr,             /* E_SELECT_OPT_TOP 15 */
                        nullptr,             /* E_SELECT_OPT_WITH 16 */
                        nullptr,             /* E_SELECT_OPT_OPTION 17 */
                        nullptr,              /* E_SELECT_OPT_INTO 18 */
                        nullptr             /*user for bigquery*/
                        );
    (yyval.node)->serialize_format = &SELECT_UNION_SERIALIZE_FORMAT;
}
#line 5214 "sqlparser_sql2003_bison.cpp"
    break;

  case 56: /* query_expression_body: query_expression_body EXCEPT opt_distinct query_term  */
#line 560 "sqlparser_sql2003.y"
{
    Node* set_op = Node::makeTerminalNode(E_SET_EXCEPT, "EXCEPT");
    (yyval.node) = Node::makeNonTerminalNode(E_SELECT, E_SELECT_PROPERTY_CNT,
                        nullptr,             /* E_SELECT_DISTINCT 0 */
                        nullptr,             /* E_SELECT_SELECT_EXPR_LIST 1 */
                        nullptr,             /* E_SELECT_FROM_LIST 2 */
                        nullptr,             /* E_SELECT_OPT_WHERE 3 */
                        nullptr,             /* E_SELECT_GROUP_BY 4 */
                        nullptr,             /* E_SELECT_HAVING 5 */
                        set_op,              /* E_SELECT_SET_OPERATION 6 */
                        (yyvsp[-1].node),                  /* E_SELECT_ALL_SPECIFIED 7 */
                        (yyvsp[-3].node),                  /* E_SELECT_FORMER_SELECT_STMT 8 */
                        (yyvsp[0].node),                  /* E_SELECT_LATER_SELECT_STMT 9 */
                        nullptr,             /* E_SELECT_ORDER_BY 10 */
                        nullptr,             /* E_SELECT_LIMIT 11 */
                        nullptr,             /* E_SELECT_FOR_UPDATE 12 */
                        nullptr,             /* E_SELECT_HINTS 13 */
                        nullptr,             /* E_SELECT_WHEN 14 */
                        nullptr,             /* E_SELECT_OPT_TOP 15 */
                        nullptr,             /* E_SELECT_OPT_WITH 16 */
                        nullptr,             /* E_SELECT_OPT_OPTION 17 */
                        nullptr,             /* E_SELECT_OPT_INTO 18 */
                        nullptr             /*user for bigquery*/
                        );
    (yyval.node)->serialize_format = &SELECT_EXCEPT_SERIALIZE_FORMAT;
}
#line 5245 "sqlparser_sql2003_bison.cpp"
    break;

  case 58: /* query_term: query_term INTERSECT opt_distinct query_primary  */
#line 591 "sqlparser_sql2003.y"
{
    Node* set_op = Node::makeTerminalNode(E_SET_INTERSECT, "INTERSECT");
    (yyval.node) = Node::makeNonTerminalNode(E_SELECT, E_SELECT_PROPERTY_CNT,
                        nullptr,             /* E_SELECT_DISTINCT 0 */
                        nullptr,             /* E_SELECT_SELECT_EXPR_LIST 1 */
                        nullptr,             /* E_SELECT_FROM_LIST 2 */
                        nullptr,             /* E_SELECT_OPT_WHERE 3 */
                        nullptr,             /* E_SELECT_GROUP_BY 4 */
                        nullptr,             /* E_SELECT_HAVING 5 */
                        set_op,              /* E_SELECT_SET_OPERATION 6 */
                        (yyvsp[-1].node),                  /* E_SELECT_ALL_SPECIFIED 7 */
                        (yyvsp[-3].node),                  /* E_SELECT_FORMER_SELECT_STMT 8 */
                        (yyvsp[0].node),                  /* E_SELECT_LATER_SELECT_STMT 9 */
                        nullptr,             /* E_SELECT_ORDER_BY 10 */
                        nullptr,             /* E_SELECT_LIMIT 11 */
                        nullptr,             /* E_SELECT_FOR_UPDATE 12 */
                        nullptr,             /* E_SELECT_HINTS 13 */
                        nullptr,             /* E_SELECT_WHEN 14 */
                        nullptr,             /* E_SELECT_OPT_TOP 15 */
                        nullptr,             /* E_SELECT_OPT_WITH 16 */
                        nullptr,             /* E_SELECT_OPT_OPTION 17 */
                        nullptr,              /* E_SELECT_OPT_INTO 18 */
                        nullptr             /*user for bigquery*/
                        );
    (yyval.node)->serialize_format = &SELECT_INTERSECT_SERIALIZE_FORMAT;
}
#line 5276 "sqlparser_sql2003_bison.cpp"
    break;

  case 60: /* query_primary: '(' query_expression ')'  */
#line 622 "sqlparser_sql2003.y"
{
    if ((yyvsp[-1].node)->getChild(E_DIRECT_SELECT_WITH)) {
	yyerror(&(yylsp[-2]), result, scanner, "error use common table expression");
	YYABORT;
    }
    (yyval.node) = Node::makeNonTerminalNode(E_SELECT_WITH_PARENS, E_PARENS_PROPERTY_CNT, (yyvsp[-1].node)->getChild(E_DIRECT_SELECT_SELECT_CLAUSE));
    (yyval.node)->serialize_format = &SINGLE_WITH_PARENS_SERIALIZE_FORMAT;
    (yyvsp[-1].node)->setChild(E_DIRECT_SELECT_SELECT_CLAUSE, nullptr);
    delete((yyvsp[-1].node));
}
#line 5291 "sqlparser_sql2003_bison.cpp"
    break;

  case 61: /* select_with_parens: '(' query_expression ')'  */
#line 637 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_SELECT_WITH_PARENS, E_PARENS_PROPERTY_CNT, (yyvsp[-1].node));
    (yyval.node)->serialize_format = &SINGLE_WITH_PARENS_SERIALIZE_FORMAT;
}
#line 5300 "sqlparser_sql2003_bison.cpp"
    break;

  case 65: /* simple_table: SELECT opt_distinct opt_top_clause select_expr_list opt_from_clause opt_where opt_groupby opt_having  */
#line 651 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_SELECT, E_SELECT_PROPERTY_CNT,
                    (yyvsp[-6].node),             /* E_SELECT_DISTINCT 0 */
                    (yyvsp[-4].node),             /* E_SELECT_SELECT_EXPR_LIST 1 */
                    (yyvsp[-3].node),             /* E_SELECT_FROM_LIST 2 */
                    (yyvsp[-2].node),             /* E_SELECT_OPT_WHERE 3 */
                    (yyvsp[-1].node),             /* E_SELECT_GROUP_BY 4 */
                    (yyvsp[0].node),             /* E_SELECT_HAVING 5 */
                    nullptr,        /* E_SELECT_SET_OPERATION 6 */
                    nullptr,        /* E_SELECT_ALL_SPECIFIED 7 */
                    nullptr,        /* E_SELECT_FORMER_SELECT_STMT 8 */
                    nullptr,        /* E_SELECT_LATER_SELECT_STMT 9 */
                    nullptr,        /* E_SELECT_ORDER_BY 10 */
                    nullptr,        /* E_SELECT_LIMIT 11 */
                    nullptr,        /* E_SELECT_FOR_UPDATE 12 */
                    nullptr,        /* E_SELECT_HINTS 13 */
                    nullptr,        /* E_SELECT_WHEN 14 */
                    (yyvsp[-5].node),        /* E_SELECT_OPT_TOP 15 */
                    nullptr,        /* E_SELECT_OPT_WITH 16 */
                    nullptr,        /* E_SELECT_OPT_OPTION 17 */
	                nullptr,         /* E_SELECT_OPT_INTO 18 */
	                nullptr             /*user for bigquery*/
                    );
    (yyval.node)->serialize_format = &SELECT_SERIALIZE_FORMAT;
}
#line 5330 "sqlparser_sql2003_bison.cpp"
    break;

  case 66: /* opt_where: %empty  */
#line 679 "sqlparser_sql2003.y"
                { (yyval.node) = nullptr; }
#line 5336 "sqlparser_sql2003_bison.cpp"
    break;

  case 67: /* opt_where: WHERE search_condition  */
#line 681 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_WHERE_CLAUSE, E_WHERE_CLAUSE_PROPERTY_CNT, (yyvsp[0].node));
    (yyval.node)->serialize_format = &WHERE_SERIALIZE_FORMAT;
}
#line 5345 "sqlparser_sql2003_bison.cpp"
    break;

  case 68: /* opt_from_clause: %empty  */
#line 688 "sqlparser_sql2003.y"
                { (yyval.node) = nullptr; }
#line 5351 "sqlparser_sql2003_bison.cpp"
    break;

  case 69: /* opt_from_clause: FROM from_list  */
#line 690 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_FROM_CLAUSE, E_FROM_CLAUSE_PROPERTY_CNT, (yyvsp[0].node));
    (yyval.node)->serialize_format = &FROM_SERIALIZE_FORMAT;
}
#line 5360 "sqlparser_sql2003_bison.cpp"
    break;

  case 70: /* opt_groupby: %empty  */
#line 697 "sqlparser_sql2003.y"
                { (yyval.node) = nullptr; }
#line 5366 "sqlparser_sql2003_bison.cpp"
    break;

  case 71: /* opt_groupby: GROUP BY opt_distinct grouping_element_list  */
#line 699 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_GROUP_BY, E_GROUP_BY_PROPERTY_CNT, (yyvsp[0].node), (yyvsp[-1].node));
    (yyval.node)->serialize_format = &GROUP_BY_SERIALIZE_FORMAT;
}
#line 5375 "sqlparser_sql2003_bison.cpp"
    break;

  case 73: /* grouping_element_list: grouping_element_list ',' grouping_element  */
#line 708 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_EXPR_LIST, E_LIST_PROPERTY_CNT, (yyvsp[-2].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = &COMMA_LIST_SERIALIZE_FORMAT;
}
#line 5384 "sqlparser_sql2003_bison.cpp"
    break;

  case 74: /* grouping_element: '(' ')'  */
#line 715 "sqlparser_sql2003.y"
                { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER,"()"); }
#line 5390 "sqlparser_sql2003_bison.cpp"
    break;

  case 76: /* grouping_element: GROUPING SETS '(' grouping_element_list ')'  */
#line 718 "sqlparser_sql2003.y"
{
    Node* fun_name = Node::makeTerminalNode(E_IDENTIFIER, "GROUPING SETS");
    (yyval.node) = Node::makeNonTerminalNode(E_FUN_CALL, E_FUN_CALL_PROPERTY_CNT,
        		fun_name, (yyvsp[-1].node), nullptr, nullptr, nullptr);
    (yyval.node)->serialize_format = &FUN_CALL_1_OVER_SERIALIZE_FORMAT;
}
#line 5401 "sqlparser_sql2003_bison.cpp"
    break;

  case 77: /* opt_order_by: %empty  */
#line 727 "sqlparser_sql2003.y"
                { (yyval.node) = nullptr; }
#line 5407 "sqlparser_sql2003_bison.cpp"
    break;

  case 79: /* order_by: ORDER BY sort_list  */
#line 733 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_ORDER_BY, E_ORDER_BY_PROPERTY_CNT, (yyvsp[0].node), nullptr);
    (yyval.node)->serialize_format = &ORDER_BY_SERIALIZE_FORMAT;
}
#line 5416 "sqlparser_sql2003_bison.cpp"
    break;

  case 81: /* sort_list: sort_list ',' sort_key  */
#line 742 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_SORT_LIST, E_LIST_PROPERTY_CNT, (yyvsp[-2].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = &COMMA_LIST_SERIALIZE_FORMAT;
}
#line 5425 "sqlparser_sql2003_bison.cpp"
    break;

  case 82: /* sort_key: row_expr opt_asc_desc  */
#line 750 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_SORT_KEY, E_SORT_KEY_PROPERTY_CNT, (yyvsp[-1].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = &DOUBLE_SERIALIZE_FORMAT;
}
#line 5434 "sqlparser_sql2003_bison.cpp"
    break;

  case 83: /* opt_asc_desc: %empty  */
#line 758 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeTerminalNode(E_SORT_ASC, "");
}
#line 5442 "sqlparser_sql2003_bison.cpp"
    break;

  case 84: /* opt_asc_desc: ASC  */
#line 762 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeTerminalNode(E_SORT_ASC, "ASC");
}
#line 5450 "sqlparser_sql2003_bison.cpp"
    break;

  case 85: /* opt_asc_desc: DESC  */
#line 766 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeTerminalNode(E_SORT_DESC, "DESC");
}
#line 5458 "sqlparser_sql2003_bison.cpp"
    break;

  case 86: /* opt_having: %empty  */
#line 772 "sqlparser_sql2003.y"
                { (yyval.node) = nullptr; }
#line 5464 "sqlparser_sql2003_bison.cpp"
    break;

  case 87: /* opt_having: HAVING search_condition  */
#line 774 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_HAVING, E_HAVING_PROPERTY_CNT, (yyvsp[0].node));
    (yyval.node)->serialize_format = &HAVING_SERIALIZE_FORMAT;
}
#line 5473 "sqlparser_sql2003_bison.cpp"
    break;

  case 88: /* with_clause: WITH with_list  */
#line 784 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_OPT_WITH_CLAUSE, E_OPT_WITH_CLAUSE_PROPERTY_CNT, (yyvsp[0].node));
    (yyval.node)->serialize_format = &WITH_CLAUSE_SERIALIZE_FORMAT;
}
#line 5482 "sqlparser_sql2003_bison.cpp"
    break;

  case 89: /* with_clause: WITH RECURSIVE with_list  */
#line 789 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_OPT_WITH_CLAUSE, E_OPT_WITH_CLAUSE_PROPERTY_CNT, (yyvsp[0].node));
    (yyval.node)->serialize_format = &WITH_RECURSIVE_CLAUSE_SERIALIZE_FORMAT;
}
#line 5491 "sqlparser_sql2003_bison.cpp"
    break;

  case 91: /* with_list: with_list ',' common_table_expr  */
#line 798 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_WITH_LIST, E_LIST_PROPERTY_CNT, (yyvsp[-2].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = &COMMA_LIST_SERIALIZE_FORMAT;
}
#line 5500 "sqlparser_sql2003_bison.cpp"
    break;

  case 92: /* common_table_expr: name_r opt_derived_column_list AS table_subquery  */
#line 807 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_COMMON_TABLE_EXPR, E_COMMON_TABLE_EXPR_PROPERTY_CNT, (yyvsp[-3].node), (yyvsp[-2].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = &COMMON_TABLE_EXPR_SERIALIZE_FORMAT;
}
#line 5509 "sqlparser_sql2003_bison.cpp"
    break;

  case 93: /* opt_derived_column_list: %empty  */
#line 814 "sqlparser_sql2003.y"
                { (yyval.node) = nullptr; }
#line 5515 "sqlparser_sql2003_bison.cpp"
    break;

  case 95: /* simple_ident_list_with_parens: '(' simple_ident_list ')'  */
#line 820 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_SIMPLE_IDENT_LIST_WITH_PARENS, E_PARENS_PROPERTY_CNT, (yyvsp[-1].node));
    (yyval.node)->serialize_format = &SINGLE_WITH_PARENS_SERIALIZE_FORMAT;
}
#line 5524 "sqlparser_sql2003_bison.cpp"
    break;

  case 97: /* simple_ident_list: simple_ident_list ',' name_r  */
#line 829 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_SIMPLE_IDENT_LIST, E_LIST_PROPERTY_CNT, (yyvsp[-2].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = &COMMA_LIST_SERIALIZE_FORMAT;
}
#line 5533 "sqlparser_sql2003_bison.cpp"
    break;

  case 98: /* opt_distinct: %empty  */
#line 836 "sqlparser_sql2003.y"
                { (yyval.node) = nullptr; }
#line 5539 "sqlparser_sql2003_bison.cpp"
    break;

  case 99: /* opt_distinct: ALL  */
#line 838 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeTerminalNode(E_ALL, "ALL");
}
#line 5547 "sqlparser_sql2003_bison.cpp"
    break;

  case 100: /* opt_distinct: DISTINCT  */
#line 842 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeTerminalNode(E_DISTINCT, "DISTINCT");
}
#line 5555 "sqlparser_sql2003_bison.cpp"
    break;

  case 102: /* select_expr_list: select_expr_list ',' projection  */
#line 850 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_SELECT_EXPR_LIST, E_LIST_PROPERTY_CNT, (yyvsp[-2].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = &COMMA_LIST_SERIALIZE_FORMAT;
}
#line 5564 "sqlparser_sql2003_bison.cpp"
    break;

  case 103: /* projection: row_expr opt_as_label  */
#line 858 "sqlparser_sql2003.y"
{
    if (!(yyvsp[0].node)) {
    	(yyval.node) = Node::makeNonTerminalNode(E_PROJECT_STRING, E_PROJECT_STRING_PROPERTY_CNT, (yyvsp[-1].node));
    	(yyval.node)->serialize_format = &SINGLE_SERIALIZE_FORMAT;
    }
    else {
	Node* alias_node = Node::makeNonTerminalNode(E_ALIAS, E_ALIAS_PROPERTY_CNT, (yyvsp[-1].node), (yyvsp[0].node), nullptr, nullptr, nullptr);
        alias_node->serialize_format = &AS_SERIALIZE_FORMAT;

        (yyval.node) = Node::makeNonTerminalNode(E_PROJECT_STRING, E_PROJECT_STRING_PROPERTY_CNT, alias_node);
        (yyval.node)->serialize_format = &SINGLE_SERIALIZE_FORMAT;
    }
}
#line 5582 "sqlparser_sql2003_bison.cpp"
    break;

  case 105: /* asterisk_expr: '*'  */
#line 876 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeTerminalNode(E_STAR, "*");
}
#line 5590 "sqlparser_sql2003_bison.cpp"
    break;

  case 106: /* asterisk_expr: name_r '.' '*'  */
#line 880 "sqlparser_sql2003.y"
{
    Node* star_node = Node::makeTerminalNode(E_STAR, "*");
    (yyval.node) = Node::makeNonTerminalNode(E_ASTERISK_QUALIFY, 4, nullptr, nullptr, (yyvsp[-2].node), star_node);
    (yyval.node)->serialize_format = &ASTERISK_QUALIFY_SERIALIZE_FORMAT;
}
#line 5600 "sqlparser_sql2003_bison.cpp"
    break;

  case 107: /* asterisk_expr: name_r '.' name_r '.' '*'  */
#line 886 "sqlparser_sql2003.y"
{
    Node* star_node = Node::makeTerminalNode(E_STAR, "*");
    (yyval.node) = Node::makeNonTerminalNode(E_ASTERISK_QUALIFY, 4, nullptr, (yyvsp[-4].node), (yyvsp[-2].node), star_node);
    (yyval.node)->serialize_format = &ASTERISK_QUALIFY_SERIALIZE_FORMAT;
}
#line 5610 "sqlparser_sql2003_bison.cpp"
    break;

  case 108: /* asterisk_expr: name_r '.' name_r '.' name_r '.' '*'  */
#line 892 "sqlparser_sql2003.y"
{
    Node* star_node = Node::makeTerminalNode(E_STAR, "*");
    (yyval.node) = Node::makeNonTerminalNode(E_ASTERISK_QUALIFY, 4, (yyvsp[-6].node), (yyvsp[-4].node), (yyvsp[-2].node), star_node);
    (yyval.node)->serialize_format = &ASTERISK_QUALIFY_SERIALIZE_FORMAT;
}
#line 5620 "sqlparser_sql2003_bison.cpp"
    break;

  case 110: /* from_list: from_list ',' table_reference  */
#line 902 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_FROM_LIST, E_LIST_PROPERTY_CNT, (yyvsp[-2].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = &COMMA_LIST_SERIALIZE_FORMAT;
}
#line 5629 "sqlparser_sql2003_bison.cpp"
    break;

  case 114: /* table_primary: '(' joined_table ')'  */
#line 916 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_JOINED_TABLE_WITH_PARENS, E_PARENS_PROPERTY_CNT, (yyvsp[-1].node));
    (yyval.node)->serialize_format = &SINGLE_WITH_PARENS_SERIALIZE_FORMAT;
}
#line 5638 "sqlparser_sql2003_bison.cpp"
    break;

  case 115: /* table_primary_non_join: relation_factor opt_as_label  */
#line 928 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_ALIAS, E_ALIAS_PROPERTY_CNT, (yyvsp[-1].node), (yyvsp[0].node), nullptr, nullptr, nullptr);
    (yyval.node)->serialize_format = &AS_SERIALIZE_FORMAT;
}
#line 5647 "sqlparser_sql2003_bison.cpp"
    break;

  case 116: /* table_primary_non_join: table_subquery opt_as_label opt_simple_ident_list_with_parens  */
#line 933 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_ALIAS, E_ALIAS_PROPERTY_CNT, (yyvsp[-2].node), (yyvsp[-1].node), (yyvsp[0].node), nullptr, nullptr);
    (yyval.node)->serialize_format = &AS_SERIALIZE_FORMAT;
}
#line 5656 "sqlparser_sql2003_bison.cpp"
    break;

  case 117: /* table_primary_non_join: relation_factor opt_as_label unpivot_clause  */
#line 938 "sqlparser_sql2003.y"
{
    // Translate "relation_factor" into "table_subquery" node for mask and filter
    Node* alias_node = Node::makeNonTerminalNode(E_ALIAS, E_ALIAS_PROPERTY_CNT, (yyvsp[-2].node), nullptr, nullptr, nullptr, nullptr);
    alias_node->serialize_format = &AS_SERIALIZE_FORMAT;

    Node* from_clause_node = Node::makeNonTerminalNode(E_FROM_CLAUSE, E_FROM_CLAUSE_PROPERTY_CNT, alias_node);
    from_clause_node->serialize_format = &FROM_SERIALIZE_FORMAT;

    Node* star_node = Node::makeTerminalNode(E_STAR, "*");

    Node* select_node = Node::makeNonTerminalNode(E_SELECT, E_SELECT_PROPERTY_CNT,
                        nullptr,             /* E_SELECT_DISTINCT 0 */
                        star_node,           /* E_SELECT_SELECT_EXPR_LIST 1 */
                        from_clause_node,    /* E_SELECT_FROM_LIST 2 */
                        nullptr,             /* E_SELECT_OPT_WHERE 3 */
                        nullptr,             /* E_SELECT_GROUP_BY 4 */
                        nullptr,             /* E_SELECT_HAVING 5 */
                        nullptr,             /* E_SELECT_SET_OPERATION 6 */
                        nullptr,             /* E_SELECT_ALL_SPECIFIED 7 */
                        nullptr,             /* E_SELECT_FORMER_SELECT_STMT 8 */
                        nullptr,             /* E_SELECT_LATER_SELECT_STMT 9 */
                        nullptr,             /* E_SELECT_ORDER_BY 10 */
                        nullptr,             /* E_SELECT_LIMIT 11 */
                        nullptr,             /* E_SELECT_FOR_UPDATE 12 */
                        nullptr,             /* E_SELECT_HINTS 13 */
                        nullptr,             /* E_SELECT_WHEN 14 */
                        nullptr,             /* E_SELECT_OPT_TOP 15 */
                        nullptr,             /* E_SELECT_OPT_WITH 16 */
                        nullptr,             /* E_SELECT_OPT_OPTION 17 */
                        nullptr,             /* E_SELECT_OPT_INTO 18 */
                        nullptr              /*user for bigquery*/
                        );
    select_node->serialize_format = &SELECT_SERIALIZE_FORMAT;

    Node* direct_select_node = Node::makeNonTerminalNode(E_DIRECT_SELECT, E_DIRECT_SELECT_PROPERTY_CNT, nullptr, select_node, nullptr, nullptr,nullptr);
    direct_select_node->serialize_format = &SELECT_DIRECT_SERIALIZE_FORMAT;

    Node* select_with_parens_node = Node::makeNonTerminalNode(E_SELECT_WITH_PARENS, E_PARENS_PROPERTY_CNT, direct_select_node);
    select_with_parens_node->serialize_format = &SINGLE_WITH_PARENS_SERIALIZE_FORMAT;
    
    if ((yyvsp[-1].node))
    {
      (yyval.node) = Node::makeNonTerminalNode(E_ALIAS, E_ALIAS_PROPERTY_CNT, select_with_parens_node, (yyvsp[-1].node), (yyvsp[0].node), nullptr, nullptr);
    }
    else
    {
      Node* identifier_node = Node::makeTerminalNode(E_IDENTIFIER, (yyvsp[-2].node)->getChild(E_TABLE_IDENT_OBJECT)->text());
      (yyval.node) = Node::makeNonTerminalNode(E_ALIAS, E_ALIAS_PROPERTY_CNT, select_with_parens_node, identifier_node, (yyvsp[0].node), nullptr, nullptr);
    }

    (yyval.node)->serialize_format = &AS_SERIALIZE_FORMAT;
}
#line 5713 "sqlparser_sql2003_bison.cpp"
    break;

  case 118: /* table_primary_non_join: table_subquery opt_as_label unpivot_clause  */
#line 991 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_ALIAS, E_ALIAS_PROPERTY_CNT, (yyvsp[-2].node), (yyvsp[-1].node), (yyvsp[0].node), nullptr, nullptr);
    (yyval.node)->serialize_format = &AS_SERIALIZE_FORMAT;
}
#line 5722 "sqlparser_sql2003_bison.cpp"
    break;

  case 119: /* table_primary_non_join: relation_factor opt_as_label pivot_clause  */
#line 996 "sqlparser_sql2003.y"
{
    // Translate "relation_factor" into "table_subquery" node for mask and filter
    Node* alias_node = Node::makeNonTerminalNode(E_ALIAS, E_ALIAS_PROPERTY_CNT, (yyvsp[-2].node), nullptr, nullptr, nullptr, nullptr);
    alias_node->serialize_format = &AS_SERIALIZE_FORMAT;

    Node* from_clause_node = Node::makeNonTerminalNode(E_FROM_CLAUSE, E_FROM_CLAUSE_PROPERTY_CNT, alias_node);
    from_clause_node->serialize_format = &FROM_SERIALIZE_FORMAT;

    Node* star_node = Node::makeTerminalNode(E_STAR, "*");

    Node* select_node = Node::makeNonTerminalNode(E_SELECT, E_SELECT_PROPERTY_CNT,
                        nullptr,             /* E_SELECT_DISTINCT 0 */
                        star_node,           /* E_SELECT_SELECT_EXPR_LIST 1 */
                        from_clause_node,    /* E_SELECT_FROM_LIST 2 */
                        nullptr,             /* E_SELECT_OPT_WHERE 3 */
                        nullptr,             /* E_SELECT_GROUP_BY 4 */
                        nullptr,             /* E_SELECT_HAVING 5 */
                        nullptr,             /* E_SELECT_SET_OPERATION 6 */
                        nullptr,             /* E_SELECT_ALL_SPECIFIED 7 */
                        nullptr,             /* E_SELECT_FORMER_SELECT_STMT 8 */
                        nullptr,             /* E_SELECT_LATER_SELECT_STMT 9 */
                        nullptr,             /* E_SELECT_ORDER_BY 10 */
                        nullptr,             /* E_SELECT_LIMIT 11 */
                        nullptr,             /* E_SELECT_FOR_UPDATE 12 */
                        nullptr,             /* E_SELECT_HINTS 13 */
                        nullptr,             /* E_SELECT_WHEN 14 */
                        nullptr,             /* E_SELECT_OPT_TOP 15 */
                        nullptr,             /* E_SELECT_OPT_WITH 16 */
                        nullptr,             /* E_SELECT_OPT_OPTION 17 */
                        nullptr,             /* E_SELECT_OPT_INTO 18 */
                        nullptr              /*user for bigquery*/
                        );
    select_node->serialize_format = &SELECT_SERIALIZE_FORMAT;

    Node* direct_select_node = Node::makeNonTerminalNode(E_DIRECT_SELECT, E_DIRECT_SELECT_PROPERTY_CNT, nullptr, select_node, nullptr, nullptr,nullptr);
    direct_select_node->serialize_format = &SELECT_DIRECT_SERIALIZE_FORMAT;

    Node* select_with_parens_node = Node::makeNonTerminalNode(E_SELECT_WITH_PARENS, E_PARENS_PROPERTY_CNT, direct_select_node);
    select_with_parens_node->serialize_format = &SINGLE_WITH_PARENS_SERIALIZE_FORMAT;
    
    if ((yyvsp[-1].node))
    {
      (yyval.node) = Node::makeNonTerminalNode(E_ALIAS, E_ALIAS_PROPERTY_CNT, select_with_parens_node, (yyvsp[-1].node), (yyvsp[0].node), nullptr, nullptr);
    }
    else
    {
      Node* identifier_node = Node::makeTerminalNode(E_IDENTIFIER, (yyvsp[-2].node)->getChild(E_TABLE_IDENT_OBJECT)->text());
      (yyval.node) = Node::makeNonTerminalNode(E_ALIAS, E_ALIAS_PROPERTY_CNT, select_with_parens_node, identifier_node, (yyvsp[0].node), nullptr, nullptr);
    }

    (yyval.node)->serialize_format = &AS_SERIALIZE_FORMAT;
}
#line 5779 "sqlparser_sql2003_bison.cpp"
    break;

  case 120: /* table_primary_non_join: table_subquery opt_as_label pivot_clause  */
#line 1049 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_ALIAS, E_ALIAS_PROPERTY_CNT, (yyvsp[-2].node), (yyvsp[-1].node), (yyvsp[0].node), nullptr, nullptr);
    (yyval.node)->serialize_format = &AS_SERIALIZE_FORMAT;
}
#line 5788 "sqlparser_sql2003_bison.cpp"
    break;

  case 121: /* unpivot_clause: UNPIVOT '(' name_r FOR name_r IN simple_ident_list_with_parens ')' opt_as_label  */
#line 1057 "sqlparser_sql2003.y"
{
    Node* npvt = Node::makeTerminalNode(E_IDENTIFIER, "UNPIVOT");
    (yyval.node) = Node::makeNonTerminalNode(E_UNPIVOT_PIVOT, 5, npvt, (yyvsp[-6].node), (yyvsp[-4].node), (yyvsp[-2].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = &PIVOT_CLAUSE_SERIALIZE_FORMAT;
}
#line 5798 "sqlparser_sql2003_bison.cpp"
    break;

  case 122: /* pivot_clause: PIVOT '(' aggregate_windowed_function FOR name_r IN simple_ident_list_with_parens ')' opt_as_label  */
#line 1064 "sqlparser_sql2003.y"
{
    Node* npvt = Node::makeTerminalNode(E_IDENTIFIER, "PIVOT");
    (yyval.node) = Node::makeNonTerminalNode(E_UNPIVOT_PIVOT, 5, npvt, (yyvsp[-6].node), (yyvsp[-4].node), (yyvsp[-2].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = &PIVOT_CLAUSE_SERIALIZE_FORMAT;
}
#line 5808 "sqlparser_sql2003_bison.cpp"
    break;

  case 123: /* opt_simple_ident_list_with_parens: %empty  */
#line 1071 "sqlparser_sql2003.y"
                { (yyval.node) = nullptr; }
#line 5814 "sqlparser_sql2003_bison.cpp"
    break;

  case 125: /* column_ref: name_r  */
#line 1078 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_OP_NAME_FIELD, E_OP_NAME_FIELD_PROPERTY_CNT,
    			(yyvsp[0].node), nullptr, nullptr, nullptr, nullptr);
    (yyval.node)->serialize_format = &OP_NAME_FIELD_SERIALIZE_FORMAT_4;
}
#line 5824 "sqlparser_sql2003_bison.cpp"
    break;

  case 126: /* column_ref: name_r '.' name_r  */
#line 1084 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_OP_NAME_FIELD, E_OP_NAME_FIELD_PROPERTY_CNT,
    			(yyvsp[0].node), (yyvsp[-2].node), nullptr, nullptr, nullptr);
    (yyval.node)->serialize_format = &OP_NAME_FIELD_SERIALIZE_FORMAT_4;
}
#line 5834 "sqlparser_sql2003_bison.cpp"
    break;

  case 127: /* column_ref: name_r '.' name_r '.' name_r  */
#line 1090 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_OP_NAME_FIELD, E_OP_NAME_FIELD_PROPERTY_CNT,
    			(yyvsp[0].node), (yyvsp[-2].node), (yyvsp[-4].node), nullptr, nullptr);
    (yyval.node)->serialize_format = &OP_NAME_FIELD_SERIALIZE_FORMAT_4;
}
#line 5844 "sqlparser_sql2003_bison.cpp"
    break;

  case 128: /* column_ref: name_r '.' name_r '.' name_r '.' name_r  */
#line 1096 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_OP_NAME_FIELD, E_OP_NAME_FIELD_PROPERTY_CNT,
    			(yyvsp[0].node), (yyvsp[-2].node), (yyvsp[-4].node), (yyvsp[-6].node), nullptr);
    (yyval.node)->serialize_format = &OP_NAME_FIELD_SERIALIZE_FORMAT_4;
}
#line 5854 "sqlparser_sql2003_bison.cpp"
    break;

  case 129: /* column_ref: name_r '.' name_r '.' name_r '.' name_r '.' name_r  */
#line 1102 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_OP_NAME_FIELD, E_OP_NAME_FIELD_PROPERTY_CNT,
    			(yyvsp[0].node), (yyvsp[-2].node), (yyvsp[-4].node), (yyvsp[-6].node), (yyvsp[-8].node));
    (yyval.node)->serialize_format = &OP_NAME_FIELD_SERIALIZE_FORMAT_4;
}
#line 5864 "sqlparser_sql2003_bison.cpp"
    break;

  case 131: /* relation_factor_list: relation_factor_list ',' relation_factor  */
#line 1166 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_RELATION_FACTOR_LIST, E_LIST_PROPERTY_CNT, (yyvsp[-2].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = &COMMA_LIST_SERIALIZE_FORMAT;
}
#line 5873 "sqlparser_sql2003_bison.cpp"
    break;

  case 132: /* relation_factor: name_r  */
#line 1175 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_TABLE_IDENT, E_TABLE_IDENT_PROPERTY_CNT, (yyvsp[0].node), nullptr, nullptr, nullptr);
    (yyval.node)->serialize_format = &SINGLE_SERIALIZE_FORMAT;
}
#line 5882 "sqlparser_sql2003_bison.cpp"
    break;

  case 133: /* relation_factor: name_r '.' name_r  */
#line 1180 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_TABLE_IDENT, E_TABLE_IDENT_PROPERTY_CNT, (yyvsp[0].node), (yyvsp[-2].node), nullptr, nullptr);
    (yyval.node)->serialize_format = &TABLE_IDENT_SERIALIZE_FORMAT_1;
}
#line 5891 "sqlparser_sql2003_bison.cpp"
    break;

  case 134: /* relation_factor: name_r '.' name_r '.' name_r  */
#line 1185 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_TABLE_IDENT, E_TABLE_IDENT_PROPERTY_CNT, (yyvsp[0].node), (yyvsp[-2].node), (yyvsp[-4].node), nullptr);
    (yyval.node)->serialize_format = &TABLE_IDENT_SERIALIZE_FORMAT_2;
}
#line 5900 "sqlparser_sql2003_bison.cpp"
    break;

  case 135: /* relation_factor: name_r '.' '.' name_r  */
#line 1190 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_TABLE_IDENT, E_TABLE_IDENT_PROPERTY_CNT, (yyvsp[0].node), nullptr, (yyvsp[-3].node), nullptr);
    (yyval.node)->serialize_format = &TABLE_IDENT_SERIALIZE_FORMAT_2;
}
#line 5909 "sqlparser_sql2003_bison.cpp"
    break;

  case 136: /* relation_factor: name_r '.' name_r '.' name_r '.' name_r  */
#line 1195 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_TABLE_IDENT, E_TABLE_IDENT_PROPERTY_CNT, (yyvsp[0].node), (yyvsp[-2].node), (yyvsp[-4].node), (yyvsp[-6].node));
    (yyval.node)->serialize_format = &TABLE_IDENT_SERIALIZE_FORMAT_3;
}
#line 5918 "sqlparser_sql2003_bison.cpp"
    break;

  case 137: /* relation_factor: name_r '.' '.' name_r '.' name_r  */
#line 1200 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_TABLE_IDENT, E_TABLE_IDENT_PROPERTY_CNT, (yyvsp[0].node), (yyvsp[-2].node), nullptr, (yyvsp[-5].node));
    (yyval.node)->serialize_format = &TABLE_IDENT_SERIALIZE_FORMAT_3;
}
#line 5927 "sqlparser_sql2003_bison.cpp"
    break;

  case 138: /* relation_factor: name_r '.' name_r '.' '.' name_r  */
#line 1205 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_TABLE_IDENT, E_TABLE_IDENT_PROPERTY_CNT, (yyvsp[0].node), nullptr, (yyvsp[-3].node), (yyvsp[-5].node));
    (yyval.node)->serialize_format = &TABLE_IDENT_SERIALIZE_FORMAT_3;
}
#line 5936 "sqlparser_sql2003_bison.cpp"
    break;

  case 139: /* relation_factor: name_r '.' '.' '.' name_r  */
#line 1210 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_TABLE_IDENT, E_TABLE_IDENT_PROPERTY_CNT, (yyvsp[0].node), nullptr, nullptr, (yyvsp[-4].node));
    (yyval.node)->serialize_format = &TABLE_IDENT_SERIALIZE_FORMAT_3;
}
#line 5945 "sqlparser_sql2003_bison.cpp"
    break;

  case 142: /* qualified_join: table_reference join_type JOIN table_reference ON search_condition  */
#line 1225 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_JOINED_TABLE, E_JOINED_TABLE_PROPERTY_CNT, (yyvsp[-4].node), (yyvsp[-5].node), (yyvsp[-2].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = &JOINED_TB_1_SERIALIZE_FORMAT;
}
#line 5954 "sqlparser_sql2003_bison.cpp"
    break;

  case 143: /* qualified_join: table_reference JOIN table_reference ON search_condition  */
#line 1230 "sqlparser_sql2003.y"
{
    Node* nd = Node::makeTerminalNode(E_JOIN_INNER, "");
    (yyval.node) = Node::makeNonTerminalNode(E_JOINED_TABLE, E_JOINED_TABLE_PROPERTY_CNT, nd, (yyvsp[-4].node), (yyvsp[-2].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = &JOINED_TB_1_SERIALIZE_FORMAT;
}
#line 5964 "sqlparser_sql2003_bison.cpp"
    break;

  case 144: /* cross_join: table_reference CROSS JOIN table_primary  */
#line 1251 "sqlparser_sql2003.y"
{
    Node* nd = Node::makeTerminalNode(E_JOIN_CROSS, "CROSS");
    (yyval.node) = Node::makeNonTerminalNode(E_JOINED_TABLE, E_JOINED_TABLE_PROPERTY_CNT, nd, (yyvsp[-3].node), (yyvsp[0].node), nullptr);
    (yyval.node)->serialize_format = &JOINED_TB_2_SERIALIZE_FORMAT;
}
#line 5974 "sqlparser_sql2003_bison.cpp"
    break;

  case 145: /* join_type: FULL join_outer  */
#line 1276 "sqlparser_sql2003.y"
{
    if ((yyvsp[0].ival))
    {
        (yyval.node) = Node::makeTerminalNode(E_JOIN_FULL, "FULL OUTER");
    }
    else
    {
        (yyval.node) = Node::makeTerminalNode(E_JOIN_FULL, "FULL");
    }
}
#line 5989 "sqlparser_sql2003_bison.cpp"
    break;

  case 146: /* join_type: LEFT join_outer  */
#line 1287 "sqlparser_sql2003.y"
{
    if ((yyvsp[0].ival))
    {
        (yyval.node) = Node::makeTerminalNode(E_JOIN_LEFT, "LEFT OUTER");
    }
    else
    {
        (yyval.node) = Node::makeTerminalNode(E_JOIN_LEFT, "LEFT");
    }
}
#line 6004 "sqlparser_sql2003_bison.cpp"
    break;

  case 147: /* join_type: RIGHT join_outer  */
#line 1298 "sqlparser_sql2003.y"
{
    if ((yyvsp[0].ival))
    {
        (yyval.node) = Node::makeTerminalNode(E_JOIN_RIGHT, "RIGHT OUTER");
    }
    else
    {
        (yyval.node) = Node::makeTerminalNode(E_JOIN_RIGHT, "RIGHT");
    }
}
#line 6019 "sqlparser_sql2003_bison.cpp"
    break;

  case 148: /* join_type: INNER  */
#line 1309 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeTerminalNode(E_JOIN_INNER, "INNER");
}
#line 6027 "sqlparser_sql2003_bison.cpp"
    break;

  case 149: /* join_outer: %empty  */
#line 1315 "sqlparser_sql2003.y"
                                { (yyval.ival) = 0; }
#line 6033 "sqlparser_sql2003_bison.cpp"
    break;

  case 150: /* join_outer: OUTER  */
#line 1316 "sqlparser_sql2003.y"
                                { (yyval.ival) = 1; /*this is a flag*/}
#line 6039 "sqlparser_sql2003_bison.cpp"
    break;

  case 152: /* search_condition: search_condition OR boolean_term  */
#line 1323 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_OP_OR, E_OP_BINARY_PROPERTY_CNT, (yyvsp[-2].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = Node::op_serialize_format(E_OP_OR);
}
#line 6048 "sqlparser_sql2003_bison.cpp"
    break;

  case 154: /* boolean_term: boolean_term AND boolean_factor  */
#line 1332 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_OP_AND, E_OP_BINARY_PROPERTY_CNT, (yyvsp[-2].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = Node::op_serialize_format(E_OP_AND);
}
#line 6057 "sqlparser_sql2003_bison.cpp"
    break;

  case 156: /* boolean_factor: NOT boolean_factor  */
#line 1341 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_OP_NOT, E_OP_UNARY_PROPERTY_CNT, (yyvsp[0].node));
    (yyval.node)->serialize_format = Node::op_serialize_format(E_OP_NOT);
}
#line 6066 "sqlparser_sql2003_bison.cpp"
    break;

  case 158: /* boolean_test: boolean_primary IS truth_value  */
#line 1350 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_OP_IS, E_OP_BINARY_PROPERTY_CNT, (yyvsp[-2].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = Node::op_serialize_format(E_OP_IS);
}
#line 6075 "sqlparser_sql2003_bison.cpp"
    break;

  case 159: /* boolean_test: boolean_primary IS NOT truth_value  */
#line 1355 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_OP_IS_NOT, E_OP_BINARY_PROPERTY_CNT, (yyvsp[-3].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = Node::op_serialize_format(E_OP_IS_NOT);
}
#line 6084 "sqlparser_sql2003_bison.cpp"
    break;

  case 161: /* boolean_primary: '(' search_condition ')'  */
#line 1364 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_EXPR_LIST_WITH_PARENS, E_PARENS_PROPERTY_CNT, (yyvsp[-1].node));
    (yyval.node)->serialize_format = &SINGLE_WITH_PARENS_SERIALIZE_FORMAT;
}
#line 6093 "sqlparser_sql2003_bison.cpp"
    break;

  case 169: /* comparison_predicate: row_expr comp_op row_expr  */
#line 1392 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode((yyvsp[-1].nodetype), E_OP_BINARY_PROPERTY_CNT, (yyvsp[-2].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = Node::op_serialize_format((yyvsp[-1].nodetype));
}
#line 6102 "sqlparser_sql2003_bison.cpp"
    break;

  case 170: /* quantified_comparison_predicate: row_expr comp_all_some_any_op table_subquery  */
#line 1400 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode((yyvsp[-1].nodetype), E_OP_BINARY_PROPERTY_CNT, (yyvsp[-2].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = Node::op_serialize_format((yyvsp[-1].nodetype));
}
#line 6111 "sqlparser_sql2003_bison.cpp"
    break;

  case 171: /* between_predicate: row_expr BETWEEN row_expr AND row_expr  */
#line 1408 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_OP_BTW, E_OP_TERNARY_PROPERTY_CNT, (yyvsp[-4].node), (yyvsp[-2].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = Node::op_serialize_format(E_OP_BTW);
}
#line 6120 "sqlparser_sql2003_bison.cpp"
    break;

  case 172: /* between_predicate: row_expr NOT BETWEEN row_expr AND row_expr  */
#line 1413 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_OP_NOT_BTW, E_OP_TERNARY_PROPERTY_CNT, (yyvsp[-5].node), (yyvsp[-2].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = Node::op_serialize_format(E_OP_NOT_BTW);
}
#line 6129 "sqlparser_sql2003_bison.cpp"
    break;

  case 173: /* like_predicate: row_expr LIKE row_expr  */
#line 1421 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_OP_LIKE, E_OP_BINARY_PROPERTY_CNT, (yyvsp[-2].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = Node::op_serialize_format(E_OP_LIKE);
}
#line 6138 "sqlparser_sql2003_bison.cpp"
    break;

  case 174: /* like_predicate: row_expr LIKE row_expr ESCAPE row_expr  */
#line 1426 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_OP_LIKE, E_OP_TERNARY_PROPERTY_CNT, (yyvsp[-4].node), (yyvsp[-2].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = Node::op_serialize_format(E_OP_LIKE);
}
#line 6147 "sqlparser_sql2003_bison.cpp"
    break;

  case 175: /* like_predicate: row_expr NOT LIKE row_expr  */
#line 1431 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_OP_NOT_LIKE, E_OP_BINARY_PROPERTY_CNT, (yyvsp[-3].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = Node::op_serialize_format(E_OP_NOT_LIKE);
}
#line 6156 "sqlparser_sql2003_bison.cpp"
    break;

  case 176: /* like_predicate: row_expr NOT LIKE row_expr ESCAPE row_expr  */
#line 1436 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_OP_NOT_LIKE, E_OP_TERNARY_PROPERTY_CNT, (yyvsp[-5].node), (yyvsp[-2].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = Node::op_serialize_format(E_OP_NOT_LIKE);
}
#line 6165 "sqlparser_sql2003_bison.cpp"
    break;

  case 177: /* in_predicate: row_expr IN in_expr  */
#line 1444 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_OP_IN, E_OP_BINARY_PROPERTY_CNT, (yyvsp[-2].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = Node::op_serialize_format(E_OP_IN);
}
#line 6174 "sqlparser_sql2003_bison.cpp"
    break;

  case 178: /* in_predicate: row_expr NOT IN in_expr  */
#line 1449 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_OP_NOT_IN, E_OP_BINARY_PROPERTY_CNT, (yyvsp[-3].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = Node::op_serialize_format(E_OP_NOT_IN);
}
#line 6183 "sqlparser_sql2003_bison.cpp"
    break;

  case 179: /* null_predicate: row_expr IS NULLX  */
#line 1457 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_OP_IS, E_OP_BINARY_PROPERTY_CNT, (yyvsp[-2].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = Node::op_serialize_format(E_OP_IS);
}
#line 6192 "sqlparser_sql2003_bison.cpp"
    break;

  case 180: /* null_predicate: row_expr IS NOT NULLX  */
#line 1462 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_OP_IS_NOT, E_OP_BINARY_PROPERTY_CNT, (yyvsp[-3].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = Node::op_serialize_format(E_OP_IS_NOT);
}
#line 6201 "sqlparser_sql2003_bison.cpp"
    break;

  case 181: /* exists_predicate: EXISTS table_subquery  */
#line 1470 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_OP_EXISTS, E_OP_UNARY_PROPERTY_CNT, (yyvsp[0].node));
    (yyval.node)->serialize_format = Node::op_serialize_format(E_OP_EXISTS);
}
#line 6210 "sqlparser_sql2003_bison.cpp"
    break;

  case 183: /* row_expr: row_expr collate_clause  */
#line 1480 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_OP_COLLATE, E_OP_BINARY_PROPERTY_CNT, (yyvsp[-1].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = &DOUBLE_SERIALIZE_FORMAT;
}
#line 6219 "sqlparser_sql2003_bison.cpp"
    break;

  case 184: /* row_expr: row_expr cnn_op factor0  */
#line 1485 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_OP_CNN, E_OP_BINARY_PROPERTY_CNT, (yyvsp[-2].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = Node::op_serialize_format((yyvsp[-1].nodetype));
}
#line 6228 "sqlparser_sql2003_bison.cpp"
    break;

  case 186: /* factor0: factor0 plus_minus_op factor1  */
#line 1494 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_OP_ADD, E_OP_BINARY_PROPERTY_CNT, (yyvsp[-2].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = Node::op_serialize_format((yyvsp[-1].nodetype));
}
#line 6237 "sqlparser_sql2003_bison.cpp"
    break;

  case 188: /* factor1: factor1 star_div_percent_mod_op factor2  */
#line 1503 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_OP_MUL, E_OP_BINARY_PROPERTY_CNT, (yyvsp[-2].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = Node::op_serialize_format((yyvsp[-1].nodetype));
}
#line 6246 "sqlparser_sql2003_bison.cpp"
    break;

  case 190: /* factor2: factor2 '^' factor3  */
#line 1512 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_OP_POW, E_OP_BINARY_PROPERTY_CNT, (yyvsp[-2].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = Node::op_serialize_format(E_OP_POW);
}
#line 6255 "sqlparser_sql2003_bison.cpp"
    break;

  case 192: /* factor3: '+' factor3  */
#line 1521 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_OP_POS, E_OP_UNARY_PROPERTY_CNT, (yyvsp[0].node));
    (yyval.node)->serialize_format = Node::op_serialize_format(E_OP_POS);
}
#line 6264 "sqlparser_sql2003_bison.cpp"
    break;

  case 193: /* factor3: '-' factor3  */
#line 1526 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_OP_NEG, E_OP_UNARY_PROPERTY_CNT, (yyvsp[0].node));
    (yyval.node)->serialize_format = Node::op_serialize_format(E_OP_NEG);
}
#line 6273 "sqlparser_sql2003_bison.cpp"
    break;

  case 197: /* factor4: '(' row_expr_list ')'  */
#line 1537 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_EXPR_LIST_WITH_PARENS, E_PARENS_PROPERTY_CNT, (yyvsp[-1].node));
    (yyval.node)->serialize_format = &SINGLE_WITH_PARENS_SERIALIZE_FORMAT;
}
#line 6282 "sqlparser_sql2003_bison.cpp"
    break;

  case 201: /* row_expr_list: row_expr_list ',' row_expr  */
#line 1548 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_EXPR_LIST, E_LIST_PROPERTY_CNT, (yyvsp[-2].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = &COMMA_LIST_SERIALIZE_FORMAT;
}
#line 6291 "sqlparser_sql2003_bison.cpp"
    break;

  case 203: /* in_expr: '(' row_expr_list ')'  */
#line 1557 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_EXPR_LIST_WITH_PARENS, E_PARENS_PROPERTY_CNT, (yyvsp[-1].node));
    (yyval.node)->serialize_format = &SINGLE_WITH_PARENS_SERIALIZE_FORMAT;
}
#line 6300 "sqlparser_sql2003_bison.cpp"
    break;

  case 206: /* comp_op: COMP_LE  */
#line 1567 "sqlparser_sql2003.y"
                        { (yyval.nodetype) = E_OP_LE; }
#line 6306 "sqlparser_sql2003_bison.cpp"
    break;

  case 207: /* comp_op: COMP_LT  */
#line 1568 "sqlparser_sql2003.y"
                        { (yyval.nodetype) = E_OP_LT; }
#line 6312 "sqlparser_sql2003_bison.cpp"
    break;

  case 208: /* comp_op: COMP_GE  */
#line 1569 "sqlparser_sql2003.y"
                        { (yyval.nodetype) = E_OP_GE; }
#line 6318 "sqlparser_sql2003_bison.cpp"
    break;

  case 209: /* comp_op: COMP_GT  */
#line 1570 "sqlparser_sql2003.y"
                        { (yyval.nodetype) = E_OP_GT; }
#line 6324 "sqlparser_sql2003_bison.cpp"
    break;

  case 210: /* comp_op: COMP_EQ  */
#line 1571 "sqlparser_sql2003.y"
                        { (yyval.nodetype) = E_OP_EQ; }
#line 6330 "sqlparser_sql2003_bison.cpp"
    break;

  case 211: /* comp_op: COMP_NE  */
#line 1572 "sqlparser_sql2003.y"
                        { (yyval.nodetype) = E_OP_NE; }
#line 6336 "sqlparser_sql2003_bison.cpp"
    break;

  case 212: /* comp_op: COMP_GT COMP_EQ  */
#line 1573 "sqlparser_sql2003.y"
                        { (yyval.nodetype) = E_OP_GE; }
#line 6342 "sqlparser_sql2003_bison.cpp"
    break;

  case 213: /* comp_op: COMP_LT COMP_EQ  */
#line 1574 "sqlparser_sql2003.y"
                        { (yyval.nodetype) = E_OP_LE; }
#line 6348 "sqlparser_sql2003_bison.cpp"
    break;

  case 214: /* comp_op: COMP_LT COMP_GT  */
#line 1575 "sqlparser_sql2003.y"
                        { (yyval.nodetype) = E_OP_NE; }
#line 6354 "sqlparser_sql2003_bison.cpp"
    break;

  case 215: /* comp_op: '!' COMP_EQ  */
#line 1576 "sqlparser_sql2003.y"
                        { (yyval.nodetype) = E_OP_NE; }
#line 6360 "sqlparser_sql2003_bison.cpp"
    break;

  case 216: /* cnn_op: CNNOP  */
#line 1580 "sqlparser_sql2003.y"
                        { (yyval.nodetype) = E_OP_CNN; }
#line 6366 "sqlparser_sql2003_bison.cpp"
    break;

  case 217: /* cnn_op: '|' '|'  */
#line 1581 "sqlparser_sql2003.y"
                        { (yyval.nodetype) = E_OP_CNN; }
#line 6372 "sqlparser_sql2003_bison.cpp"
    break;

  case 218: /* comp_all_some_any_op: comp_op all_some_any  */
#line 1587 "sqlparser_sql2003.y"
{
    (yyval.nodetype) = Node::comp_all_some_any_op_form((yyvsp[-1].nodetype), (yyvsp[0].ival));
}
#line 6380 "sqlparser_sql2003_bison.cpp"
    break;

  case 219: /* plus_minus_op: '+'  */
#line 1593 "sqlparser_sql2003.y"
                { (yyval.nodetype) = E_OP_ADD; }
#line 6386 "sqlparser_sql2003_bison.cpp"
    break;

  case 220: /* plus_minus_op: '-'  */
#line 1594 "sqlparser_sql2003.y"
                { (yyval.nodetype) = E_OP_MINUS; }
#line 6392 "sqlparser_sql2003_bison.cpp"
    break;

  case 221: /* star_div_percent_mod_op: '*'  */
#line 1598 "sqlparser_sql2003.y"
                { (yyval.nodetype) = E_OP_MUL; }
#line 6398 "sqlparser_sql2003_bison.cpp"
    break;

  case 222: /* star_div_percent_mod_op: '/'  */
#line 1599 "sqlparser_sql2003.y"
                { (yyval.nodetype) = E_OP_DIV; }
#line 6404 "sqlparser_sql2003_bison.cpp"
    break;

  case 223: /* star_div_percent_mod_op: '%'  */
#line 1600 "sqlparser_sql2003.y"
                { (yyval.nodetype) = E_OP_REM; }
#line 6410 "sqlparser_sql2003_bison.cpp"
    break;

  case 224: /* star_div_percent_mod_op: MOD  */
#line 1601 "sqlparser_sql2003.y"
                { (yyval.nodetype) = E_OP_MOD; }
#line 6416 "sqlparser_sql2003_bison.cpp"
    break;

  case 225: /* star_div_percent_mod_op: '&'  */
#line 1602 "sqlparser_sql2003.y"
            { (yyval.nodetype) = E_OP_AND_SPC; }
#line 6422 "sqlparser_sql2003_bison.cpp"
    break;

  case 233: /* expr_const: DEFAULT  */
#line 1613 "sqlparser_sql2003.y"
                        { (yyval.node) = Node::makeTerminalNode(E_STRING, "DEFAULT"); }
#line 6428 "sqlparser_sql2003_bison.cpp"
    break;

  case 234: /* case_expr: CASE case_arg when_clause_list case_default END  */
#line 1619 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_CASE, E_CASE_PROPERTY_CNT, (yyvsp[-3].node), (yyvsp[-2].node), (yyvsp[-1].node));
    (yyval.node)->serialize_format = Node::op_serialize_format(E_CASE);
}
#line 6437 "sqlparser_sql2003_bison.cpp"
    break;

  case 235: /* case_arg: %empty  */
#line 1626 "sqlparser_sql2003.y"
                { (yyval.node) = nullptr; }
#line 6443 "sqlparser_sql2003_bison.cpp"
    break;

  case 238: /* when_clause_list: when_clause_list when_clause  */
#line 1633 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_WHEN_LIST, E_LIST_PROPERTY_CNT, (yyvsp[-1].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = &SPACE_LIST_SERIALIZE_FORMAT;
}
#line 6452 "sqlparser_sql2003_bison.cpp"
    break;

  case 239: /* when_clause: WHEN search_condition THEN row_expr  */
#line 1641 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_WHEN, E_WHEN_PROPERTY_CNT, (yyvsp[-2].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = &WHEN_SERIALIZE_FORMAT;
}
#line 6461 "sqlparser_sql2003_bison.cpp"
    break;

  case 240: /* when_clause: WHEN row_expr THEN row_expr  */
#line 1646 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_WHEN, E_WHEN_PROPERTY_CNT, (yyvsp[-2].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = &WHEN_SERIALIZE_FORMAT;
}
#line 6470 "sqlparser_sql2003_bison.cpp"
    break;

  case 241: /* case_default: %empty  */
#line 1653 "sqlparser_sql2003.y"
                { (yyval.node) = nullptr; }
#line 6476 "sqlparser_sql2003_bison.cpp"
    break;

  case 242: /* case_default: ELSE row_expr  */
#line 1655 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_CASE_DEFAULT, E_CASE_DEFAULT_PROPERTY_CNT, (yyvsp[0].node));
    (yyval.node)->serialize_format = &ELSE_SERIALIZE_FORMAT;
}
#line 6485 "sqlparser_sql2003_bison.cpp"
    break;

  case 247: /* aggregate_windowed_function: aggregate_function_name '(' row_expr ')' over_clause  */
#line 1672 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_FUN_CALL, E_FUN_CALL_PROPERTY_CNT,
    		(yyvsp[-4].node), (yyvsp[-2].node), (yyvsp[0].node), nullptr, nullptr);
    (yyval.node)->serialize_format = &FUN_CALL_1_OVER_SERIALIZE_FORMAT;
}
#line 6495 "sqlparser_sql2003_bison.cpp"
    break;

  case 248: /* aggregate_windowed_function: aggregate_function_name '(' row_expr ')'  */
#line 1678 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_FUN_CALL, E_FUN_CALL_PROPERTY_CNT,
    		(yyvsp[-3].node), (yyvsp[-1].node), nullptr, nullptr, nullptr);
    (yyval.node)->serialize_format = &FUN_CALL_1_OVER_SERIALIZE_FORMAT;
}
#line 6505 "sqlparser_sql2003_bison.cpp"
    break;

  case 249: /* aggregate_windowed_function: aggregate_function_name '(' distinct_or_all row_expr ')' over_clause  */
#line 1684 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_FUN_CALL, E_FUN_CALL_PROPERTY_CNT,
    		(yyvsp[-5].node), (yyvsp[-3].node), (yyvsp[-2].node), nullptr, (yyvsp[0].node));
    (yyval.node)->serialize_format = &FUN_CALL_DIS_OVER_SERIALIZE_FORMAT;
}
#line 6515 "sqlparser_sql2003_bison.cpp"
    break;

  case 250: /* aggregate_windowed_function: aggregate_function_name '(' distinct_or_all row_expr ')'  */
#line 1690 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_FUN_CALL, E_FUN_CALL_PROPERTY_CNT,
    		(yyvsp[-4].node), (yyvsp[-2].node), (yyvsp[-1].node), nullptr, nullptr);
    (yyval.node)->serialize_format = &FUN_CALL_DIS_OVER_SERIALIZE_FORMAT;
}
#line 6525 "sqlparser_sql2003_bison.cpp"
    break;

  case 251: /* aggregate_windowed_function: aggregate_function_name '(' '*' ')' over_clause  */
#line 1696 "sqlparser_sql2003.y"
{
    if (!Node::CHECK_FUNCTION_CALL_WITH_STAR((yyvsp[-4].node)))
    {
        yyerror(&(yylsp[-4]), result, scanner, "error use *");
        YYABORT;
    }
    Node* star = Node::makeTerminalNode(E_STAR, "*");
    (yyval.node) = Node::makeNonTerminalNode(E_FUN_CALL, E_FUN_CALL_PROPERTY_CNT,
    		(yyvsp[-4].node), star, (yyvsp[0].node), nullptr, nullptr);
    (yyval.node)->serialize_format = &FUN_CALL_1_OVER_SERIALIZE_FORMAT;
}
#line 6541 "sqlparser_sql2003_bison.cpp"
    break;

  case 252: /* aggregate_windowed_function: aggregate_function_name '(' '*' ')'  */
#line 1708 "sqlparser_sql2003.y"
{
    if (!Node::CHECK_FUNCTION_CALL_WITH_STAR((yyvsp[-3].node)))
    {
        yyerror(&(yylsp[-3]), result, scanner, "error use *");
        YYABORT;
    }
    Node* star = Node::makeTerminalNode(E_STAR, "*");
    (yyval.node) = Node::makeNonTerminalNode(E_FUN_CALL, E_FUN_CALL_PROPERTY_CNT,
    		(yyvsp[-3].node), star, nullptr, nullptr, nullptr);
    (yyval.node)->serialize_format = &FUN_CALL_1_OVER_SERIALIZE_FORMAT;
}
#line 6557 "sqlparser_sql2003_bison.cpp"
    break;

  case 253: /* aggregate_function_name: AVG  */
#line 1722 "sqlparser_sql2003.y"
                        { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "AVG"); }
#line 6563 "sqlparser_sql2003_bison.cpp"
    break;

  case 254: /* aggregate_function_name: MAX  */
#line 1723 "sqlparser_sql2003.y"
                        { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "MAX"); }
#line 6569 "sqlparser_sql2003_bison.cpp"
    break;

  case 255: /* aggregate_function_name: MIN  */
#line 1724 "sqlparser_sql2003.y"
                        { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "MIN"); }
#line 6575 "sqlparser_sql2003_bison.cpp"
    break;

  case 256: /* aggregate_function_name: SUM  */
#line 1725 "sqlparser_sql2003.y"
                        { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "SUM"); }
#line 6581 "sqlparser_sql2003_bison.cpp"
    break;

  case 257: /* aggregate_function_name: STDDEV_POP  */
#line 1726 "sqlparser_sql2003.y"
                        { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "STDDEV_POP"); }
#line 6587 "sqlparser_sql2003_bison.cpp"
    break;

  case 258: /* aggregate_function_name: STDDEV_SAMP  */
#line 1727 "sqlparser_sql2003.y"
                        { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "STDDEV_SAMP"); }
#line 6593 "sqlparser_sql2003_bison.cpp"
    break;

  case 259: /* aggregate_function_name: VAR_POP  */
#line 1728 "sqlparser_sql2003.y"
                        { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "VAR_POP"); }
#line 6599 "sqlparser_sql2003_bison.cpp"
    break;

  case 260: /* aggregate_function_name: VAR_SAMP  */
#line 1729 "sqlparser_sql2003.y"
                        { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "VAR_SAMP"); }
#line 6605 "sqlparser_sql2003_bison.cpp"
    break;

  case 261: /* aggregate_function_name: COUNT  */
#line 1730 "sqlparser_sql2003.y"
                        { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "COUNT"); }
#line 6611 "sqlparser_sql2003_bison.cpp"
    break;

  case 262: /* aggregate_function_name: GROUPING  */
#line 1731 "sqlparser_sql2003.y"
                        { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "GROUPING"); }
#line 6617 "sqlparser_sql2003_bison.cpp"
    break;

  case 263: /* ranking_windowed_function: ranking_function_name '(' ')' over_clause  */
#line 1736 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_FUN_CALL, E_FUN_CALL_PROPERTY_CNT,
    		(yyvsp[-3].node), nullptr, (yyvsp[0].node), nullptr, nullptr);
    (yyval.node)->serialize_format = &FUN_CALL_1_OVER_SERIALIZE_FORMAT;
}
#line 6627 "sqlparser_sql2003_bison.cpp"
    break;

  case 264: /* ranking_function_name: RANK  */
#line 1744 "sqlparser_sql2003.y"
                        { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "RANK"); }
#line 6633 "sqlparser_sql2003_bison.cpp"
    break;

  case 265: /* ranking_function_name: DENSE_RANK  */
#line 1745 "sqlparser_sql2003.y"
                        { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "DENSE_RANK"); }
#line 6639 "sqlparser_sql2003_bison.cpp"
    break;

  case 266: /* ranking_function_name: PERCENT_RANK  */
#line 1746 "sqlparser_sql2003.y"
                        { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "PERCENT_RANK"); }
#line 6645 "sqlparser_sql2003_bison.cpp"
    break;

  case 267: /* ranking_function_name: CUME_DIST  */
#line 1747 "sqlparser_sql2003.y"
                        { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "CUME_DIST"); }
#line 6651 "sqlparser_sql2003_bison.cpp"
    break;

  case 268: /* ranking_function_name: ROW_NUMBER  */
#line 1748 "sqlparser_sql2003.y"
                        { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "ROW_NUMBER"); }
#line 6657 "sqlparser_sql2003_bison.cpp"
    break;

  case 269: /* over_clause: OVER window_name  */
#line 1753 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "OVER "+ (yyvsp[0].node)->text()); delete((yyvsp[0].node));
}
#line 6665 "sqlparser_sql2003_bison.cpp"
    break;

  case 270: /* over_clause: OVER window_specification  */
#line 1757 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_OVER_CLAUSE, 1, (yyvsp[0].node));
    (yyval.node)->serialize_format = &OVER_CLAUSE_SERIALIZE_FORMAT;
}
#line 6674 "sqlparser_sql2003_bison.cpp"
    break;

  case 271: /* window_specification: '(' window_specification_details ')'  */
#line 1766 "sqlparser_sql2003.y"
{ (yyval.node) = (yyvsp[-1].node); }
#line 6680 "sqlparser_sql2003_bison.cpp"
    break;

  case 273: /* window_specification_details: opt_existing_window_name opt_window_partition_clause opt_order_by opt_window_frame_clause  */
#line 1775 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_WINDOW_SPECIFIC, E_WINDOW_SPECIFIC_PROPERTY_CNT, (yyvsp[-3].node), (yyvsp[-2].node), (yyvsp[-1].node), (yyvsp[0].node));
    (yyval.node)->serialize_format = &WINDOW_SPECIFIC_CLAUSE_SERIALIZE_FORMAT;
}
#line 6689 "sqlparser_sql2003_bison.cpp"
    break;

  case 274: /* opt_existing_window_name: %empty  */
#line 1782 "sqlparser_sql2003.y"
                { (yyval.node) = nullptr; }
#line 6695 "sqlparser_sql2003_bison.cpp"
    break;

  case 276: /* opt_window_partition_clause: %empty  */
#line 1787 "sqlparser_sql2003.y"
                { (yyval.node) = nullptr; }
#line 6701 "sqlparser_sql2003_bison.cpp"
    break;

  case 277: /* opt_window_partition_clause: PARTITION BY row_expr_list  */
#line 1789 "sqlparser_sql2003.y"
{ (yyval.node) = (yyvsp[0].node); }
#line 6707 "sqlparser_sql2003_bison.cpp"
    break;

  case 278: /* opt_window_frame_clause: %empty  */
#line 1793 "sqlparser_sql2003.y"
                { (yyval.node) = nullptr; }
#line 6713 "sqlparser_sql2003_bison.cpp"
    break;

  case 279: /* opt_window_frame_clause: window_frame_units window_frame_extent opt_window_frame_exclusion  */
#line 1795 "sqlparser_sql2003.y"
{
    std::string s3 = (yyvsp[0].node) ? (yyvsp[0].node)->text() : "";
    (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, (yyvsp[-2].node)->text()+" "+(yyvsp[-1].node)->text()+" "+s3);
    delete((yyvsp[-2].node)); delete((yyvsp[-1].node)); delete((yyvsp[0].node));
}
#line 6723 "sqlparser_sql2003_bison.cpp"
    break;

  case 280: /* window_frame_units: ROWS  */
#line 1803 "sqlparser_sql2003.y"
                { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER,"ROWS"); }
#line 6729 "sqlparser_sql2003_bison.cpp"
    break;

  case 281: /* window_frame_units: RANGE  */
#line 1804 "sqlparser_sql2003.y"
                { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER,"RANGE"); }
#line 6735 "sqlparser_sql2003_bison.cpp"
    break;

  case 284: /* window_frame_start: UNBOUNDED PRECEDING  */
#line 1813 "sqlparser_sql2003.y"
                                { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER,"UNBOUNDED PRECEDING"); }
#line 6741 "sqlparser_sql2003_bison.cpp"
    break;

  case 285: /* window_frame_start: CURRENT ROW  */
#line 1814 "sqlparser_sql2003.y"
                                { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER,"CURRENT ROW"); }
#line 6747 "sqlparser_sql2003_bison.cpp"
    break;

  case 287: /* window_frame_preceding: expr_const PRECEDING  */
#line 1819 "sqlparser_sql2003.y"
                                { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER,(yyvsp[-1].node)->text()+" PRECEDING"); delete((yyvsp[-1].node)); }
#line 6753 "sqlparser_sql2003_bison.cpp"
    break;

  case 288: /* window_frame_between: BETWEEN window_frame_bound AND window_frame_bound  */
#line 1824 "sqlparser_sql2003.y"
{ (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "BETWEEN "+(yyvsp[-2].node)->text()+" AND "+(yyvsp[0].node)->text()); delete((yyvsp[-2].node)); delete((yyvsp[0].node)); }
#line 6759 "sqlparser_sql2003_bison.cpp"
    break;

  case 290: /* window_frame_bound: UNBOUNDED FOLLOWING  */
#line 1829 "sqlparser_sql2003.y"
                                { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER,"UNBOUNDED FOLLOWING"); }
#line 6765 "sqlparser_sql2003_bison.cpp"
    break;

  case 292: /* window_frame_following: expr_const FOLLOWING  */
#line 1834 "sqlparser_sql2003.y"
                                { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER,(yyvsp[-1].node)->text()+" FOLLOWING"); delete((yyvsp[-1].node)); }
#line 6771 "sqlparser_sql2003_bison.cpp"
    break;

  case 293: /* opt_window_frame_exclusion: %empty  */
#line 1838 "sqlparser_sql2003.y"
                { (yyval.node) = nullptr; }
#line 6777 "sqlparser_sql2003_bison.cpp"
    break;

  case 294: /* opt_window_frame_exclusion: EXCLUDE CURRENT ROW  */
#line 1839 "sqlparser_sql2003.y"
                                { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER,"EXCLUDE CURRENT ROW"); }
#line 6783 "sqlparser_sql2003_bison.cpp"
    break;

  case 295: /* opt_window_frame_exclusion: EXCLUDE GROUP  */
#line 1840 "sqlparser_sql2003.y"
                                { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER,"EXCLUDE GROUP"); }
#line 6789 "sqlparser_sql2003_bison.cpp"
    break;

  case 296: /* opt_window_frame_exclusion: EXCLUDE TIES  */
#line 1841 "sqlparser_sql2003.y"
                                { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER,"EXCLUDE TIES"); }
#line 6795 "sqlparser_sql2003_bison.cpp"
    break;

  case 297: /* opt_window_frame_exclusion: EXCLUDE NO OTHERS  */
#line 1842 "sqlparser_sql2003.y"
                                { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER,"EXCLUDE NO OTHERS"); }
#line 6801 "sqlparser_sql2003_bison.cpp"
    break;

  case 298: /* scalar_function: relation_factor_func '(' ')'  */
#line 1847 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_FUN_CALL, E_FUN_CALL_PROPERTY_CNT,
    		(yyvsp[-2].node), nullptr, nullptr, nullptr, nullptr);
    (yyval.node)->serialize_format = &FUN_CALL_1_OVER_SERIALIZE_FORMAT;
}
#line 6811 "sqlparser_sql2003_bison.cpp"
    break;

  case 299: /* scalar_function: relation_factor_func '(' row_expr_list ')'  */
#line 1853 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_FUN_CALL, E_FUN_CALL_PROPERTY_CNT,
    		(yyvsp[-3].node), (yyvsp[-1].node), nullptr, nullptr, nullptr);
    (yyval.node)->serialize_format = &FUN_CALL_1_OVER_SERIALIZE_FORMAT;
}
#line 6821 "sqlparser_sql2003_bison.cpp"
    break;

  case 300: /* function_call_keyword: non_second_primary_datetime_field '(' row_expr ')'  */
#line 1865 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_FUN_CALL, E_FUN_CALL_PROPERTY_CNT,
    		(yyvsp[-3].node), (yyvsp[-1].node), nullptr, nullptr, nullptr);
    (yyval.node)->serialize_format = &FUN_CALL_1_OVER_SERIALIZE_FORMAT;
}
#line 6831 "sqlparser_sql2003_bison.cpp"
    break;

  case 301: /* function_call_keyword: RIGHT '(' row_expr_list ')'  */
#line 1871 "sqlparser_sql2003.y"
{
    Node* fun_name = Node::makeTerminalNode(E_IDENTIFIER, "RIGHT");
    (yyval.node) = Node::makeNonTerminalNode(E_FUN_CALL, E_FUN_CALL_PROPERTY_CNT,
    		fun_name, (yyvsp[-1].node), nullptr, nullptr, nullptr);
    (yyval.node)->serialize_format = &FUN_CALL_1_OVER_SERIALIZE_FORMAT;
}
#line 6842 "sqlparser_sql2003_bison.cpp"
    break;

  case 302: /* function_call_keyword: LEFT '(' row_expr_list ')'  */
#line 1878 "sqlparser_sql2003.y"
{
    Node* fun_name = Node::makeTerminalNode(E_IDENTIFIER, "LEFT");
    (yyval.node) = Node::makeNonTerminalNode(E_FUN_CALL, E_FUN_CALL_PROPERTY_CNT,
    		fun_name, (yyvsp[-1].node), nullptr, nullptr, nullptr);
    (yyval.node)->serialize_format = &FUN_CALL_1_OVER_SERIALIZE_FORMAT;
}
#line 6853 "sqlparser_sql2003_bison.cpp"
    break;

  case 303: /* function_call_keyword: CAST '(' row_expr AS data_type ')'  */
#line 1885 "sqlparser_sql2003.y"
{
    Node* fun_name = Node::makeTerminalNode(E_IDENTIFIER, "CAST");
    (yyval.node) = Node::makeNonTerminalNode(E_FUN_CALL, E_FUN_CALL_PROPERTY_CNT,
    		fun_name, (yyvsp[-3].node), nullptr, nullptr, (yyvsp[-1].node));
    (yyval.node)->serialize_format = &FUN_CALL_AS_SERIALIZE_FORMAT;
}
#line 6864 "sqlparser_sql2003_bison.cpp"
    break;

  case 304: /* function_call_keyword: CONVERT '(' data_type ',' row_expr ',' INTNUM ')'  */
#line 1892 "sqlparser_sql2003.y"
{
    Node* fun_name = Node::makeTerminalNode(E_IDENTIFIER, "CONVERT");
    Node* sub_expr_list_node = Node::makeNonTerminalNode(E_EXPR_LIST, E_LIST_PROPERTY_CNT, (yyvsp[-5].node), (yyvsp[-3].node));
    sub_expr_list_node->serialize_format = &COMMA_LIST_SERIALIZE_FORMAT;
    Node* expr_list_node = Node::makeNonTerminalNode(E_EXPR_LIST, E_LIST_PROPERTY_CNT, sub_expr_list_node, (yyvsp[-1].node));
    expr_list_node->serialize_format = &COMMA_LIST_SERIALIZE_FORMAT;

    (yyval.node) = Node::makeNonTerminalNode(E_FUN_CALL, E_FUN_CALL_PROPERTY_CNT,	fun_name, expr_list_node, nullptr, nullptr, nullptr);
    (yyval.node)->serialize_format = &FUN_CALL_BIGQUERY_S_FORMAT;
}
#line 6879 "sqlparser_sql2003_bison.cpp"
    break;

  case 305: /* function_call_keyword: CONVERT '(' data_type ',' row_expr ')'  */
#line 1903 "sqlparser_sql2003.y"
{
    Node* fun_name = Node::makeTerminalNode(E_IDENTIFIER, "CONVERT");
    Node* expr_list_node = Node::makeNonTerminalNode(E_EXPR_LIST, E_LIST_PROPERTY_CNT, (yyvsp[-3].node), (yyvsp[-1].node));
    expr_list_node->serialize_format = &COMMA_LIST_SERIALIZE_FORMAT;

    (yyval.node) = Node::makeNonTerminalNode(E_FUN_CALL, E_FUN_CALL_PROPERTY_CNT,	fun_name, expr_list_node, nullptr, nullptr, nullptr);
    (yyval.node)->serialize_format = &FUN_CALL_BIGQUERY_S_FORMAT;
}
#line 6892 "sqlparser_sql2003_bison.cpp"
    break;

  case 306: /* function_call_keyword: COALESCE '(' row_expr_list ')'  */
#line 1912 "sqlparser_sql2003.y"
{
    Node* fun_name = Node::makeTerminalNode(E_IDENTIFIER, "COALESCE");
    (yyval.node) = Node::makeNonTerminalNode(E_FUN_CALL, E_FUN_CALL_PROPERTY_CNT,
    		fun_name, (yyvsp[-1].node), nullptr, nullptr, nullptr);
    (yyval.node)->serialize_format = &FUN_CALL_1_OVER_SERIALIZE_FORMAT;
}
#line 6903 "sqlparser_sql2003_bison.cpp"
    break;

  case 307: /* function_call_keyword: CURRENT_TIMESTAMP  */
#line 1919 "sqlparser_sql2003.y"
{
    Node* fun_name = Node::makeTerminalNode(E_IDENTIFIER, "CURRENT_TIMESTAMP");
    (yyval.node) = Node::makeNonTerminalNode(E_FUN_CALL, E_FUN_CALL_PROPERTY_CNT,
    		fun_name, nullptr, nullptr, nullptr, nullptr);
    (yyval.node)->serialize_format = &SINGLE_SERIALIZE_FORMAT;
}
#line 6914 "sqlparser_sql2003_bison.cpp"
    break;

  case 308: /* function_call_keyword: CURRENT_TIMESTAMP '(' INTNUM ')'  */
#line 1926 "sqlparser_sql2003.y"
{
    Node* fun_name = Node::makeTerminalNode(E_IDENTIFIER, "CURRENT_TIMESTAMP");
    (yyval.node) = Node::makeNonTerminalNode(E_FUN_CALL, E_FUN_CALL_PROPERTY_CNT,
    		fun_name, (yyvsp[-1].node), nullptr, nullptr, nullptr);
    (yyval.node)->serialize_format = &FUN_CALL_1_OVER_SERIALIZE_FORMAT;
}
#line 6925 "sqlparser_sql2003_bison.cpp"
    break;

  case 309: /* function_call_keyword: CURRENT_USER  */
#line 1933 "sqlparser_sql2003.y"
{
    Node* fun_name = Node::makeTerminalNode(E_IDENTIFIER, "CURRENT_USER");
    (yyval.node) = Node::makeNonTerminalNode(E_FUN_CALL, E_FUN_CALL_PROPERTY_CNT,
    		fun_name, nullptr, nullptr, nullptr, nullptr);
    (yyval.node)->serialize_format = &SINGLE_SERIALIZE_FORMAT;
}
#line 6936 "sqlparser_sql2003_bison.cpp"
    break;

  case 310: /* function_call_keyword: NULLIF '(' row_expr ',' row_expr ')'  */
#line 1940 "sqlparser_sql2003.y"
{
    Node* expr_list = Node::makeNonTerminalNode(E_EXPR_LIST, E_LIST_PROPERTY_CNT, (yyvsp[-3].node), (yyvsp[-1].node));
    expr_list->serialize_format = &COMMA_LIST_SERIALIZE_FORMAT;
    Node* fun_name = Node::makeTerminalNode(E_IDENTIFIER, "NULLIF");
    (yyval.node) = Node::makeNonTerminalNode(E_FUN_CALL, E_FUN_CALL_PROPERTY_CNT,
    		fun_name, expr_list, nullptr, nullptr, nullptr);
    (yyval.node)->serialize_format = &FUN_CALL_1_OVER_SERIALIZE_FORMAT;
}
#line 6949 "sqlparser_sql2003_bison.cpp"
    break;

  case 311: /* function_call_keyword: SESSION_USER  */
#line 1949 "sqlparser_sql2003.y"
{
    Node* fun_name = Node::makeTerminalNode(E_IDENTIFIER, "SESSION_USER");
    (yyval.node) = Node::makeNonTerminalNode(E_FUN_CALL, E_FUN_CALL_PROPERTY_CNT,
    		fun_name, nullptr, nullptr, nullptr, nullptr);
    (yyval.node)->serialize_format = &SINGLE_SERIALIZE_FORMAT;
}
#line 6960 "sqlparser_sql2003_bison.cpp"
    break;

  case 312: /* function_call_keyword: SYSTEM_USER  */
#line 1956 "sqlparser_sql2003.y"
{
    Node* fun_name = Node::makeTerminalNode(E_IDENTIFIER, "SYSTEM_USER");
    (yyval.node) = Node::makeNonTerminalNode(E_FUN_CALL, E_FUN_CALL_PROPERTY_CNT,
    		fun_name, nullptr, nullptr, nullptr, nullptr);
    (yyval.node)->serialize_format = &SINGLE_SERIALIZE_FORMAT;
}
#line 6971 "sqlparser_sql2003_bison.cpp"
    break;

  case 313: /* function_call_keyword: TRIM '(' row_expr ')'  */
#line 1963 "sqlparser_sql2003.y"
{
    Node* fun_name = Node::makeTerminalNode(E_IDENTIFIER, "TRIM");

    (yyval.node) = Node::makeNonTerminalNode(E_FUN_CALL, E_FUN_CALL_PROPERTY_CNT,
    		fun_name, (yyvsp[-1].node), nullptr, nullptr, nullptr);
    (yyval.node)->serialize_format = &FUN_CALL_1_OVER_SERIALIZE_FORMAT;
}
#line 6983 "sqlparser_sql2003_bison.cpp"
    break;

  case 314: /* function_call_keyword: TRIM '(' row_expr FROM row_expr ')'  */
#line 1971 "sqlparser_sql2003.y"
{
    Node* fun_name = Node::makeTerminalNode(E_IDENTIFIER, "TRIM");

    (yyval.node) = Node::makeNonTerminalNode(E_FUN_CALL, E_FUN_CALL_PROPERTY_CNT,
    		fun_name, (yyvsp[-1].node), (yyvsp[-3].node), nullptr, nullptr);
    (yyval.node)->serialize_format = &FUNC_TRIM_SERIALIZE_FORMAT;
}
#line 6995 "sqlparser_sql2003_bison.cpp"
    break;

  case 319: /* user_defined_type_name: relation_factor_type  */
#line 1992 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeTerminalNode(E_STRING, (yyvsp[0].node)->text());
    delete((yyvsp[0].node));
}
#line 7004 "sqlparser_sql2003_bison.cpp"
    break;

  case 320: /* relation_factor_type: name_type  */
#line 1999 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_TABLE_IDENT, E_TABLE_IDENT_PROPERTY_CNT, (yyvsp[0].node), nullptr, nullptr, nullptr);
    (yyval.node)->serialize_format = &SINGLE_SERIALIZE_FORMAT;
}
#line 7013 "sqlparser_sql2003_bison.cpp"
    break;

  case 321: /* relation_factor_type: name_type '.' name_type  */
#line 2004 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_TABLE_IDENT, E_TABLE_IDENT_PROPERTY_CNT, (yyvsp[0].node), (yyvsp[-2].node), nullptr, nullptr);
    (yyval.node)->serialize_format = &TABLE_IDENT_SERIALIZE_FORMAT_1;
}
#line 7022 "sqlparser_sql2003_bison.cpp"
    break;

  case 322: /* relation_factor_type: name_type '.' name_type '.' name_type  */
#line 2009 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_TABLE_IDENT, E_TABLE_IDENT_PROPERTY_CNT, (yyvsp[0].node), (yyvsp[-2].node), (yyvsp[-4].node), nullptr);
    (yyval.node)->serialize_format = &TABLE_IDENT_SERIALIZE_FORMAT_2;
}
#line 7031 "sqlparser_sql2003_bison.cpp"
    break;

  case 323: /* relation_factor_type: name_type '.' name_type '.' name_type '.' name_type  */
#line 2014 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_TABLE_IDENT, E_TABLE_IDENT_PROPERTY_CNT, (yyvsp[0].node), (yyvsp[-2].node), (yyvsp[-4].node), (yyvsp[-6].node));
    (yyval.node)->serialize_format = &TABLE_IDENT_SERIALIZE_FORMAT_3;
}
#line 7040 "sqlparser_sql2003_bison.cpp"
    break;

  case 324: /* relation_factor_func: name_f  */
#line 2022 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_TABLE_IDENT, E_TABLE_IDENT_PROPERTY_CNT, (yyvsp[0].node), nullptr, nullptr, nullptr);
    (yyval.node)->serialize_format = &SINGLE_SERIALIZE_FORMAT;
}
#line 7049 "sqlparser_sql2003_bison.cpp"
    break;

  case 325: /* relation_factor_func: name_r '.' name_f  */
#line 2027 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_TABLE_IDENT, E_TABLE_IDENT_PROPERTY_CNT, (yyvsp[0].node), (yyvsp[-2].node), nullptr, nullptr);
    (yyval.node)->serialize_format = &TABLE_IDENT_SERIALIZE_FORMAT_1;
}
#line 7058 "sqlparser_sql2003_bison.cpp"
    break;

  case 326: /* relation_factor_func: name_r '.' name_r '.' name_f  */
#line 2032 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_TABLE_IDENT, E_TABLE_IDENT_PROPERTY_CNT, (yyvsp[0].node), (yyvsp[-2].node), (yyvsp[-4].node), nullptr);
    (yyval.node)->serialize_format = &TABLE_IDENT_SERIALIZE_FORMAT_2;
}
#line 7067 "sqlparser_sql2003_bison.cpp"
    break;

  case 327: /* relation_factor_func: name_r '.' name_r '.' name_r '.' name_f  */
#line 2037 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_TABLE_IDENT, E_TABLE_IDENT_PROPERTY_CNT, (yyvsp[0].node), (yyvsp[-2].node), (yyvsp[-4].node), (yyvsp[-6].node));
    (yyval.node)->serialize_format = &TABLE_IDENT_SERIALIZE_FORMAT_3;
}
#line 7076 "sqlparser_sql2003_bison.cpp"
    break;

  case 328: /* reference_type: REF '(' relation_factor ')' SCOPE relation_factor  */
#line 2045 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeTerminalNode(E_STRING, "REF("+(yyvsp[-3].node)->text()+")SCOPE "+(yyvsp[0].node)->text());
    delete((yyvsp[-3].node));delete((yyvsp[0].node));
}
#line 7085 "sqlparser_sql2003_bison.cpp"
    break;

  case 329: /* collection_type: data_type ARRAY '(' INTNUM ')'  */
#line 2053 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeTerminalNode(E_STRING, (yyvsp[-4].node)->text()+" ARRAY("+(yyvsp[-1].node)->text()+")");
    delete((yyvsp[-4].node));
    delete((yyvsp[-1].node));
}
#line 7095 "sqlparser_sql2003_bison.cpp"
    break;

  case 330: /* collection_type: data_type ARRAY  */
#line 2059 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeTerminalNode(E_STRING, (yyvsp[-1].node)->text()+" ARRAY");
    delete((yyvsp[-1].node));
}
#line 7104 "sqlparser_sql2003_bison.cpp"
    break;

  case 331: /* collection_type: data_type MULTISET  */
#line 2064 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeTerminalNode(E_STRING, (yyvsp[-1].node)->text()+" MULTISET");
    delete((yyvsp[-1].node));
}
#line 7113 "sqlparser_sql2003_bison.cpp"
    break;

  case 332: /* predefined_type: character_string_type CHARACTER SET relation_factor collate_clause  */
#line 2072 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeTerminalNode(E_STRING, (yyvsp[-4].node)->text()+" CHARACTER SET "+(yyvsp[-1].node)->text()+(yyvsp[0].node)->text());
    delete((yyvsp[-4].node));delete((yyvsp[-1].node));delete((yyvsp[0].node));
}
#line 7122 "sqlparser_sql2003_bison.cpp"
    break;

  case 333: /* predefined_type: character_string_type CHARACTER SET relation_factor  */
#line 2077 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeTerminalNode(E_STRING, (yyvsp[-3].node)->text()+" CHARACTER SET "+(yyvsp[0].node)->text());
    delete((yyvsp[-3].node));delete((yyvsp[0].node));
}
#line 7131 "sqlparser_sql2003_bison.cpp"
    break;

  case 334: /* predefined_type: character_string_type collate_clause  */
#line 2082 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeTerminalNode(E_STRING, (yyvsp[-1].node)->text()+(yyvsp[0].node)->text());
    delete((yyvsp[-1].node));delete((yyvsp[0].node));
}
#line 7140 "sqlparser_sql2003_bison.cpp"
    break;

  case 336: /* predefined_type: national_character_string_type collate_clause  */
#line 2088 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeTerminalNode(E_STRING, (yyvsp[-1].node)->text()+(yyvsp[0].node)->text());
    delete((yyvsp[-1].node));delete((yyvsp[0].node));
}
#line 7149 "sqlparser_sql2003_bison.cpp"
    break;

  case 343: /* interval_type: INTERVAL interval_qualifier  */
#line 2102 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeTerminalNode(E_STRING, "INTERVAL "+(yyvsp[0].node)->text());
    delete((yyvsp[0].node));
}
#line 7158 "sqlparser_sql2003_bison.cpp"
    break;

  case 344: /* interval_qualifier: start_field TO end_field  */
#line 2110 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeTerminalNode(E_STRING, (yyvsp[-2].node)->text()+" TO "+(yyvsp[0].node)->text());
    delete((yyvsp[-2].node));delete((yyvsp[0].node));
}
#line 7167 "sqlparser_sql2003_bison.cpp"
    break;

  case 346: /* start_field: non_second_primary_datetime_field '(' INTNUM ')'  */
#line 2119 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeTerminalNode(E_STRING, (yyvsp[-3].node)->text()+"("+(yyvsp[-1].node)->text()+")");
    delete((yyvsp[-3].node));
    delete((yyvsp[-1].node));
}
#line 7177 "sqlparser_sql2003_bison.cpp"
    break;

  case 349: /* end_field: SECOND '(' INTNUM ')'  */
#line 2130 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeTerminalNode(E_STRING, "SECOND("+(yyvsp[-1].node)->text()+")");
    delete((yyvsp[-1].node));
}
#line 7186 "sqlparser_sql2003_bison.cpp"
    break;

  case 350: /* end_field: SECOND  */
#line 2135 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeTerminalNode(E_STRING, "SECOND");
}
#line 7194 "sqlparser_sql2003_bison.cpp"
    break;

  case 351: /* single_datetime_field: non_second_primary_datetime_field '(' INTNUM ')'  */
#line 2142 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeTerminalNode(E_STRING, (yyvsp[-3].node)->text()+"("+(yyvsp[-1].node)->text()+")");
    delete((yyvsp[-3].node));delete((yyvsp[-1].node));
}
#line 7203 "sqlparser_sql2003_bison.cpp"
    break;

  case 353: /* single_datetime_field: SECOND '(' INTNUM ',' INTNUM ')'  */
#line 2148 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeTerminalNode(E_STRING, "SECOND("+(yyvsp[-3].node)->text()+","+(yyvsp[-1].node)->text()+")");
    delete((yyvsp[-3].node));delete((yyvsp[-1].node));
}
#line 7212 "sqlparser_sql2003_bison.cpp"
    break;

  case 354: /* single_datetime_field: SECOND '(' INTNUM ')'  */
#line 2153 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeTerminalNode(E_STRING, "SECOND("+(yyvsp[-1].node)->text()+")");
    delete((yyvsp[-1].node));
}
#line 7221 "sqlparser_sql2003_bison.cpp"
    break;

  case 355: /* single_datetime_field: SECOND  */
#line 2158 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeTerminalNode(E_STRING, "SECOND");
}
#line 7229 "sqlparser_sql2003_bison.cpp"
    break;

  case 356: /* non_second_primary_datetime_field: YEAR  */
#line 2165 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeTerminalNode(E_STRING, "YEAR");
}
#line 7237 "sqlparser_sql2003_bison.cpp"
    break;

  case 357: /* non_second_primary_datetime_field: MONTH  */
#line 2169 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeTerminalNode(E_STRING, "MONTH");
}
#line 7245 "sqlparser_sql2003_bison.cpp"
    break;

  case 358: /* non_second_primary_datetime_field: DAY  */
#line 2173 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeTerminalNode(E_STRING, "DAY");
}
#line 7253 "sqlparser_sql2003_bison.cpp"
    break;

  case 359: /* non_second_primary_datetime_field: HOUR  */
#line 2177 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeTerminalNode(E_STRING, "HOUR");
}
#line 7261 "sqlparser_sql2003_bison.cpp"
    break;

  case 360: /* non_second_primary_datetime_field: MINUTE  */
#line 2181 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeTerminalNode(E_STRING, "MINUTE");
}
#line 7269 "sqlparser_sql2003_bison.cpp"
    break;

  case 361: /* boolean_type: BOOLEAN  */
#line 2188 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeTerminalNode(E_STRING, "BOOLEAN");
}
#line 7277 "sqlparser_sql2003_bison.cpp"
    break;

  case 362: /* datetime_type: DATE  */
#line 2195 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeTerminalNode(E_STRING, "DATE");
}
#line 7285 "sqlparser_sql2003_bison.cpp"
    break;

  case 363: /* datetime_type: TIME '(' INTNUM ')' WITH TIME ZONE  */
#line 2199 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeTerminalNode(E_STRING, "TIME("+(yyvsp[-4].node)->text()+") WITH TIME ZONE");
    delete((yyvsp[-4].node));
}
#line 7294 "sqlparser_sql2003_bison.cpp"
    break;

  case 364: /* datetime_type: TIME '(' INTNUM ')' WITHOUT TIME ZONE  */
#line 2204 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeTerminalNode(E_STRING, "TIME("+(yyvsp[-4].node)->text()+") WITHOUT TIME ZONE");
    delete((yyvsp[-4].node));
}
#line 7303 "sqlparser_sql2003_bison.cpp"
    break;

  case 365: /* datetime_type: TIME '(' INTNUM ')'  */
#line 2209 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeTerminalNode(E_STRING, "TIME("+(yyvsp[-1].node)->text()+")");
    delete((yyvsp[-1].node));
}
#line 7312 "sqlparser_sql2003_bison.cpp"
    break;

  case 366: /* datetime_type: TIME WITH TIME ZONE  */
#line 2214 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeTerminalNode(E_STRING, "TIME WITH TIME ZONE");
}
#line 7320 "sqlparser_sql2003_bison.cpp"
    break;

  case 367: /* datetime_type: TIME WITHOUT TIME ZONE  */
#line 2218 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeTerminalNode(E_STRING, "TIME WITHOUT TIME ZONE");
}
#line 7328 "sqlparser_sql2003_bison.cpp"
    break;

  case 368: /* datetime_type: TIMESTAMP '(' INTNUM ')' WITH TIME ZONE  */
#line 2222 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeTerminalNode(E_STRING, "TIMESTAMP("+(yyvsp[-4].node)->text()+") WITH TIME ZONE");
    delete((yyvsp[-4].node));
}
#line 7337 "sqlparser_sql2003_bison.cpp"
    break;

  case 369: /* datetime_type: TIMESTAMP '(' INTNUM ')' WITHOUT TIME ZONE  */
#line 2227 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeTerminalNode(E_STRING, "TIMESTAMP("+(yyvsp[-4].node)->text()+") WITHOUT TIME ZONE");
    delete((yyvsp[-4].node));
}
#line 7346 "sqlparser_sql2003_bison.cpp"
    break;

  case 370: /* datetime_type: TIMESTAMP '(' INTNUM ')'  */
#line 2232 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeTerminalNode(E_STRING, "TIMESTAMP("+(yyvsp[-1].node)->text()+")");
    delete((yyvsp[-1].node));
}
#line 7355 "sqlparser_sql2003_bison.cpp"
    break;

  case 371: /* datetime_type: TIMESTAMP WITH TIME ZONE  */
#line 2237 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeTerminalNode(E_STRING, "TIMESTAMP WITH TIME ZONE");
}
#line 7363 "sqlparser_sql2003_bison.cpp"
    break;

  case 372: /* datetime_type: TIMESTAMP WITHOUT TIME ZONE  */
#line 2241 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeTerminalNode(E_STRING, "TIMESTAMP WITHOUT TIME ZONE");
}
#line 7371 "sqlparser_sql2003_bison.cpp"
    break;

  case 375: /* exact_numeric_type: NUMERIC '(' INTNUM ',' INTNUM ')'  */
#line 2253 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeTerminalNode(E_STRING, "NUMERIC("+(yyvsp[-3].node)->text()+","+(yyvsp[-1].node)->text()+")");
    delete((yyvsp[-3].node));delete((yyvsp[-1].node));
}
#line 7380 "sqlparser_sql2003_bison.cpp"
    break;

  case 376: /* exact_numeric_type: NUMERIC '(' INTNUM ')'  */
#line 2258 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeTerminalNode(E_STRING, "NUMERIC("+(yyvsp[-1].node)->text()+")");
    delete((yyvsp[-1].node));
}
#line 7389 "sqlparser_sql2003_bison.cpp"
    break;

  case 377: /* exact_numeric_type: DECIMAL '(' INTNUM ',' INTNUM ')'  */
#line 2263 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeTerminalNode(E_STRING, "DECIMAL("+(yyvsp[-3].node)->text()+","+(yyvsp[-1].node)->text()+")");
    delete((yyvsp[-3].node));delete((yyvsp[-1].node));
}
#line 7398 "sqlparser_sql2003_bison.cpp"
    break;

  case 378: /* exact_numeric_type: DECIMAL '(' INTNUM ')'  */
#line 2268 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeTerminalNode(E_STRING, "DECIMAL("+(yyvsp[-1].node)->text()+")");
    delete((yyvsp[-1].node));
}
#line 7407 "sqlparser_sql2003_bison.cpp"
    break;

  case 379: /* exact_numeric_type: DEC '(' INTNUM ',' INTNUM ')'  */
#line 2273 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeTerminalNode(E_STRING, "DEC("+(yyvsp[-3].node)->text()+","+(yyvsp[-1].node)->text()+")");
    delete((yyvsp[-3].node));delete((yyvsp[-1].node));
}
#line 7416 "sqlparser_sql2003_bison.cpp"
    break;

  case 380: /* exact_numeric_type: DEC '(' INTNUM ')'  */
#line 2278 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeTerminalNode(E_STRING, "DEC("+(yyvsp[-1].node)->text()+")");
    delete((yyvsp[-1].node));
}
#line 7425 "sqlparser_sql2003_bison.cpp"
    break;

  case 381: /* exact_numeric_type: DEC  */
#line 2283 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeTerminalNode(E_STRING, "DEC");
}
#line 7433 "sqlparser_sql2003_bison.cpp"
    break;

  case 382: /* exact_numeric_type: SMALLINT  */
#line 2287 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeTerminalNode(E_STRING, "SMALLINT");
}
#line 7441 "sqlparser_sql2003_bison.cpp"
    break;

  case 383: /* exact_numeric_type: INTEGER  */
#line 2291 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeTerminalNode(E_STRING, "INTEGER");
}
#line 7449 "sqlparser_sql2003_bison.cpp"
    break;

  case 384: /* exact_numeric_type: INT  */
#line 2295 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeTerminalNode(E_STRING, "INT");
}
#line 7457 "sqlparser_sql2003_bison.cpp"
    break;

  case 385: /* exact_numeric_type: BIGINT  */
#line 2299 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeTerminalNode(E_STRING, "BIGINT");
}
#line 7465 "sqlparser_sql2003_bison.cpp"
    break;

  case 386: /* exact_numeric_type: NUMERIC  */
#line 2303 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeTerminalNode(E_STRING, "NUMERIC");
}
#line 7473 "sqlparser_sql2003_bison.cpp"
    break;

  case 387: /* exact_numeric_type: DECIMAL  */
#line 2307 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeTerminalNode(E_STRING, "DECIMAL");
}
#line 7481 "sqlparser_sql2003_bison.cpp"
    break;

  case 388: /* approximate_numeric_type: FLOAT '(' INTNUM ')'  */
#line 2314 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeTerminalNode(E_STRING, "FLOAT("+(yyvsp[-1].node)->text()+")");
    delete((yyvsp[-1].node));
}
#line 7490 "sqlparser_sql2003_bison.cpp"
    break;

  case 389: /* approximate_numeric_type: DOUBLE PRECISION  */
#line 2319 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeTerminalNode(E_STRING, "DOUBLE PRECISION");
}
#line 7498 "sqlparser_sql2003_bison.cpp"
    break;

  case 390: /* approximate_numeric_type: FLOAT  */
#line 2323 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeTerminalNode(E_STRING, "FLOAT");
}
#line 7506 "sqlparser_sql2003_bison.cpp"
    break;

  case 391: /* approximate_numeric_type: REAL  */
#line 2327 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeTerminalNode(E_STRING, "REAL");
}
#line 7514 "sqlparser_sql2003_bison.cpp"
    break;

  case 392: /* character_string_type: CHARACTER '(' data_type_len ')'  */
#line 2334 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeTerminalNode(E_STRING, "CHARACTER("+(yyvsp[-1].node)->text()+")");
    delete((yyvsp[-1].node));
}
#line 7523 "sqlparser_sql2003_bison.cpp"
    break;

  case 393: /* character_string_type: CHAR '(' data_type_len ')'  */
#line 2339 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeTerminalNode(E_STRING, "CHAR("+(yyvsp[-1].node)->text()+")");
    delete((yyvsp[-1].node));
}
#line 7532 "sqlparser_sql2003_bison.cpp"
    break;

  case 394: /* character_string_type: CHARACTER VARYING '(' data_type_len ')'  */
#line 2344 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeTerminalNode(E_STRING, "CHARACTER VARYING("+(yyvsp[-1].node)->text()+")");
    delete((yyvsp[-1].node));
}
#line 7541 "sqlparser_sql2003_bison.cpp"
    break;

  case 395: /* character_string_type: CHAR VARYING '(' data_type_len ')'  */
#line 2349 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeTerminalNode(E_STRING, "CHAR VARYING("+(yyvsp[-1].node)->text()+")");
    delete((yyvsp[-1].node));
}
#line 7550 "sqlparser_sql2003_bison.cpp"
    break;

  case 396: /* character_string_type: VARCHAR '(' data_type_len ')'  */
#line 2354 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeTerminalNode(E_STRING, "VARCHAR("+(yyvsp[-1].node)->text()+")");
    delete((yyvsp[-1].node));
}
#line 7559 "sqlparser_sql2003_bison.cpp"
    break;

  case 397: /* character_string_type: VARCHAR  */
#line 2359 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeTerminalNode(E_STRING, "VARCHAR");
}
#line 7567 "sqlparser_sql2003_bison.cpp"
    break;

  case 398: /* character_string_type: NVARCHAR  */
#line 2363 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeTerminalNode(E_STRING, "NVARCHAR");
}
#line 7575 "sqlparser_sql2003_bison.cpp"
    break;

  case 399: /* character_string_type: NVARCHAR '(' data_type_len ')'  */
#line 2367 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeTerminalNode(E_STRING, "NVARCHAR("+(yyvsp[-1].node)->text()+")");
    delete((yyvsp[-1].node));
}
#line 7584 "sqlparser_sql2003_bison.cpp"
    break;

  case 400: /* character_string_type: CHARACTER LARGE OBJECT '(' large_object_length ')'  */
#line 2372 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeTerminalNode(E_STRING, "CHARACTER LARGE OBJECT("+(yyvsp[-1].node)->text()+")");
    delete((yyvsp[-1].node));
}
#line 7593 "sqlparser_sql2003_bison.cpp"
    break;

  case 401: /* character_string_type: CHARACTER LARGE OBJECT  */
#line 2377 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeTerminalNode(E_STRING, "CHARACTER LARGE OBJECT");
}
#line 7601 "sqlparser_sql2003_bison.cpp"
    break;

  case 402: /* character_string_type: CHAR LARGE OBJECT '(' large_object_length ')'  */
#line 2381 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeTerminalNode(E_STRING, "CHAR LARGE OBJECT("+(yyvsp[-1].node)->text()+")");
    delete((yyvsp[-1].node));
}
#line 7610 "sqlparser_sql2003_bison.cpp"
    break;

  case 403: /* character_string_type: CHAR LARGE OBJECT  */
#line 2386 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeTerminalNode(E_STRING, "CHAR LARGE OBJECT");
}
#line 7618 "sqlparser_sql2003_bison.cpp"
    break;

  case 404: /* character_string_type: CLOB '(' large_object_length ')'  */
#line 2390 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeTerminalNode(E_STRING, "CLOB("+(yyvsp[-1].node)->text()+")");
    delete((yyvsp[-1].node));
}
#line 7627 "sqlparser_sql2003_bison.cpp"
    break;

  case 405: /* character_string_type: CLOB  */
#line 2395 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeTerminalNode(E_STRING, "CLOB");
}
#line 7635 "sqlparser_sql2003_bison.cpp"
    break;

  case 406: /* character_string_type: CHAR  */
#line 2399 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeTerminalNode(E_STRING, "CHAR");
}
#line 7643 "sqlparser_sql2003_bison.cpp"
    break;

  case 407: /* character_string_type: CHARACTER  */
#line 2403 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeTerminalNode(E_STRING, "CHARACTER");
}
#line 7651 "sqlparser_sql2003_bison.cpp"
    break;

  case 409: /* data_type_len: MAX  */
#line 2410 "sqlparser_sql2003.y"
       { (yyval.node) = Node::makeTerminalNode(E_STRING, "MAX"); }
#line 7657 "sqlparser_sql2003_bison.cpp"
    break;

  case 410: /* binary_large_object_string_type: BINARY LARGE OBJECT '(' INTNUM ')'  */
#line 2415 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeTerminalNode(E_STRING, "BINARY LARGE OBJECT("+(yyvsp[-1].node)->text()+")");
    delete((yyvsp[-1].node));
}
#line 7666 "sqlparser_sql2003_bison.cpp"
    break;

  case 411: /* binary_large_object_string_type: BINARY LARGE OBJECT  */
#line 2420 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeTerminalNode(E_STRING, "BINARY LARGE OBJECT");
}
#line 7674 "sqlparser_sql2003_bison.cpp"
    break;

  case 412: /* binary_large_object_string_type: BLOB '(' INTNUM ')'  */
#line 2424 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeTerminalNode(E_STRING, "BLOB("+(yyvsp[-1].node)->text()+")");
    delete((yyvsp[-1].node));
}
#line 7683 "sqlparser_sql2003_bison.cpp"
    break;

  case 413: /* binary_large_object_string_type: BLOB  */
#line 2429 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeTerminalNode(E_STRING, "BLOB");
}
#line 7691 "sqlparser_sql2003_bison.cpp"
    break;

  case 414: /* national_character_string_type: NATIONAL CHARACTER '(' INTNUM ')'  */
#line 2436 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeTerminalNode(E_STRING, "NATIONAL CHARACTER("+(yyvsp[-1].node)->text()+")");
    delete((yyvsp[-1].node));
}
#line 7700 "sqlparser_sql2003_bison.cpp"
    break;

  case 415: /* national_character_string_type: NATIONAL CHARACTER  */
#line 2441 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeTerminalNode(E_STRING, "NATIONAL CHARACTER");
}
#line 7708 "sqlparser_sql2003_bison.cpp"
    break;

  case 416: /* national_character_string_type: NATIONAL CHAR '(' INTNUM ')'  */
#line 2445 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeTerminalNode(E_STRING, "NATIONAL CHAR("+(yyvsp[-1].node)->text()+")");
    delete((yyvsp[-1].node));
}
#line 7717 "sqlparser_sql2003_bison.cpp"
    break;

  case 417: /* national_character_string_type: NATIONAL CHAR  */
#line 2450 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeTerminalNode(E_STRING, "NATIONAL CHAR");
}
#line 7725 "sqlparser_sql2003_bison.cpp"
    break;

  case 418: /* national_character_string_type: NCHAR '(' data_type_len ')'  */
#line 2454 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeTerminalNode(E_STRING, "NCHAR("+(yyvsp[-1].node)->text()+")");
    delete((yyvsp[-1].node));
}
#line 7734 "sqlparser_sql2003_bison.cpp"
    break;

  case 419: /* national_character_string_type: NATIONAL CHARACTER VARYING '(' INTNUM ')'  */
#line 2459 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeTerminalNode(E_STRING, "NATIONAL CHARACTER VARYING("+(yyvsp[-1].node)->text()+")");
    delete((yyvsp[-1].node));
}
#line 7743 "sqlparser_sql2003_bison.cpp"
    break;

  case 420: /* national_character_string_type: NATIONAL CHAR VARYING '(' INTNUM ')'  */
#line 2464 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeTerminalNode(E_STRING, "NATIONAL CHAR VARYING("+(yyvsp[-1].node)->text()+")");
    delete((yyvsp[-1].node));
}
#line 7752 "sqlparser_sql2003_bison.cpp"
    break;

  case 421: /* national_character_string_type: NCHAR VARYING '(' data_type_len ')'  */
#line 2469 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeTerminalNode(E_STRING, "NCHAR VARYING("+(yyvsp[-1].node)->text()+")");
    delete((yyvsp[-1].node));
}
#line 7761 "sqlparser_sql2003_bison.cpp"
    break;

  case 422: /* national_character_string_type: NATIONAL CHARACTER LARGE OBJECT '(' large_object_length ')'  */
#line 2474 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeTerminalNode(E_STRING, "NATIONAL CHARACTER LARGE OBJECT("+(yyvsp[-1].node)->text()+")");
    delete((yyvsp[-1].node));
}
#line 7770 "sqlparser_sql2003_bison.cpp"
    break;

  case 423: /* national_character_string_type: NATIONAL CHARACTER LARGE OBJECT  */
#line 2479 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeTerminalNode(E_STRING, "NATIONAL CHARACTER LARGE OBJECT");
}
#line 7778 "sqlparser_sql2003_bison.cpp"
    break;

  case 424: /* national_character_string_type: NCHAR LARGE OBJECT '(' large_object_length ')'  */
#line 2483 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeTerminalNode(E_STRING, "NCHAR LARGE OBJECT("+(yyvsp[-1].node)->text()+")");
    delete((yyvsp[-1].node));
}
#line 7787 "sqlparser_sql2003_bison.cpp"
    break;

  case 425: /* national_character_string_type: NCHAR LARGE OBJECT  */
#line 2488 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeTerminalNode(E_STRING, "NCHAR LARGE OBJECT");
}
#line 7795 "sqlparser_sql2003_bison.cpp"
    break;

  case 426: /* national_character_string_type: NCLOB '(' large_object_length ')'  */
#line 2492 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeTerminalNode(E_STRING, "NCLOB("+(yyvsp[-1].node)->text()+")");
    delete((yyvsp[-1].node));
}
#line 7804 "sqlparser_sql2003_bison.cpp"
    break;

  case 427: /* national_character_string_type: NCLOB  */
#line 2497 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeTerminalNode(E_STRING, "NCLOB");
}
#line 7812 "sqlparser_sql2003_bison.cpp"
    break;

  case 428: /* national_character_string_type: NCHAR  */
#line 2501 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeTerminalNode(E_STRING, "NCHAR");
}
#line 7820 "sqlparser_sql2003_bison.cpp"
    break;

  case 429: /* large_object_length: INTNUM multiplier char_length_units  */
#line 2508 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeTerminalNode(E_STRING, (yyvsp[-2].node)->text()+" "+(yyvsp[-1].node)->text()+" "+(yyvsp[0].node)->text());
    delete((yyvsp[-2].node));delete((yyvsp[-1].node));delete((yyvsp[0].node));
}
#line 7829 "sqlparser_sql2003_bison.cpp"
    break;

  case 430: /* large_object_length: INTNUM multiplier  */
#line 2513 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeTerminalNode(E_STRING, (yyvsp[-1].node)->text()+" "+(yyvsp[0].node)->text());
    delete((yyvsp[-1].node));delete((yyvsp[0].node));
}
#line 7838 "sqlparser_sql2003_bison.cpp"
    break;

  case 431: /* large_object_length: INTNUM char_length_units  */
#line 2518 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeTerminalNode(E_STRING, (yyvsp[-1].node)->text()+" "+(yyvsp[0].node)->text());
    delete((yyvsp[-1].node));delete((yyvsp[0].node));
}
#line 7847 "sqlparser_sql2003_bison.cpp"
    break;

  case 432: /* large_object_length: INTNUM  */
#line 2523 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeTerminalNode(E_STRING, (yyvsp[0].node)->text());
    delete((yyvsp[0].node));
}
#line 7856 "sqlparser_sql2003_bison.cpp"
    break;

  case 433: /* char_length_units: CHARACTERS  */
#line 2531 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeTerminalNode(E_STRING, "CHARACTERS");
}
#line 7864 "sqlparser_sql2003_bison.cpp"
    break;

  case 434: /* char_length_units: CODE_UNITS  */
#line 2535 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeTerminalNode(E_STRING, "CODE_UNITS");
}
#line 7872 "sqlparser_sql2003_bison.cpp"
    break;

  case 435: /* char_length_units: OCTETS  */
#line 2539 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeTerminalNode(E_STRING, "OCTETS");
}
#line 7880 "sqlparser_sql2003_bison.cpp"
    break;

  case 436: /* multiplier: K  */
#line 2546 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeTerminalNode(E_STRING, "K");
}
#line 7888 "sqlparser_sql2003_bison.cpp"
    break;

  case 437: /* multiplier: M  */
#line 2550 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeTerminalNode(E_STRING, "M");
}
#line 7896 "sqlparser_sql2003_bison.cpp"
    break;

  case 438: /* multiplier: G  */
#line 2554 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeTerminalNode(E_STRING, "G");
}
#line 7904 "sqlparser_sql2003_bison.cpp"
    break;

  case 439: /* distinct_or_all: ALL  */
#line 2561 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeTerminalNode(E_ALL, "ALL");
}
#line 7912 "sqlparser_sql2003_bison.cpp"
    break;

  case 440: /* distinct_or_all: DISTINCT  */
#line 2565 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeTerminalNode(E_DISTINCT, "DISTINCT");
}
#line 7920 "sqlparser_sql2003_bison.cpp"
    break;

  case 441: /* all_some_any: ALL  */
#line 2571 "sqlparser_sql2003.y"
                { (yyval.ival) = 0; }
#line 7926 "sqlparser_sql2003_bison.cpp"
    break;

  case 442: /* all_some_any: SOME  */
#line 2572 "sqlparser_sql2003.y"
                { (yyval.ival) = 1; }
#line 7932 "sqlparser_sql2003_bison.cpp"
    break;

  case 443: /* all_some_any: ANY  */
#line 2573 "sqlparser_sql2003.y"
                { (yyval.ival) = 2; }
#line 7938 "sqlparser_sql2003_bison.cpp"
    break;

  case 444: /* opt_as_label: %empty  */
#line 2577 "sqlparser_sql2003.y"
                { (yyval.node) = nullptr; }
#line 7944 "sqlparser_sql2003_bison.cpp"
    break;

  case 446: /* as_label: AS label  */
#line 2582 "sqlparser_sql2003.y"
                { (yyval.node) = (yyvsp[0].node); }
#line 7950 "sqlparser_sql2003_bison.cpp"
    break;

  case 450: /* collate_clause: COLLATE relation_factor  */
#line 2594 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeTerminalNode(E_STRING, "COLLATE "+(yyvsp[0].node)->text());
    delete((yyvsp[0].node));
}
#line 7959 "sqlparser_sql2003_bison.cpp"
    break;

  case 460: /* reserved_type: INT  */
#line 2618 "sqlparser_sql2003.y"
                 { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "INT"); }
#line 7965 "sqlparser_sql2003_bison.cpp"
    break;

  case 461: /* reserved_type: ARRAY  */
#line 2619 "sqlparser_sql2003.y"
                        { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "ARRAY"); }
#line 7971 "sqlparser_sql2003_bison.cpp"
    break;

  case 462: /* reserved_type: BINARY  */
#line 2620 "sqlparser_sql2003.y"
                        { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "BINARY"); }
#line 7977 "sqlparser_sql2003_bison.cpp"
    break;

  case 463: /* reserved_type: TIMESTAMP  */
#line 2621 "sqlparser_sql2003.y"
                        { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "TIMESTAMP"); }
#line 7983 "sqlparser_sql2003_bison.cpp"
    break;

  case 464: /* reserved_type: TIME  */
#line 2622 "sqlparser_sql2003.y"
                        { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "TIME");  }
#line 7989 "sqlparser_sql2003_bison.cpp"
    break;

  case 465: /* reserved_type: CHARACTERS  */
#line 2623 "sqlparser_sql2003.y"
                                { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "CHARACTERS"); }
#line 7995 "sqlparser_sql2003_bison.cpp"
    break;

  case 466: /* reserved_type: DATE  */
#line 2624 "sqlparser_sql2003.y"
                { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "DATE"); }
#line 8001 "sqlparser_sql2003_bison.cpp"
    break;

  case 467: /* reserved_type: FLOAT  */
#line 2625 "sqlparser_sql2003.y"
                 { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "FLOAT"); }
#line 8007 "sqlparser_sql2003_bison.cpp"
    break;

  case 468: /* reserved_type: CHAR  */
#line 2626 "sqlparser_sql2003.y"
                { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "CHAR"); }
#line 8013 "sqlparser_sql2003_bison.cpp"
    break;

  case 469: /* reserved_type: NCHAR  */
#line 2627 "sqlparser_sql2003.y"
                 { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "NCHAR"); }
#line 8019 "sqlparser_sql2003_bison.cpp"
    break;

  case 470: /* reserved_type: CHARACTER  */
#line 2628 "sqlparser_sql2003.y"
                     { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "CHARACTER"); }
#line 8025 "sqlparser_sql2003_bison.cpp"
    break;

  case 471: /* reserved_type: NUMERIC  */
#line 2629 "sqlparser_sql2003.y"
                   { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "NUMERIC"); }
#line 8031 "sqlparser_sql2003_bison.cpp"
    break;

  case 472: /* reserved_type: DECIMAL  */
#line 2630 "sqlparser_sql2003.y"
                     { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "DECIMAL"); }
#line 8037 "sqlparser_sql2003_bison.cpp"
    break;

  case 473: /* reserved_type: REAL  */
#line 2631 "sqlparser_sql2003.y"
                { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "REAL"); }
#line 8043 "sqlparser_sql2003_bison.cpp"
    break;

  case 474: /* reserved_type: BIGINT  */
#line 2632 "sqlparser_sql2003.y"
                  { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "BIGINT"); }
#line 8049 "sqlparser_sql2003_bison.cpp"
    break;

  case 475: /* reserved_type: INTEGER  */
#line 2633 "sqlparser_sql2003.y"
                     { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "INTEGER"); }
#line 8055 "sqlparser_sql2003_bison.cpp"
    break;

  case 476: /* reserved_type: SMALLINT  */
#line 2634 "sqlparser_sql2003.y"
                    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "SMALLINT"); }
#line 8061 "sqlparser_sql2003_bison.cpp"
    break;

  case 477: /* reserved_type: VARCHAR  */
#line 2635 "sqlparser_sql2003.y"
                        { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "VARCHAR"); }
#line 8067 "sqlparser_sql2003_bison.cpp"
    break;

  case 478: /* reserved_type: NVARCHAR  */
#line 2636 "sqlparser_sql2003.y"
                    { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "NVARCHAR"); }
#line 8073 "sqlparser_sql2003_bison.cpp"
    break;

  case 481: /* reserved_func: TRIM  */
#line 2642 "sqlparser_sql2003.y"
                                { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "TRIM"); }
#line 8079 "sqlparser_sql2003_bison.cpp"
    break;

  case 482: /* reserved_func: CONVERT  */
#line 2643 "sqlparser_sql2003.y"
                                { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "CONVERT"); }
#line 8085 "sqlparser_sql2003_bison.cpp"
    break;

  case 483: /* reserved_func: CAST  */
#line 2644 "sqlparser_sql2003.y"
                        { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "CAST"); }
#line 8091 "sqlparser_sql2003_bison.cpp"
    break;

  case 484: /* reserved_other: K  */
#line 2647 "sqlparser_sql2003.y"
                        { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "K"); }
#line 8097 "sqlparser_sql2003_bison.cpp"
    break;

  case 485: /* reserved_other: M  */
#line 2648 "sqlparser_sql2003.y"
                        { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "M"); }
#line 8103 "sqlparser_sql2003_bison.cpp"
    break;

  case 486: /* reserved_other: G  */
#line 2649 "sqlparser_sql2003.y"
                        { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "G"); }
#line 8109 "sqlparser_sql2003_bison.cpp"
    break;

  case 487: /* reserved_other: CODE_UNITS  */
#line 2650 "sqlparser_sql2003.y"
                                { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "CODE_UNITS"); }
#line 8115 "sqlparser_sql2003_bison.cpp"
    break;

  case 488: /* reserved_other: CORRESPONDING  */
#line 2651 "sqlparser_sql2003.y"
                        { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "CORRESPONDING"); }
#line 8121 "sqlparser_sql2003_bison.cpp"
    break;

  case 489: /* reserved_other: FOLLOWING  */
#line 2652 "sqlparser_sql2003.y"
                        { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "FOLLOWING"); }
#line 8127 "sqlparser_sql2003_bison.cpp"
    break;

  case 490: /* reserved_other: INTERVAL  */
#line 2653 "sqlparser_sql2003.y"
                        { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "INTERVAL"); }
#line 8133 "sqlparser_sql2003_bison.cpp"
    break;

  case 491: /* reserved_other: LARGE  */
#line 2654 "sqlparser_sql2003.y"
                        { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "LARGE"); }
#line 8139 "sqlparser_sql2003_bison.cpp"
    break;

  case 492: /* reserved_other: MULTISET  */
#line 2655 "sqlparser_sql2003.y"
                        { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "MULTISET"); }
#line 8145 "sqlparser_sql2003_bison.cpp"
    break;

  case 493: /* reserved_other: OBJECT  */
#line 2656 "sqlparser_sql2003.y"
                        { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "OBJECT"); }
#line 8151 "sqlparser_sql2003_bison.cpp"
    break;

  case 494: /* reserved_other: OCTETS  */
#line 2657 "sqlparser_sql2003.y"
                        { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "OCTETS"); }
#line 8157 "sqlparser_sql2003_bison.cpp"
    break;

  case 495: /* reserved_other: ONLY  */
#line 2658 "sqlparser_sql2003.y"
                        { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "ONLY"); }
#line 8163 "sqlparser_sql2003_bison.cpp"
    break;

  case 496: /* reserved_other: PRECEDING  */
#line 2659 "sqlparser_sql2003.y"
                        { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "PRECEDING"); }
#line 8169 "sqlparser_sql2003_bison.cpp"
    break;

  case 497: /* reserved_other: PRECISION  */
#line 2660 "sqlparser_sql2003.y"
                        { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "PRECISION"); }
#line 8175 "sqlparser_sql2003_bison.cpp"
    break;

  case 498: /* reserved_other: RECURSIVE  */
#line 2661 "sqlparser_sql2003.y"
                        { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "RECURSIVE"); }
#line 8181 "sqlparser_sql2003_bison.cpp"
    break;

  case 499: /* reserved_other: REF  */
#line 2662 "sqlparser_sql2003.y"
                                { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "REF"); }
#line 8187 "sqlparser_sql2003_bison.cpp"
    break;

  case 500: /* reserved_other: ROW  */
#line 2663 "sqlparser_sql2003.y"
                                { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "ROW"); }
#line 8193 "sqlparser_sql2003_bison.cpp"
    break;

  case 501: /* reserved_other: SCOPE  */
#line 2664 "sqlparser_sql2003.y"
                        { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "SCOPE"); }
#line 8199 "sqlparser_sql2003_bison.cpp"
    break;

  case 502: /* reserved_other: SECOND  */
#line 2665 "sqlparser_sql2003.y"
                        { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "SECOND"); }
#line 8205 "sqlparser_sql2003_bison.cpp"
    break;

  case 503: /* reserved_other: UNBOUNDED  */
#line 2666 "sqlparser_sql2003.y"
                        { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "UNBOUNDED"); }
#line 8211 "sqlparser_sql2003_bison.cpp"
    break;

  case 504: /* reserved_other: WITHOUT  */
#line 2667 "sqlparser_sql2003.y"
                        { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "WITHOUT"); }
#line 8217 "sqlparser_sql2003_bison.cpp"
    break;

  case 505: /* reserved_other: ZONE  */
#line 2668 "sqlparser_sql2003.y"
                        { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "ZONE"); }
#line 8223 "sqlparser_sql2003_bison.cpp"
    break;

  case 506: /* reserved_other: FOR  */
#line 2669 "sqlparser_sql2003.y"
                                { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "FOR"); }
#line 8229 "sqlparser_sql2003_bison.cpp"
    break;

  case 507: /* reserved_other: OF  */
#line 2670 "sqlparser_sql2003.y"
                                { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "OF"); }
#line 8235 "sqlparser_sql2003_bison.cpp"
    break;

  case 508: /* reserved_other: READ  */
#line 2671 "sqlparser_sql2003.y"
                        { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "READ"); }
#line 8241 "sqlparser_sql2003_bison.cpp"
    break;

  case 509: /* reserved_other: DESC  */
#line 2672 "sqlparser_sql2003.y"
                        { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "DESC"); }
#line 8247 "sqlparser_sql2003_bison.cpp"
    break;

  case 510: /* reserved_other: TIES  */
#line 2673 "sqlparser_sql2003.y"
                        { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "TIES"); }
#line 8253 "sqlparser_sql2003_bison.cpp"
    break;

  case 511: /* reserved_other: SETS  */
#line 2674 "sqlparser_sql2003.y"
                        { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "SETS"); }
#line 8259 "sqlparser_sql2003_bison.cpp"
    break;

  case 512: /* reserved_other: OTHERS  */
#line 2675 "sqlparser_sql2003.y"
                        { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "OTHERS"); }
#line 8265 "sqlparser_sql2003_bison.cpp"
    break;

  case 513: /* reserved_other: EXCLUDE  */
#line 2676 "sqlparser_sql2003.y"
                        { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "EXCLUDE"); }
#line 8271 "sqlparser_sql2003_bison.cpp"
    break;

  case 514: /* reserved_other: ASC  */
#line 2677 "sqlparser_sql2003.y"
                                { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "ASC"); }
#line 8277 "sqlparser_sql2003_bison.cpp"
    break;

  case 515: /* reserved_other: COALESCE  */
#line 2678 "sqlparser_sql2003.y"
                                { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "COALESCE"); }
#line 8283 "sqlparser_sql2003_bison.cpp"
    break;

  case 516: /* reserved_other: NULLIF  */
#line 2679 "sqlparser_sql2003.y"
                                        { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "NULLIF"); }
#line 8289 "sqlparser_sql2003_bison.cpp"
    break;

  case 517: /* reserved_other: DECLARE  */
#line 2680 "sqlparser_sql2003.y"
                        { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "DECLARE"); }
#line 8295 "sqlparser_sql2003_bison.cpp"
    break;

  case 518: /* reserved_other: FILE_TABLE  */
#line 2681 "sqlparser_sql2003.y"
                           { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "FileTable"); }
#line 8301 "sqlparser_sql2003_bison.cpp"
    break;

  case 523: /* opt_if_exists_clause: %empty  */
#line 2692 "sqlparser_sql2003.y"
                { (yyval.node) = nullptr; }
#line 8307 "sqlparser_sql2003_bison.cpp"
    break;

  case 524: /* opt_if_exists_clause: IF EXISTS  */
#line 2693 "sqlparser_sql2003.y"
                { (yyval.node) = Node::makeTerminalNode(E_IDENTIFIER, "IF EXISTS"); }
#line 8313 "sqlparser_sql2003_bison.cpp"
    break;

  case 525: /* opt_top_clause: %empty  */
#line 2696 "sqlparser_sql2003.y"
                { (yyval.node) = nullptr; }
#line 8319 "sqlparser_sql2003_bison.cpp"
    break;

  case 526: /* opt_top_clause: TOP intnum_parens  */
#line 2698 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeTerminalNode(E_STRING, "TOP "+(yyvsp[0].node)->text() );
    delete((yyvsp[0].node));
}
#line 8328 "sqlparser_sql2003_bison.cpp"
    break;

  case 529: /* intnum_parens: '(' intnum_parens ')'  */
#line 2707 "sqlparser_sql2003.y"
{
    (yyval.node) = Node::makeNonTerminalNode(E_EXPR_LIST_WITH_PARENS, E_PARENS_PROPERTY_CNT, (yyvsp[-1].node));
    (yyval.node)->serialize_format = &SINGLE_WITH_PARENS_SERIALIZE_FORMAT;
}
#line 8337 "sqlparser_sql2003_bison.cpp"
    break;


#line 8341 "sqlparser_sql2003_bison.cpp"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == SQL2003_SQL2003_EMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      {
        yypcontext_t yyctx
          = {yyssp, yytoken, &yylloc};
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == -1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *,
                             YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (yymsg)
              {
                yysyntax_error_status
                  = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
                yymsgp = yymsg;
              }
            else
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = YYENOMEM;
              }
          }
        yyerror (&yylloc, result, scanner, yymsgp);
        if (yysyntax_error_status == YYENOMEM)
          YYNOMEM;
      }
    }

  yyerror_range[1] = yylloc;
  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= SQL2003_YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == SQL2003_YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, &yylloc, result, scanner);
          yychar = SQL2003_SQL2003_EMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp, yylsp, result, scanner);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  ++yylsp;
  YYLLOC_DEFAULT (*yylsp, yyerror_range, 2);

  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (&yylloc, result, scanner, YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != SQL2003_SQL2003_EMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, &yylloc, result, scanner);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp, yylsp, result, scanner);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
  return yyresult;
}

#line 2712 "sqlparser_sql2003.y"

/*********************************
 ** Section 4: Additional C code
 *********************************/

/* empty */
