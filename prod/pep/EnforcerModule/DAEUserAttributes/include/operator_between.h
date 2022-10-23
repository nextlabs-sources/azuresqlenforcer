#ifndef USERATTR_SPECIAL_H
#define USERATTR_SPECIAL_H

#include <time.h>
/*
 * TIMESTAMP@ IS [NOT] 2010-02-15 02:30:15 ~ 2010-02-20 13:30:15 [ time_zone ]
 * TIME@ IS [NOT] 02:30:15 ~ 13:30:15 [ <time zone> ]
 * IP@ IS [NOT] 0.0.0.0 ~ 255.255.255.255 
 * time_zone: UTC +8
 *              UTC -7
 *                 UTC +8:45
 *
 *
 *    2010-02-15 02:30:15 ~ 2010-02-20 13:30:15
 *
 *    TIMESTAMP@ IS [NOT] 2010-02-15 02:30:15 ~ 2010-02-20 13:30:15 UTC +8:45
 *
 *
 * */
#define TIMESTAMP_CHAR "TIMESTAMP@" 
#define TIME_CHAR "TIME@" 
#define IP_CHAR "IP@" 
#define APPLICATION_CHAR "APPLICATION@" 

enum BETWEEN_OP_RESULT { BETWEEN_OP_ERR, BETWEEN_OP_TR, BETWEEN_OP_FL };

/*
 * [from, to]
 * from > to will return ERR
 * */
BETWEEN_OP_RESULT cur_timestamp_between(const char *v);
/*
 * [from, to]
 * from > to means not (to, from)
 * */
BETWEEN_OP_RESULT cur_time_between(const char *v);
/*
* [from, to]
* from > to will return ERR
* one of the ip in 'v' return TR 
* */
BETWEEN_OP_RESULT cur_ip_between(const char *v);


/*
* APPLICATION@ IS [NOT] appname.exe
* This rule applies only to ODBC
* if local appname same as "appname.exe" and return true, but return false.
*/
bool compare_with_cur_application(const char* strname);


#endif