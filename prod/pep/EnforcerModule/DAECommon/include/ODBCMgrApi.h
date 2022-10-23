#ifndef ODBC_MGR_API_H
#define ODBC_MGR_API_H

#include "odbc.h"
class ODBCMgrApi
{
protected:
	ODBCMgrApi() ;
	ODBCMgrApi(const ODBCMgrApi&) {}

public:
	static ODBCMgrApi& Instance()
	{
		static ODBCMgrApi odbcApi;
		return odbcApi;
	}

	bool InitODBCMgrApi();

public:
	SQLAllocHandleFun  SQLAllocHandle = nullptr;
	SQLSetEnvAttrFun   SQLSetEnvAttr = nullptr;
	SQLDriverConnectFun SQLDriverConnect = nullptr;
	SQLDriverConnectAFun SQLDriverConnectA = nullptr;
	SQLDriverConnectWFun SQLDriverConnectW = nullptr;
	SQLExecDirectFun SQLExecDirect = nullptr;
	SQLExecDirectAFun SQLExecDirectA = nullptr;
	SQLExecDirectWFun    SQLExecDirectW = nullptr;
	SQLNumResultColsFun  SQLNumResultCols = nullptr;
	SQLCloseCursorFun   SQLCloseCursor = nullptr;
	SQLFreeStmtFun   SQLFreeStmt = nullptr;
	SQLBindColFun  SQLBindCol = nullptr;
	SQLFetchFun  SQLFetch = nullptr;
	SQLGetDataFun SQLGetData = nullptr;
	SQLGetDiagRecFun SQLGetDiagRec = nullptr;
	SQLGetDiagRecAFun SQLGetDiagRecA = nullptr;
	SQLGetDiagRecWFun SQLGetDiagRecW = nullptr;
	SQLConnectFun SQLConnect = nullptr;
	SQLConnectAFun SQLConnectA = nullptr;
	SQLConnectWFun SQLConnectW = nullptr;
	SQLBrowseConnectFun SQLBrowseConnect = nullptr;
	SQLBrowseConnectAFun SQLBrowseConnectA = nullptr;
	SQLBrowseConnectWFun SQLBrowseConnectW = nullptr;
    SQLFreeHandleFun SQLFreeHandle = nullptr;
	SQLDisconnectFun SQLDisconnect = nullptr;
	SQLGetInfoFun SQLGetInfo = nullptr;
	//add by jeff 19-04-18
	
	SQLCompleteAsyncFun SQLCompleteAsync = nullptr;
	SQLCancelHandleFun SQLCancelHandle = nullptr;
	SQLGetInfoAFun SQLGetInfoA = nullptr;
	SQLGetInfoWFun SQLGetInfoW = nullptr;
	//
	SQLAllocConnectFun SQLAllocConnect = nullptr;
	SQLAllocEnvFun SQLAllocEnv = nullptr;
	SQLAllocHandleStdFun SQLAllocHandleStd = nullptr;
	SQLAllocStmtFun  SQLAllocStmt = nullptr;
	//SQLBindColFun SQLBindCol = nullptr;
	SQLBindParamFun SQLBindParam = nullptr;
	SQLBindParameterFun SQLBindParameter = nullptr;
	SQLBulkOperationsFun SQLBulkOperations = nullptr;
	//Sqlcancel Sqlcancel = nullptr;
	//SQLCloseCursor SQLCloseCursor = nullptr;

	SQLColAttributeFun SQLColAttribute = nullptr;
	SQLColAttributeAFun SQLColAttributeA = nullptr;
	SQLColAttributeWFun SQLColAttributeW = nullptr;
	SQLColAttributesFun SQLColAttributes = nullptr;
	SQLColAttributesAFun SQLColAttributesA = nullptr;
	SQLColAttributesWFun SQLColAttributesW = nullptr;
	SQLColumnPrivilegesFun SQLColumnPrivileges = nullptr;
	SQLColumnPrivilegesAFun SQLColumnPrivilegesA = nullptr;
	SQLColumnPrivilegesWFun SQLColumnPrivilegesW = nullptr;
	SQLColumnsFun SQLColumns = nullptr;

