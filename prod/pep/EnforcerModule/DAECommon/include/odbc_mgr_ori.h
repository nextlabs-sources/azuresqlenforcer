#ifndef ODBC_MGR_ORI
#define ODBC_MGR_ORI

/*
 * this is a Cross platform ODBC LOWER API
 *  
 */

#ifdef WIN32
#include "ODBCMgrApi.h"
#define ODBC_MGR_INS    ODBCMgrApi::Instance()
#else
#include "ODBCMgrApi.h"
#define ODBC_MGR_INS    ODBCMgrApi::Instance()
#endif

#endif