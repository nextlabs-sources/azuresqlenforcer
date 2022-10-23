#include "ODBCMgrApi.h"
#include "commfun.h"

#define GET_ODBC_PROC(funname) this->funname=(funname##Fun) CommonFun::GetProcAddress(hODBCMgr, #funname);

ODBCMgrApi::ODBCMgrApi() {
    if(!m_bInit){
        m_bInit = InitODBCMgrApi();
    }
}

bool ODBCMgrApi::InitODBCMgrApi() {
	auto hODBCMgr = CommonFun::LoadShareLibrary(CommonFun::GetODBCOriModule().c_str());
	if(nullptr == hODBCMgr)
		return false;

	GET_ODBC_PROC(SQLAllocHandle);
	GET_ODBC_PROC(SQLSetEnvAttr);
	GET_ODBC_PROC(SQLDriverConnect);
	GET_ODBC_PROC(SQLDriverConnectA);
	GET_ODBC_PROC(SQLDriverConnectW);
	GET_ODBC_PROC(SQLExecDirect);
	GET_ODBC_PROC(SQLExecDirectA);
	GET_ODBC_PROC(SQLExecDirectW);
	GET_ODBC_PROC(SQLNumResultCols);
	GET_ODBC_PROC(SQLCloseCursor);
	GET_ODBC_PROC(SQLFreeStmt);
	GET_ODBC_PROC(SQLBindCol);
	GET_ODBC_PROC(SQLFetch);
	GET_ODBC_PROC(SQLGetData);
	GET_ODBC_PROC(SQLGetDiagRec);
	GET_ODBC_PROC(SQLGetDiagRecA);
	GET_ODBC_PROC(SQLGetDiagRecW);
	GET_ODBC_PROC(SQLConnect);
	GET_ODBC_PROC(SQLConnectA);
	GET_ODBC_PROC(SQLConnectW);
    GET_ODBC_PROC(SQLBrowseConnect);
	GET_ODBC_PROC(SQLBrowseConnectA);
	GET_ODBC_PROC(SQLBrowseConnectW);
	GET_ODBC_PROC(SQLFreeHandle);
	GET_ODBC_PROC(SQLDisconnect);
	GET_ODBC_PROC(SQLGetInfo);

	GET_ODBC_PROC(SQLGetInfoA);
	GET_ODBC_PROC(SQLGetInfoW);

	//add by  jeff 19-04-18
	GET_ODBC_PROC(SQLAllocConnect);
	GET_ODBC_PROC(SQLAllocEnv);
	GET_ODBC_PROC(SQLAllocHandleStd);
	GET_ODBC_PROC(SQLAllocStmt);
	//GET_ODBC_PROC(SQLBindCol);
	GET_ODBC_PROC(SQLBindParam);
	GET_ODBC_PROC(SQLBindParameter);
	GET_ODBC_PROC(SQLBulkOperations);
	//Sqlcancel
	//GET_ODBC_PROC(SQLCloseCursor);

	GET_ODBC_PROC(SQLColAttribute);
	GET_ODBC_PROC(SQLColAttributeA);
	GET_ODBC_PROC(SQLColAttributeW);
	GET_ODBC_PROC(SQLColAttributes);
	GET_ODBC_PROC(SQLColAttributesA);
	GET_ODBC_PROC(SQLColAttributesW);
	GET_ODBC_PROC(SQLColumnPrivileges);
	GET_ODBC_PROC(SQLColumnPrivilegesA);
	GET_ODBC_PROC(SQLColumnPrivilegesW);
	GET_ODBC_PROC(SQLColumns);

	GET_ODBC_PROC(SQLColumnsA);
	GET_ODBC_PROC(SQLColumnsW);
	GET_ODBC_PROC(SQLDescribeCol);
	GET_ODBC_PROC(SQLDescribeColA);
	GET_ODBC_PROC(SQLDescribeColW);
	GET_ODBC_PROC(SQLDescribeParam);
	GET_ODBC_PROC(SQLEndTran);
	GET_ODBC_PROC(SQLError);
	GET_ODBC_PROC(SQLErrorA);
	GET_ODBC_PROC(SQLErrorW);

	GET_ODBC_PROC(SQLExecute);
	GET_ODBC_PROC(SQLExtendedFetch);
	//GET_ODBC_PROC(SQLFetch);
	GET_ODBC_PROC(SQLFetchScroll);
	GET_ODBC_PROC(SQLForeignKeys);
	GET_ODBC_PROC(SQLForeignKeysA);
	GET_ODBC_PROC(SQLForeignKeysW);
	GET_ODBC_PROC(SQLFreeConnect);
	//GET_ODBC_PROC(SQLFreeStmt);
	GET_ODBC_PROC(SQLGetConnectAttr);

	GET_ODBC_PROC(SQLGetConnectAttrA);
	GET_ODBC_PROC(SQLGetConnectAttrW);
	GET_ODBC_PROC(SQLGetConnectOption);
	GET_ODBC_PROC(SQLGetConnectOptionA);
	GET_ODBC_PROC(SQLGetConnectOptionW);
	GET_ODBC_PROC(SQLGetCursorName);
	GET_ODBC_PROC(SQLGetCursorNameA);
	GET_ODBC_PROC(SQLGetCursorNameW);
	//GET_ODBC_PROC(SQLGetData);
	GET_ODBC_PROC(SQLGetDiagField);


	GET_ODBC_PROC(SQLGetDiagFieldA);
	GET_ODBC_PROC(SQLGetDiagFieldW);
	GET_ODBC_PROC(SQLGetEnvAttr);
	GET_ODBC_PROC(SQLGetFunctions);
	GET_ODBC_PROC(SQLGetStmtAttr);
	GET_ODBC_PROC(SQLGetStmtAttrA);
	GET_ODBC_PROC(SQLGetStmtAttrW);
	GET_ODBC_PROC(SQLGetStmtOption);
	GET_ODBC_PROC(SQLGetTypeInfo);
	GET_ODBC_PROC(SQLGetTypeInfoA);

	GET_ODBC_PROC(SQLGetTypeInfoW);
	GET_ODBC_PROC(SQLMoreResults);
	GET_ODBC_PROC(SQLNativeSql);
	GET_ODBC_PROC(SQLNativeSqlA);
	GET_ODBC_PROC(SQLNativeSqlW);
	GET_ODBC_PROC(SQLNumParams);
	//GET_ODBC_PROC(SQLNumResultCols);
	GET_ODBC_PROC(SQLParamData);
	GET_ODBC_PROC(SQLParamOptions);
	GET_ODBC_PROC(SQLPrepare);


	GET_ODBC_PROC(SQLPrepareA);
	GET_ODBC_PROC(SQLPrepareW);
	GET_ODBC_PROC(SQLPrimaryKeys);
	GET_ODBC_PROC(SQLPrimaryKeysA);
	GET_ODBC_PROC(SQLPrimaryKeysW);
	GET_ODBC_PROC(SQLProcedureColumns);
	GET_ODBC_PROC(SQLProcedureColumnsA);
	GET_ODBC_PROC(SQLProcedureColumnsW);
	GET_ODBC_PROC(SQLProcedures);
	GET_ODBC_PROC(SQLProceduresA);


	GET_ODBC_PROC(SQLProceduresW);
	GET_ODBC_PROC(SQLPutData);
	GET_ODBC_PROC(SQLRowCount);
	GET_ODBC_PROC(SQLSetConnectAttr);
	GET_ODBC_PROC(SQLSetConnectAttrA);
	GET_ODBC_PROC(SQLSetConnectAttrW);
	GET_ODBC_PROC(SQLSetConnectOption);
	GET_ODBC_PROC(SQLSetConnectOptionA);
	GET_ODBC_PROC(SQLSetConnectOptionW);
	GET_ODBC_PROC(SQLSetCursorName);


	GET_ODBC_PROC(SQLSetCursorNameA);
	GET_ODBC_PROC(SQLSetCursorNameW);
	GET_ODBC_PROC(SQLSetParam);
	GET_ODBC_PROC(SQLSetPos);
	GET_ODBC_PROC(SQLSetScrollOptions);
	GET_ODBC_PROC(SQLSetStmtAttr);
	GET_ODBC_PROC(SQLSetStmtAttrA);
	GET_ODBC_PROC(SQLSetStmtAttrW);
	GET_ODBC_PROC(SQLSetStmtOption);
	GET_ODBC_PROC(SQLSpecialColumns);



	GET_ODBC_PROC(SQLSpecialColumnsA);
	GET_ODBC_PROC(SQLSpecialColumnsW);
	GET_ODBC_PROC(SQLStatistics);
	GET_ODBC_PROC(SQLStatisticsA);
	GET_ODBC_PROC(SQLStatisticsW);
	GET_ODBC_PROC(SQLTablePrivileges);
	GET_ODBC_PROC(SQLTablePrivilegesA);
	GET_ODBC_PROC(SQLTablePrivilegesW);
	GET_ODBC_PROC(SQLTables);
	GET_ODBC_PROC(SQLTablesA);

	
	GET_ODBC_PROC(SQLTablesW);
	GET_ODBC_PROC(SQLTransact);


	//19-04-24
	GET_ODBC_PROC(SQLCancel);
	GET_ODBC_PROC(SQLCancelHandle);
	GET_ODBC_PROC(SQLCompleteAsync);
	GET_ODBC_PROC(SQLCopyDesc);
	GET_ODBC_PROC(SQLDataSources);
	GET_ODBC_PROC(SQLDataSourcesA);
	GET_ODBC_PROC(SQLDataSourcesW);
	GET_ODBC_PROC(SQLDrivers);
	GET_ODBC_PROC(SQLDriversA);
	GET_ODBC_PROC(SQLDriversW);
	GET_ODBC_PROC(SQLFreeEnv);
	//GET_ODBC_PROC(SQLFreeHandle);
	GET_ODBC_PROC(SQLGetDescField);

	GET_ODBC_PROC(SQLGetDescFieldA);
	GET_ODBC_PROC(SQLGetDescFieldW);
	GET_ODBC_PROC(SQLGetDescRec);
	GET_ODBC_PROC(SQLGetDescRecA);
	GET_ODBC_PROC(SQLGetDescRecW);
	GET_ODBC_PROC(SQLSetDescField);
	GET_ODBC_PROC(SQLSetDescFieldA);
	GET_ODBC_PROC(SQLSetDescFieldW);
	GET_ODBC_PROC(SQLSetDescRec);
	//GET_ODBC_PROC(SQLSetEnvAttr);

    if(SQLAllocHandle == nullptr||
       SQLSetEnvAttr == nullptr ||
       SQLDriverConnect == nullptr ||
       SQLDriverConnectA == nullptr ||
       SQLDriverConnectW == nullptr ||
            SQLFreeEnv==nullptr ||
            SQLGetEnvAttr == nullptr ||
            SQLAllocEnv ==nullptr) {
        printf("load odbc error\n");
        return false;
    }


	return true;
}