	SQLColumnsAFun SQLColumnsA = nullptr;
	SQLColumnsWFun SQLColumnsW = nullptr;
	SQLDescribeColFun  SQLDescribeCol = nullptr;
	SQLDescribeColAFun SQLDescribeColA = nullptr;
	SQLDescribeColWFun SQLDescribeColW = nullptr;
	SQLDescribeParamFun SQLDescribeParam = nullptr;
	SQLEndTranFun SQLEndTran = nullptr;
	SQLErrorFun SQLError = nullptr;
	SQLErrorAFun SQLErrorA = nullptr;
	SQLErrorWFun SQLErrorW = nullptr;

	SQLExecuteFun SQLExecute = nullptr;
	SQLExtendedFetchFun SQLExtendedFetch = nullptr;
	//SQLFetchFun SQLFetch = nullptr;
	SQLFetchScrollFun SQLFetchScroll = nullptr;
	SQLForeignKeysFun SQLForeignKeys = nullptr;
	SQLForeignKeysAFun SQLForeignKeysA = nullptr;
	SQLForeignKeysWFun SQLForeignKeysW = nullptr;
	SQLFreeConnectFun SQLFreeConnect = nullptr;
	//SQLFreeStmtFun SQLFreeStmt = nullptr;
	SQLGetConnectAttrFun SQLGetConnectAttr = nullptr;

	SQLGetConnectAttrAFun SQLGetConnectAttrA = nullptr;
	SQLGetConnectAttrWFun SQLGetConnectAttrW = nullptr;
	SQLGetConnectOptionFun SQLGetConnectOption = nullptr;
	SQLGetConnectOptionAFun SQLGetConnectOptionA = nullptr;
	SQLGetConnectOptionWFun SQLGetConnectOptionW = nullptr;
	SQLGetCursorNameFun SQLGetCursorName  = nullptr;
	SQLGetCursorNameAFun SQLGetCursorNameA = nullptr;
	SQLGetCursorNameWFun SQLGetCursorNameW = nullptr;
	//SQLGetDataFun SQLGetData = nullptr;
	SQLGetDiagFieldFun SQLGetDiagField = nullptr;

	SQLGetDiagFieldAFun SQLGetDiagFieldA = nullptr;
	SQLGetDiagFieldWFun SQLGetDiagFieldW = nullptr;
	SQLGetEnvAttrFun SQLGetEnvAttr = nullptr;
	SQLGetFunctionsFun SQLGetFunctions = nullptr;
	SQLGetStmtAttrFun SQLGetStmtAttr = nullptr;
	SQLGetStmtAttrAFun SQLGetStmtAttrA = nullptr;
	SQLGetStmtAttrWFun SQLGetStmtAttrW  = nullptr;
	SQLGetStmtOptionFun SQLGetStmtOption = nullptr;
	SQLGetTypeInfoFun SQLGetTypeInfo = nullptr;
	SQLGetTypeInfoAFun SQLGetTypeInfoA = nullptr;

	SQLGetTypeInfoWFun SQLGetTypeInfoW = nullptr;
	SQLMoreResultsFun SQLMoreResults = nullptr;
	SQLNativeSqlFun SQLNativeSql = nullptr;
	SQLNativeSqlAFun SQLNativeSqlA = nullptr;
	SQLNativeSqlWFun SQLNativeSqlW = nullptr;
	SQLNumParamsFun SQLNumParams = nullptr;
	//SQLNumResultColsFun SQLNumResultCols = nullptr;
	SQLParamDataFun SQLParamData = nullptr;
	SQLParamOptionsFun SQLParamOptions = nullptr;
	SQLPrepareFun SQLPrepare = nullptr;

	SQLPrepareAFun SQLPrepareA = nullptr;
	SQLPrepareWFun SQLPrepareW = nullptr;
	SQLPrimaryKeysFun SQLPrimaryKeys = nullptr;
	SQLPrimaryKeysAFun SQLPrimaryKeysA = nullptr;
	SQLPrimaryKeysWFun SQLPrimaryKeysW = nullptr;
	SQLProcedureColumnsFun SQLProcedureColumns = nullptr;
	SQLProcedureColumnsAFun SQLProcedureColumnsA = nullptr;
	SQLProcedureColumnsWFun SQLProcedureColumnsW = nullptr;
	SQLProceduresFun SQLProcedures = nullptr;
	SQLProceduresAFun SQLProceduresA = nullptr;

