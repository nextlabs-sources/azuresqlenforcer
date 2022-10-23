/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_SQL2003_SQLPARSER_SQL2003_BISON_H_INCLUDED
# define YY_SQL2003_SQLPARSER_SQL2003_BISON_H_INCLUDED
/* Debug traces.  */
#ifndef SQL2003_DEBUG
# if defined YYDEBUG
#if YYDEBUG
#   define SQL2003_DEBUG 1
#  else
#   define SQL2003_DEBUG 0
#  endif
# else /* ! defined YYDEBUG */
#  define SQL2003_DEBUG 0
# endif /* ! defined YYDEBUG */
#endif  /* ! defined SQL2003_DEBUG */
#if SQL2003_DEBUG
extern int sql2003_debug;
#endif
/* "%code requires" blocks.  */
#line 42 "sqlparser_sql2003.y"

// %code requires block

#include "node.h"

#line 63 "sqlparser_sql2003_bison.h"

/* Token kinds.  */
#ifndef SQL2003_TOKENTYPE
# define SQL2003_TOKENTYPE
  enum sql2003_tokentype
  {
    SQL2003_SQL2003_EMPTY = -2,
    SQL2003_YYEOF = 0,             /* "end of file"  */
    SQL2003_SQL2003_error = 256,   /* error  */
    SQL2003_SQL2003_UNDEF = 257,   /* "invalid token"  */
    SQL2003_NAME = 258,            /* NAME  */
    SQL2003_STRING = 259,          /* STRING  */
    SQL2003_INTNUM = 260,          /* INTNUM  */
    SQL2003_BOOL = 261,            /* BOOL  */
    SQL2003_APPROXNUM = 262,       /* APPROXNUM  */
    SQL2003_NULLX = 263,           /* NULLX  */
    SQL2003_UNKNOWN = 264,         /* UNKNOWN  */
    SQL2003_QUESTIONMARK = 265,    /* QUESTIONMARK  */
    SQL2003_PARAM = 266,           /* PARAM  */
    SQL2003_CREATE_TABLE = 267,    /* CREATE_TABLE  */
    SQL2003_CREATE_TABLE_REST = 268, /* CREATE_TABLE_REST  */
    SQL2003_UMINUS = 269,          /* UMINUS  */
    SQL2003_ALL = 270,             /* ALL  */
    SQL2003_AND = 271,             /* AND  */
    SQL2003_ANY = 272,             /* ANY  */
    SQL2003_ARRAY = 273,           /* ARRAY  */
    SQL2003_AS = 274,              /* AS  */
    SQL2003_ASC = 275,             /* ASC  */
    SQL2003_AVG = 276,             /* AVG  */
    SQL2003_BETWEEN = 277,         /* BETWEEN  */
    SQL2003_BIGINT = 278,          /* BIGINT  */
    SQL2003_BINARY = 279,          /* BINARY  */
    SQL2003_BLOB = 280,            /* BLOB  */
    SQL2003_BOOLEAN = 281,         /* BOOLEAN  */
    SQL2003_BY = 282,              /* BY  */
    SQL2003_CALL = 283,            /* CALL  */
    SQL2003_CASE = 284,            /* CASE  */
    SQL2003_CAST = 285,            /* CAST  */
    SQL2003_CHAR = 286,            /* CHAR  */
    SQL2003_CHARACTER = 287,       /* CHARACTER  */
    SQL2003_CHARACTERS = 288,      /* CHARACTERS  */
    SQL2003_CLOB = 289,            /* CLOB  */
    SQL2003_CNNOP = 290,           /* CNNOP  */
    SQL2003_COALESCE = 291,        /* COALESCE  */
    SQL2003_CODE_UNITS = 292,      /* CODE_UNITS  */
    SQL2003_COLLATE = 293,         /* COLLATE  */
    SQL2003_COMP_EQ = 294,         /* COMP_EQ  */
    SQL2003_COMP_GE = 295,         /* COMP_GE  */
    SQL2003_COMP_GT = 296,         /* COMP_GT  */
    SQL2003_COMP_LE = 297,         /* COMP_LE  */
    SQL2003_COMP_LT = 298,         /* COMP_LT  */
    SQL2003_COMP_NE = 299,         /* COMP_NE  */
    SQL2003_CONVERT = 300,         /* CONVERT  */
    SQL2003_CORRESPONDING = 301,   /* CORRESPONDING  */
    SQL2003_COUNT = 302,           /* COUNT  */
    SQL2003_CREATE = 303,          /* CREATE  */
    SQL2003_CROSS = 304,           /* CROSS  */
    SQL2003_CUME_DIST = 305,       /* CUME_DIST  */
    SQL2003_CURRENT = 306,         /* CURRENT  */
    SQL2003_CURRENT_TIMESTAMP = 307, /* CURRENT_TIMESTAMP  */
    SQL2003_CURRENT_USER = 308,    /* CURRENT_USER  */
    SQL2003_DATE = 309,            /* DATE  */
    SQL2003_DAY = 310,             /* DAY  */
    SQL2003_DEC = 311,             /* DEC  */
    SQL2003_DECIMAL = 312,         /* DECIMAL  */
    SQL2003_DEFAULT = 313,         /* DEFAULT  */
    SQL2003_DELETE = 314,          /* DELETE  */
    SQL2003_DENSE_RANK = 315,      /* DENSE_RANK  */
    SQL2003_DESC = 316,            /* DESC  */
    SQL2003_DISTINCT = 317,        /* DISTINCT  */
    SQL2003_DOUBLE = 318,          /* DOUBLE  */
    SQL2003_DROP = 319,            /* DROP  */
    SQL2003_ELSE = 320,            /* ELSE  */
    SQL2003_END = 321,             /* END  */
    SQL2003_END_P = 322,           /* END_P  */
    SQL2003_ESCAPE = 323,          /* ESCAPE  */
    SQL2003_ERROR = 324,           /* ERROR  */
    SQL2003_EXCEPT = 325,          /* EXCEPT  */
    SQL2003_EXCLUDE = 326,         /* EXCLUDE  */
    SQL2003_EXEC = 327,            /* EXEC  */
    SQL2003_EXECUTE = 328,         /* EXECUTE  */
    SQL2003_EXISTS = 329,          /* EXISTS  */
    SQL2003_FILE_TABLE = 330,      /* FILE_TABLE  */
    SQL2003_FLOAT = 331,           /* FLOAT  */
    SQL2003_FOLLOWING = 332,       /* FOLLOWING  */
    SQL2003_FOR = 333,             /* FOR  */
    SQL2003_FROM = 334,            /* FROM  */
    SQL2003_FULL = 335,            /* FULL  */
    SQL2003_G = 336,               /* G  */
    SQL2003_GROUP = 337,           /* GROUP  */
    SQL2003_GROUPING = 338,        /* GROUPING  */
    SQL2003_HAVING = 339,          /* HAVING  */
    SQL2003_HOUR = 340,            /* HOUR  */
    SQL2003_IF = 341,              /* IF  */
    SQL2003_IN = 342,              /* IN  */
    SQL2003_INNER = 343,           /* INNER  */
    SQL2003_INSERT = 344,          /* INSERT  */
    SQL2003_INT = 345,             /* INT  */
    SQL2003_INTEGER = 346,         /* INTEGER  */
    SQL2003_INTERSECT = 347,       /* INTERSECT  */
    SQL2003_INTERVAL = 348,        /* INTERVAL  */
    SQL2003_INTO = 349,            /* INTO  */
    SQL2003_IS = 350,              /* IS  */
    SQL2003_JOIN = 351,            /* JOIN  */
    SQL2003_K = 352,               /* K  */
    SQL2003_LARGE = 353,           /* LARGE  */
    SQL2003_LEFT = 354,            /* LEFT  */
    SQL2003_LIKE = 355,            /* LIKE  */
    SQL2003_M = 356,               /* M  */
    SQL2003_MAX = 357,             /* MAX  */
    SQL2003_MIN = 358,             /* MIN  */
    SQL2003_MINUTE = 359,          /* MINUTE  */
    SQL2003_MOD = 360,             /* MOD  */
    SQL2003_MONTH = 361,           /* MONTH  */
    SQL2003_MULTISET = 362,        /* MULTISET  */
    SQL2003_NATIONAL = 363,        /* NATIONAL  */
    SQL2003_NATURAL = 364,         /* NATURAL  */
    SQL2003_NCHAR = 365,           /* NCHAR  */
    SQL2003_NCLOB = 366,           /* NCLOB  */
    SQL2003_NO = 367,              /* NO  */
    SQL2003_NOT = 368,             /* NOT  */
    SQL2003_NULLIF = 369,          /* NULLIF  */
    SQL2003_NUMERIC = 370,         /* NUMERIC  */
    SQL2003_OBJECT = 371,          /* OBJECT  */
    SQL2003_OCTETS = 372,          /* OCTETS  */
    SQL2003_OF = 373,              /* OF  */
    SQL2003_ON = 374,              /* ON  */
    SQL2003_ONLY = 375,            /* ONLY  */
    SQL2003_OR = 376,              /* OR  */
    SQL2003_ORDER = 377,           /* ORDER  */
    SQL2003_OTHERS = 378,          /* OTHERS  */
    SQL2003_OUTER = 379,           /* OUTER  */
    SQL2003_OVER = 380,            /* OVER  */
    SQL2003_PARTITION = 381,       /* PARTITION  */
    SQL2003_PERCENT_RANK = 382,    /* PERCENT_RANK  */
    SQL2003_PRECEDING = 383,       /* PRECEDING  */
    SQL2003_PRECISION = 384,       /* PRECISION  */
    SQL2003_RANGE = 385,           /* RANGE  */
    SQL2003_RANK = 386,            /* RANK  */
    SQL2003_READ = 387,            /* READ  */
    SQL2003_REAL = 388,            /* REAL  */
    SQL2003_RECURSIVE = 389,       /* RECURSIVE  */
    SQL2003_REF = 390,             /* REF  */
    SQL2003_RIGHT = 391,           /* RIGHT  */
    SQL2003_ROW = 392,             /* ROW  */
    SQL2003_ROWS = 393,            /* ROWS  */
    SQL2003_ROW_NUMBER = 394,      /* ROW_NUMBER  */
    SQL2003_SCOPE = 395,           /* SCOPE  */
    SQL2003_SECOND = 396,          /* SECOND  */
    SQL2003_SELECT = 397,          /* SELECT  */
    SQL2003_SESSION_USER = 398,    /* SESSION_USER  */
    SQL2003_SET = 399,             /* SET  */
    SQL2003_SETS = 400,            /* SETS  */
    SQL2003_SMALLINT = 401,        /* SMALLINT  */
    SQL2003_SOME = 402,            /* SOME  */
    SQL2003_SP_EXECUTESQL = 403,   /* SP_EXECUTESQL  */
    SQL2003_STDDEV_POP = 404,      /* STDDEV_POP  */
    SQL2003_STDDEV_SAMP = 405,     /* STDDEV_SAMP  */
    SQL2003_SUM = 406,             /* SUM  */
    SQL2003_SYSTEM_USER = 407,     /* SYSTEM_USER  */
    SQL2003_TABLE = 408,           /* TABLE  */
    SQL2003_THEN = 409,            /* THEN  */
    SQL2003_TIES = 410,            /* TIES  */
    SQL2003_TIME = 411,            /* TIME  */
    SQL2003_TIMESTAMP = 412,       /* TIMESTAMP  */
    SQL2003_TO = 413,              /* TO  */
    SQL2003_UNBOUNDED = 414,       /* UNBOUNDED  */
    SQL2003_UNION = 415,           /* UNION  */
    SQL2003_UPDATE = 416,          /* UPDATE  */
    SQL2003_USING = 417,           /* USING  */
    SQL2003_VALUES = 418,          /* VALUES  */
    SQL2003_VARCHAR = 419,         /* VARCHAR  */
    SQL2003_VARYING = 420,         /* VARYING  */
    SQL2003_VAR_POP = 421,         /* VAR_POP  */
    SQL2003_VAR_SAMP = 422,        /* VAR_SAMP  */
    SQL2003_WHEN = 423,            /* WHEN  */
    SQL2003_WHERE = 424,           /* WHERE  */
    SQL2003_WITH = 425,            /* WITH  */
    SQL2003_WITHOUT = 426,         /* WITHOUT  */
    SQL2003_YEAR = 427,            /* YEAR  */
    SQL2003_ZONE = 428,            /* ZONE  */
    SQL2003_TOP = 429,             /* TOP  */
    SQL2003_NVARCHAR = 430,        /* NVARCHAR  */
    SQL2003_USE = 431,             /* USE  */
    SQL2003_TRIM = 432,            /* TRIM  */
    SQL2003_UNPIVOT = 433,         /* UNPIVOT  */
    SQL2003_PIVOT = 434,           /* PIVOT  */
    SQL2003_DECLARE = 435          /* DECLARE  */
  };
  typedef enum sql2003_tokentype sql2003_token_kind_t;
#endif

/* Value type.  */
#if ! defined SQL2003_STYPE && ! defined SQL2003_STYPE_IS_DECLARED
union SQL2003_STYPE
{
#line 85 "sqlparser_sql2003.y"

    struct Node* node;
    int    ival;
    NodeType nodetype;

#line 266 "sqlparser_sql2003_bison.h"

};
typedef union SQL2003_STYPE SQL2003_STYPE;
# define SQL2003_STYPE_IS_TRIVIAL 1
# define SQL2003_STYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined SQL2003_LTYPE && ! defined SQL2003_LTYPE_IS_DECLARED
typedef struct SQL2003_LTYPE SQL2003_LTYPE;
struct SQL2003_LTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define SQL2003_LTYPE_IS_DECLARED 1
# define SQL2003_LTYPE_IS_TRIVIAL 1
#endif




int sql2003_parse (ParseResult* result, yyscan_t scanner);


#endif /* !YY_SQL2003_SQLPARSER_SQL2003_BISON_H_INCLUDED  */