	SQLProceduresWFun SQLProceduresW = nullptr;
	SQLPutDataFun SQLPutData = nullptr;
	SQLRowCountFun SQLRowCount = nullptr;
	SQLSetConnectAttrFun SQLSetConnectAttr = nullptr;
	SQLSetConnectAttrAFun SQLSetConnectAttrA = nullptr;
	SQLSetConnectAttrWFun SQLSetConnectAttrW = nullptr;
	SQLSetConnectOptionFun SQLSetConnectOption = nullptr;
	SQLSetConnectOptionAFun SQLSetConnectOptionA = nullptr;
	SQLSetConnectOptionWFun SQLSetConnectOptionW = nullptr;
	SQLSetCursorNameFun SQLSetCursorName = nullptr;

	SQLSetCursorNameAFun SQLSetCursorNameA = nullptr;
	SQLSetCursorNameWFun SQLSetCursorNameW = nullptr;
	SQLSetParamFun SQLSetParam = nullptr;
	SQLSetPosFun SQLSetPos = nullptr;
	SQLSetScrollOptionsFun SQLSetScrollOptions = nullptr;
	SQLSetStmtAttrFun SQLSetStmtAttr = nullptr;
	SQLSetStmtAttrAFun SQLSetStmtAttrA = nullptr;
	SQLSetStmtAttrWFun SQLSetStmtAttrW = nullptr;
	SQLSetStmtOptionFun SQLSetStmtOption = nullptr;
	SQLSpecialColumnsFun SQLSpecialColumns = nullptr;

	SQLSpecialColumnsAFun SQLSpecialColumnsA = nullptr;
	SQLSpecialColumnsWFun SQLSpecialColumnsW = nullptr;
	SQLStatisticsFun SQLStatistics = nullptr;
	SQLStatisticsAFun SQLStatisticsA = nullptr;
	SQLStatisticsWFun SQLStatisticsW = nullptr;
	SQLTablePrivilegesFun SQLTablePrivileges = nullptr;
	SQLTablePrivilegesAFun SQLTablePrivilegesA = nullptr;
	SQLTablePrivilegesWFun SQLTablePrivilegesW = nullptr;
	SQLTablesFun SQLTables = nullptr;
	SQLTablesAFun SQLTablesA = nullptr;

	SQLTablesWFun SQLTablesW = nullptr;
	SQLTransactFun SQLTransact = nullptr;

	//19-04-24
	SQLCancelFun SQLCancel = nullptr;
	SQLCopyDescFun SQLCopyDesc = nullptr;
	SQLDataSourcesFun SQLDataSources = nullptr;
	SQLDataSourcesAFun SQLDataSourcesA = nullptr;
	SQLDataSourcesWFun SQLDataSourcesW = nullptr;
	SQLDriversFun SQLDrivers = nullptr;
	SQLDriversAFun SQLDriversA = nullptr;
	SQLDriversWFun SQLDriversW = nullptr;
	SQLFreeEnvFun SQLFreeEnv = nullptr;
	SQLGetDescFieldFun SQLGetDescField = nullptr;

	SQLGetDescFieldAFun SQLGetDescFieldA = nullptr;
	SQLGetDescFieldWFun SQLGetDescFieldW = nullptr;
	SQLGetDescRecFun SQLGetDescRec = nullptr;
	SQLGetDescRecAFun SQLGetDescRecA = nullptr;
	SQLGetDescRecWFun SQLGetDescRecW = nullptr;
	SQLSetDescFieldFun SQLSetDescField = nullptr;
	SQLSetDescFieldAFun SQLSetDescFieldA = nullptr;
	SQLSetDescFieldWFun SQLSetDescFieldW = nullptr;
	SQLSetDescRecFun SQLSetDescRec = nullptr;
	//SQLSetEnvAttrFun SQLSetEnvAttr = nullptr;
	bool m_bInit = false;
};

#endif

