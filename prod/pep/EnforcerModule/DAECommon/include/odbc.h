#ifndef ODBC_H
#define ODBC_H

#ifdef WIN32
#include <windows.h>
#endif

#include <sql.h>

typedef SQLRETURN(SQL_API *SQLAllocHandleFun)(
    SQLSMALLINT		HandleType,
    SQLHANDLE 		HandleParent,
    SQLHANDLE		*NewHandlePointer);

typedef SQLRETURN(SQL_API *SQLSetEnvAttrFun)(
    SQLHENV 		EnvironmentHandle,
    SQLINTEGER 		Attribute,
    SQLPOINTER 		Value,
    SQLINTEGER 		StringLength);

typedef SQLRETURN(SQL_API *SQLDriverConnectFun)(
    SQLHDBC			hdbc,
    SQLHWND			hwnd,
    SQLCHAR			*szConnStrIn,
    SQLSMALLINT		cbConnStrIn,
    SQLCHAR			*szConnStrOut,
    SQLSMALLINT		cbConnStrOutMax,
    SQLSMALLINT		*pcbConnStrOut,
    SQLUSMALLINT	fDriverCompletion);

typedef SQLRETURN(SQL_API *SQLDriverConnectWFun)
(
	SQLHDBC			hdbc,
	SQLHWND			hwnd,
	SQLWCHAR		*szConnStrIn,
	SQLSMALLINT		cchConnStrIn,
	SQLWCHAR		*szConnStrOut,
	SQLSMALLINT		cchConnStrOutMax,
	SQLSMALLINT		*pcchConnStrOut,
	SQLUSMALLINT	fDriverCompletion);

typedef SQLRETURN(SQL_API *SQLDriverConnectAFun)(
    SQLHDBC			hdbc,
    SQLHWND			hwnd,
    SQLCHAR			*szConnStrIn,
    SQLSMALLINT		cbConnStrIn,
    SQLCHAR			*szConnStrOut,
    SQLSMALLINT		cbConnStrOutMax,
    SQLSMALLINT		*pcbConnStrOut,
    SQLUSMALLINT	fDriverCompletion);

typedef SQLRETURN(SQL_API *SQLExecDirectFun)(
	SQLHSTMT		StatementHandle,
	SQLCHAR			*StatementText,
	SQLINTEGER		TextLength);

typedef SQLRETURN(SQL_API *SQLExecDirectAFun)(
    SQLHSTMT		hstmt,
    SQLCHAR			*szSqlStr,
    SQLINTEGER		cbSqlStr);
	
typedef SQLRETURN(SQL_API *SQLExecDirectWFun)(
	SQLHSTMT		hstmt,
	SQLWCHAR		*szSqlStr,
	SQLINTEGER		TextLength);

typedef SQLRETURN(SQL_API *SQLNumResultColsFun)(
	SQLHSTMT		StatementHandle,
	SQLSMALLINT		*ColumnCount);

typedef SQLRETURN(SQL_API *SQLCloseCursorFun)(
	SQLHSTMT		StatementHandle);

typedef SQLRETURN(SQL_API *SQLFreeStmtFun)(
	SQLHSTMT		StatementHandle,
	SQLUSMALLINT	Option);

typedef SQLRETURN(SQL_API *SQLBindColFun)(
	SQLHSTMT		StatementHandle,
	SQLUSMALLINT	ColumnNumber,
	SQLSMALLINT		TargetType,
	SQLPOINTER		TargetValue,
	SQLLEN			BufferLength,
	SQLLEN			*StrLen_or_Ind);

typedef SQLRETURN(SQL_API *SQLColAttributeFun) (
	SQLHSTMT		StatementHandle,
	SQLUSMALLINT	ColumnNumber,
  	SQLUSMALLINT	FieldIdentifier,
  	SQLPOINTER		CharacterAttributePtr,
  	SQLSMALLINT		BufferLength,
  	SQLSMALLINT		*StringLengthPtr,
  	SQLLEN			*NumericAttributePtr);

typedef	SQLRETURN(SQL_API *SQLColAttributeWFun)(
	SQLHSTMT        hstmt,
	SQLUSMALLINT    iCol,
	SQLUSMALLINT    iField,
	SQLPOINTER      pCharAttr,
	SQLSMALLINT     cbDescMax,
	SQLSMALLINT     *pcbCharAttr,
	SQLLEN          *pNumAttr);

typedef SQLRETURN(SQL_API *SQLFetchFun)(
	SQLHSTMT		StatementHandle);

typedef SQLRETURN(SQL_API *SQLGetDataFun)(
	SQLHSTMT		StatementHandle,
	SQLUSMALLINT	Col_or_Param_Num,
	SQLSMALLINT		TargetType,
	SQLPOINTER		TargetValuePtr,
	SQLLEN			BufferLength,
	SQLLEN			*StrLen_or_IndPtr);

typedef SQLRETURN(SQL_API *SQLGetDiagRecFun)(
	SQLSMALLINT     fHandleType,
	SQLHANDLE       handle,
	SQLSMALLINT     iRecord,
	SQLCHAR			*szSqlState,
	SQLINTEGER		*pfNativeError,
	SQLCHAR			*szErrorMsg,
	SQLSMALLINT     cchErrorMsgMax,
	SQLSMALLINT		*pcchErrorMsg);

typedef SQLRETURN(SQL_API *SQLGetDiagRecAFun)(
	SQLSMALLINT     fHandleType,
	SQLHANDLE       handle,
	SQLSMALLINT     iRecord,
	SQLCHAR			*szSqlState,
	SQLINTEGER*     pfNativeError,
	SQLCHAR			*szErrorMsg,
	SQLSMALLINT     cchErrorMsgMax,
	SQLSMALLINT		*pcchErrorMsg);

typedef SQLRETURN(SQL_API *SQLGetDiagRecWFun)(
	SQLSMALLINT     fHandleType,
	SQLHANDLE       handle,
	SQLSMALLINT     iRecord,
	SQLWCHAR		*szSqlState,
	SQLINTEGER		*pfNativeError,
	SQLWCHAR		*szErrorMsg,
	SQLSMALLINT     cchErrorMsgMax,
	SQLSMALLINT		*pcchErrorMsg);

typedef SQLRETURN(SQL_API *SQLConnectFun)(
	SQLHDBC			ConnectionHandle,
	SQLCHAR			*ServerName,
	SQLSMALLINT		NameLength1,
	SQLCHAR			*UserName,
	SQLSMALLINT		NameLength2,
	SQLCHAR			*Authentication,
	SQLSMALLINT		NameLength3);

typedef SQLRETURN(SQL_API *SQLConnectAFun)(
    SQLHDBC         hdbc,
    SQLCHAR         *szDSN,
    SQLSMALLINT     cbDSN,
    SQLCHAR         *szUID,
    SQLSMALLINT     cbUID,
    SQLCHAR         *szAuthStr,
    SQLSMALLINT     cbAuthStr);

typedef SQLRETURN(SQL_API *SQLConnectWFun)(
    SQLHDBC         hdbc,
    SQLWCHAR        *szDSN,
    SQLSMALLINT     cbDSN,
    SQLWCHAR        *szUID,
    SQLSMALLINT     cbUID,
    SQLWCHAR        *szAuthStr,
    SQLSMALLINT     cbAuthStr);

typedef SQLRETURN(SQL_API *SQLBrowseConnectFun)(
    SQLHDBC			hdbc,
    SQLCHAR 		*szConnStrIn,
    SQLSMALLINT     cbConnStrIn,
    SQLCHAR 		*szConnStrOut,
    SQLSMALLINT     cbConnStrOutMax,
    SQLSMALLINT     *pcbConnStrOut);

typedef SQLRETURN(SQL_API *SQLBrowseConnectAFun)(
    SQLHDBC			hdbc,
    SQLCHAR			*szConnStrIn,
    SQLSMALLINT		cbConnStrIn,
    SQLCHAR			*szConnStrOut,
    SQLSMALLINT		cbConnStrOutMax,
    SQLSMALLINT		*pcbConnStrOut);

typedef SQLRETURN(SQL_API *SQLBrowseConnectWFun)(
    SQLHDBC         hdbc,
    SQLWCHAR        *szConnStrIn,
    SQLSMALLINT     cbConnStrIn,
    SQLWCHAR        *szConnStrOut,
    SQLSMALLINT     cbConnStrOutMax,
    SQLSMALLINT    	*pcbConnStrOut);

typedef SQLRETURN(SQL_API *SQLFreeHandleFun)(
    SQLSMALLINT     HandleType,
    SQLHANDLE       Handle);

typedef SQLRETURN(SQL_API *SQLDisconnectFun)(
    SQLHDBC         ConnectionHandle);

typedef SQLRETURN(SQL_API *SQLGetInfoFun)(
    SQLHDBC			ConnectionHandle,
    SQLUSMALLINT 	InfoType,
    SQLPOINTER 		InfoValue,
    SQLSMALLINT 	BufferLength,
    SQLSMALLINT 	*StringLength);

typedef SQLRETURN(SQL_API *SQLCancelHandleFun)(
	SQLSMALLINT		HandleType,
	SQLHANDLE		Handle);

typedef SQLRETURN(SQL_API *SQLCompleteAsyncFun)(
	SQLSMALLINT  	HandleType,
	SQLHANDLE		Handle,
	RETCODE			*AsyncRetCodePtr);

typedef SQLRETURN(SQL_API *SQLGetInfoAFun )(
	SQLHDBC			ConnectionHandle,
	SQLUSMALLINT	InfoType,
	SQLPOINTER		InfoValue,
	SQLSMALLINT		BufferLength,
	SQLSMALLINT		*StringLength);

typedef SQLRETURN(SQL_API * SQLGetInfoWFun )(
	SQLHDBC			ConnectionHandle,
	SQLUSMALLINT	InfoType,
	SQLPOINTER		InfoValue,
	SQLSMALLINT		BufferLength,
	SQLSMALLINT		*StringLength);

//add by jeff 19-04-18
typedef SQLRETURN(SQL_API *SQLAllocConnectFun)(
    SQLHENV			henv,
    SQLHDBC 		*phdbc);

typedef SQLRETURN(SQL_API *SQLAllocEnvFun)(
    SQLHENV 		*phenv);

typedef SQLRETURN(SQL_API *SQLAllocHandleStdFun)(
	SQLSMALLINT		handleType,
	SQLHANDLE		inputHandle,
	SQLHANDLE 		*outputHandlePtr);

typedef SQLRETURN(SQL_API *SQLAllocStmtFun )(
    SQLHDBC			hdbc,
    SQLHSTMT		*phstmt);

typedef SQLRETURN(SQL_API * SQLBindParamFun )(
    SQLHSTMT		hstmt,
    SQLUSMALLINT	ipar,
    SQLSMALLINT		fCType,
    SQLSMALLINT		fSqlType,
    SQLULEN			cbParamDef,
    SQLSMALLINT		ibScale,
    SQLPOINTER		rgbValue,
    SQLLEN			*pcbValue);

typedef SQLRETURN(SQL_API *SQLBindParameterFun )(
    SQLHSTMT		hstmt,
    SQLUSMALLINT	ipar,
    SQLSMALLINT		fParamType,
    SQLSMALLINT		fCType,
    SQLSMALLINT		fSqlType,
    SQLULEN		  	cbColDef,
    SQLSMALLINT		ibScale,
    SQLPOINTER		rgbValue,
    SQLLEN		  	cbValueMax,
    SQLLEN			*pcbValue);

typedef SQLRETURN(SQL_API *SQLBulkOperationsFun )(
	SQLHSTMT		StatementHandle,
	SQLSMALLINT 	Operation);

typedef SQLRETURN(SQL_API * SQLCancelFun )(
	SQLHSTMT		hstmt);

typedef SQLRETURN(SQL_API * SQLColAttributeAFun )(
	SQLHSTMT		statementHandle,
	SQLUSMALLINT	ColumnNumber,
	SQLUSMALLINT	FieldIdentifier,
	SQLPOINTER		CharacterAttributePtr,
	SQLSMALLINT		BufferLength,
	SQLSMALLINT		*StringLengthPtr,
	SQLLEN			*NumericAttributePtr);

typedef SQLRETURN(SQL_API * SQLColAttributesFun )(
	SQLHSTMT 		statementHandle,
	SQLUSMALLINT	icol,
	SQLUSMALLINT	fDescType,
	SQLPOINTER		rgbDesc,
	SQLSMALLINT		cbDescMax,
	SQLSMALLINT 	*pcbDesc,
	SQLLEN 			*pfDesc);

typedef SQLRETURN(SQL_API * SQLColAttributesAFun )(
	SQLHSTMT		statementHandle,
	SQLUSMALLINT	icol,
	SQLUSMALLINT	fDescType,
	SQLPOINTER		rgbDesc,
	SQLSMALLINT		cbDescMax,
	SQLSMALLINT		*pcbDesc,
	SQLLEN			*pfDesc);

typedef SQLRETURN(SQL_API * SQLColAttributesWFun )(
	SQLHSTMT		statementHandle,
	SQLUSMALLINT	icol,
	SQLUSMALLINT	fDescType,
	SQLPOINTER		rgbDesc,
	SQLSMALLINT		cbDescMax,
	SQLSMALLINT 	*pcbDesc,
	SQLLEN 			*pfDesc);

typedef SQLRETURN(SQL_API * SQLColumnPrivilegesFun )(
	SQLHSTMT		hstmt,
    SQLCHAR 		*szTableQualifier,
    SQLSMALLINT		cbTableQualifier,
    SQLCHAR			*szTableOwner,
    SQLSMALLINT		cbTableOwner,
    SQLCHAR			*szTableName,
    SQLSMALLINT		cbTableName,
    SQLCHAR			*szColumnName,
    SQLSMALLINT		cbColumnName);

typedef SQLRETURN(SQL_API * SQLColumnPrivilegesAFun )(
	SQLHSTMT 		hstmt,
    SQLCHAR 		*szTableQualifier,
    SQLSMALLINT		cbTableQualifier,
    SQLCHAR 		*szTableOwner,
    SQLSMALLINT		cbTableOwner,
    SQLCHAR 		*szTableName,
    SQLSMALLINT		cbTableName,
    SQLCHAR 		*szColumnName,
    SQLSMALLINT		cbColumnName);

typedef SQLRETURN(SQL_API * SQLColumnPrivilegesWFun )(
	SQLHSTMT		hstmt,
    SQLWCHAR		*szTableQualifier,
    SQLSMALLINT		cbTableQualifier,
    SQLWCHAR		*szTableOwner,
    SQLSMALLINT		cbTableOwner,
    SQLWCHAR		*szTableName,
    SQLSMALLINT		cbTableName,
    SQLWCHAR		*szColumnName,
    SQLSMALLINT 	cbColumnName);

typedef SQLRETURN(SQL_API * SQLColumnsFun )(
	SQLHSTMT		hstmt,
    SQLCHAR			*szTableQualifier,
    SQLSMALLINT		cbTableQualifier,
    SQLCHAR 		*szTableOwner,
    SQLSMALLINT		cbTableOwner,
    SQLCHAR 		*szTableName,
    SQLSMALLINT		cbTableName,
    SQLCHAR 		*szColumnName,
    SQLSMALLINT		cbColumnName);

typedef SQLRETURN(SQL_API * SQLColumnsAFun )(
	SQLHSTMT		hstmt,
    SQLCHAR			*szTableQualifier,
    SQLSMALLINT		cbTableQualifier,
    SQLCHAR			*szTableOwner,
    SQLSMALLINT		cbTableOwner,
    SQLCHAR			*szTableName,
    SQLSMALLINT		cbTableName,
    SQLCHAR			*szColumnName,
    SQLSMALLINT		cbColumnName);

typedef SQLRETURN(SQL_API * SQLColumnsWFun )(
	SQLHSTMT 		hstmt,
    SQLWCHAR 		*szTableQualifier,
    SQLSMALLINT		cbTableQualifier,
    SQLWCHAR 		*szTableOwner,
    SQLSMALLINT		cbTableOwner,
    SQLWCHAR 		*szTableName,
    SQLSMALLINT		cbTableName,
    SQLWCHAR 		*szColumnName,
    SQLSMALLINT		cbColumnName);

typedef SQLRETURN(SQL_API * SQLDescribeColFun )(
	SQLHSTMT 		hstmt,
    SQLUSMALLINT	icol,
    SQLCHAR			*szColName,
    SQLSMALLINT		cbColNameMax,
    SQLSMALLINT		*pcbColName,
    SQLSMALLINT		*pfSqlType,
    SQLULEN			*pcbColDef,
    SQLSMALLINT		*pibScale,
    SQLSMALLINT		*pfNullable);

typedef SQLRETURN(SQL_API * SQLDescribeColAFun )(
	SQLHSTMT 		hstmt,
    SQLUSMALLINT	icol,
    SQLCHAR			*szColName,
    SQLSMALLINT		cbColNameMax,
    SQLSMALLINT		*pcbColName,
    SQLSMALLINT		*pfSqlType,
    SQLULEN			*pcbColDef,
    SQLSMALLINT		*pibScale,
    SQLSMALLINT		*pfNullable);

typedef SQLRETURN(SQL_API * SQLDescribeColWFun )(
	SQLHSTMT		hstmt,
	SQLUSMALLINT	icol,
	SQLWCHAR		*szColName,
	SQLSMALLINT		cbColNameMax,
	SQLSMALLINT		*pcbColName,
	SQLSMALLINT		*pfSqlType,
	SQLULEN			*pcbColDef,
	SQLSMALLINT		*pibScale,
	SQLSMALLINT		*pfNullable);

typedef SQLRETURN(SQL_API * SQLDescribeParamFun )(
	SQLHSTMT		hstmt,
	SQLUSMALLINT	ipar,
	SQLSMALLINT 	*pfSqlType,
	SQLULEN 		*pcbColDef,
	SQLSMALLINT 	*pibScale,
	SQLSMALLINT 	*pfNullable);

typedef SQLRETURN(SQL_API * SQLEndTranFun )(
	SQLSMALLINT		handleType,
	SQLHANDLE		Handle,
	SQLSMALLINT		completionType);

typedef SQLRETURN(SQL_API * SQLErrorFun )(
	SQLHENV		  	henv,
	SQLHDBC		  	hdbc,
	SQLHSTMT		hstmt,
	SQLCHAR 		*szSqlstate,
	SQLINTEGER 		*pfNativeError,
	SQLCHAR 		*szErrorMsg,
	SQLSMALLINT		cbErrorMsgMax,
  	SQLSMALLINT 	*pcbErrorMsg);

typedef SQLRETURN(SQL_API * SQLErrorAFun )(
	SQLHENV		  	henv,
	SQLHDBC		  	hdbc,
	SQLHSTMT		hstmt,
	SQLCHAR 		*szSqlstate,
	SQLINTEGER 		*pfNativeError,
	SQLCHAR 		*szErrorMsg,
	SQLSMALLINT		cbErrorMsgMax,
	SQLSMALLINT 	*pcbErrorMsg);

typedef SQLRETURN(SQL_API * SQLErrorWFun )(
	SQLHENV		  	henv,
	SQLHDBC		  	hdbc,
	SQLHSTMT		hstmt,
	SQLWCHAR 		*szSqlstate,
	SQLINTEGER 		*pfNativeError,
	SQLWCHAR 		*szErrorMsg,
	SQLSMALLINT		cbErrorMsgMax,
	SQLSMALLINT 	*pcbErrorMsg);

typedef SQLRETURN(SQL_API * SQLExecuteFun )(
	SQLHSTMT hstmt);

typedef SQLRETURN(SQL_API * SQLExtendedFetchFun )(
	SQLHSTMT		hstmt,
	SQLUSMALLINT	fFetchType,
	SQLLEN			irow,
	SQLULEN 		*pcrow,
	SQLUSMALLINT 	*rgfRowStatus);

typedef SQLRETURN(SQL_API * SQLFetchScrollFun )(
	SQLHSTMT		StatementHandle,
	SQLSMALLINT		FetchOrientation,
	SQLLEN		  	FetchOffset);

typedef SQLRETURN(SQL_API * SQLForeignKeysFun )(
	SQLHSTMT 		hstmt,
    SQLCHAR 		*szPkTableQualifier,
    SQLSMALLINT 	cbPkTableQualifier,
    SQLCHAR 		*szPkTableOwner,
    SQLSMALLINT 	cbPkTableOwner,
    SQLCHAR 		*szPkTableName,
    SQLSMALLINT 	cbPkTableName,
    SQLCHAR 		*szFkTableQualifier,
    SQLSMALLINT 	cbFkTableQualifier,
    SQLCHAR 		*szFkTableOwner,
    SQLSMALLINT		cbFkTableOwner,
    SQLCHAR 		*szFkTableName,
    SQLSMALLINT		cbFkTableName);

typedef SQLRETURN(SQL_API * SQLForeignKeysAFun )(
	SQLHSTMT		hstmt,
    SQLCHAR			*szPkTableQualifier,
    SQLSMALLINT		cbPkTableQualifier,
    SQLCHAR			*szPkTableOwner,
    SQLSMALLINT		cbPkTableOwner,
    SQLCHAR			*szPkTableName,
    SQLSMALLINT		cbPkTableName,
    SQLCHAR			*szFkTableQualifier,
    SQLSMALLINT		cbFkTableQualifier,
    SQLCHAR			*szFkTableOwner,
    SQLSMALLINT		cbFkTableOwner,
    SQLCHAR			*szFkTableName,
    SQLSMALLINT		cbFkTableName);

typedef SQLRETURN(SQL_API * SQLForeignKeysWFun )(
	SQLHSTMT		hstmt,
    SQLWCHAR		*szPkTableQualifier,
    SQLSMALLINT		cbPkTableQualifier,
    SQLWCHAR		*szPkTableOwner,
    SQLSMALLINT		cbPkTableOwner,
    SQLWCHAR		*szPkTableName,
    SQLSMALLINT		cbPkTableName,
    SQLWCHAR		*szFkTableQualifier,
    SQLSMALLINT		cbFkTableQualifier,
    SQLWCHAR		*szFkTableOwner,
    SQLSMALLINT		cbFkTableOwner,
    SQLWCHAR		*szFkTableName,
    SQLSMALLINT		cbFkTableName);

typedef SQLRETURN(SQL_API * SQLFreeConnectFun )(
	SQLHDBC			hdbc);

typedef SQLRETURN(SQL_API * SQLGetConnectAttrFun )(
	SQLHDBC 		connectionHandle,
	SQLINTEGER 		Attribute,
	SQLPOINTER 		ValuePtr,
	SQLINTEGER 		StringLength,
	SQLINTEGER		*StringLengthPtr);

typedef SQLRETURN(SQL_API * SQLGetConnectAttrAFun )(
	SQLHDBC 		connectionHandle,
	SQLINTEGER 		Attribute,
	SQLPOINTER 		ValuePtr,
	SQLINTEGER 		StringLength,
	SQLINTEGER		*StringLengthPtr);

typedef SQLRETURN(SQL_API * SQLGetConnectAttrWFun )(
	SQLHDBC 		connectionHandle,
	SQLINTEGER 		Attribute,
	SQLPOINTER 		ValuePtr,
	SQLINTEGER 		StringLength,
	SQLINTEGER		*StringLengthPtr);

typedef SQLRETURN(SQL_API * SQLGetConnectOptionFun )(
	SQLHDBC hdbc,
    SQLUSMALLINT 	fOption,
    SQLPOINTER 		pvParam);

typedef SQLRETURN(SQL_API * SQLGetConnectOptionAFun )(
	SQLHDBC hdbc,
    SQLUSMALLINT 	fOption,
    SQLPOINTER 		pvParam);

typedef SQLRETURN(SQL_API * SQLGetConnectOptionWFun )(
	SQLHDBC hdbc,
    SQLUSMALLINT 	fOption,
    SQLPOINTER 	pvParam);

typedef SQLRETURN(SQL_API * SQLGetCursorNameFun )(
	SQLHSTMT 		hstmt,
	SQLCHAR  		*szCursor,
	SQLSMALLINT 	cbCursorMax,
	SQLSMALLINT 	*pcbCursor);

typedef SQLRETURN(SQL_API * SQLGetCursorNameAFun )(
	SQLHSTMT 		hstmt,
	SQLCHAR  		*szCursor,
	SQLSMALLINT 	cbCursorMax,
	SQLSMALLINT 	*pcbCursor);

typedef SQLRETURN(SQL_API * SQLGetCursorNameWFun )(
	SQLHSTMT 		hstmt,
	SQLWCHAR  		*szCursor,
	SQLSMALLINT 	cbCursorMax,
	SQLSMALLINT 	*pcbCursor);

typedef SQLRETURN(SQL_API * SQLGetDiagFieldFun )(
	SQLSMALLINT		HandleType,
	SQLHANDLE		Handle,
	SQLSMALLINT		RecNumber,
	SQLSMALLINT		DiagIdentifier,
	SQLPOINTER		DiagInfoPtr,
	SQLSMALLINT		BufferLength,
	SQLSMALLINT		*StringLengthPtr);

typedef SQLRETURN(SQL_API * SQLGetDiagFieldAFun )(
	SQLSMALLINT		HandleType,
	SQLHANDLE		Handle,
	SQLSMALLINT		RecNumber,
	SQLSMALLINT		DiagIdentifier,
	SQLPOINTER		DiagInfoPtr,
	SQLSMALLINT		BufferLength,
	SQLSMALLINT		*StringLengthPtr);

typedef SQLRETURN(SQL_API * SQLGetDiagFieldWFun )(
	SQLSMALLINT		HandleType,
	SQLHANDLE		Handle,
	SQLSMALLINT		RecNumber,
	SQLSMALLINT		DiagIdentifier,
	SQLPOINTER		DiagInfoPtr,
	SQLSMALLINT		BufferLength,
	SQLSMALLINT		*StringLengthPtr);

typedef SQLRETURN(SQL_API * SQLGetEnvAttrFun )(
	SQLHENV		  	EnvironmentHandle,
	SQLINTEGER		Attribute,
	SQLPOINTER		ValuePtr,
	SQLINTEGER		BufferLength,
	SQLINTEGER		*StringLengthPtr);

typedef SQLRETURN(SQL_API * SQLGetFunctionsFun )(
	SQLHDBC		  	hdbc,
	SQLUSMALLINT	fFunc,
	SQLUSMALLINT	*pfExists);

typedef SQLRETURN(SQL_API * SQLGetStmtAttrFun )(
	SQLHSTMT		statementHandle,
	SQLINTEGER		Attribute,
	SQLPOINTER		ValuePtr,
	SQLINTEGER		BufferLength,
	SQLINTEGER		*StringLengthPtr);

typedef SQLRETURN(SQL_API * SQLGetStmtAttrAFun )(
	SQLHSTMT		statementHandle,
	SQLINTEGER		Attribute,
	SQLPOINTER		ValuePtr,
	SQLINTEGER		BufferLength,
	SQLINTEGER		*StringLengthPtr);

typedef SQLRETURN(SQL_API * SQLGetStmtAttrWFun )(
	SQLHSTMT		statementHandle,
	SQLINTEGER		Attribute,
	SQLPOINTER		ValuePtr,
	SQLINTEGER		BufferLength,
	SQLINTEGER		*StringLengthPtr);

typedef SQLRETURN(SQL_API *SQLGetStmtOptionFun )(
    SQLHSTMT		hstmt,
    SQLUSMALLINT 	fOption,
    SQLPOINTER 		pvParam);

typedef SQLRETURN(SQL_API * SQLGetTypeInfoFun )(
	SQLHSTMT 		hstmt,
    SQLSMALLINT 	fSqlType);

typedef SQLRETURN(SQL_API * SQLGetTypeInfoAFun )(
	SQLHSTMT 		hstmt,
    SQLSMALLINT 	fSqlType);

typedef SQLRETURN(SQL_API * SQLGetTypeInfoWFun )(
	SQLHSTMT 		hstmt,
    SQLSMALLINT 	fSqlType);

typedef SQLRETURN(SQL_API * SQLMoreResultsFun )(
	SQLHSTMT 		hstmt);

typedef SQLRETURN(SQL_API * SQLNativeSqlFun )(
	SQLHDBC 		hdbc,
    SQLCHAR 		*szSqlStrIn,
    SQLINTEGER 		cbSqlStrIn,
    SQLCHAR 		*szSqlStr,
    SQLINTEGER 		cbSqlStrMax,
    SQLINTEGER 		*pcbSqlStr);

typedef SQLRETURN(SQL_API * SQLNativeSqlAFun )(
	SQLHDBC			hdbc,
    SQLCHAR			*szSqlStrIn,
    SQLINTEGER		cbSqlStrIn,
    SQLCHAR			*szSqlStr,
    SQLINTEGER		cbSqlStrMax,
    SQLINTEGER		*pcbSqlStr);

typedef SQLRETURN(SQL_API * SQLNativeSqlWFun )(
	SQLHDBC 		hdbc,
    SQLWCHAR		*szSqlStrIn,
    SQLINTEGER		cbSqlStrIn,
    SQLWCHAR		*szSqlStr,
    SQLINTEGER		cbSqlStrMax,
    SQLINTEGER		*pcbSqlStr);

typedef SQLRETURN(SQL_API * SQLNumParamsFun )(
	SQLHSTMT		hstmt,
	SQLSMALLINT		*pcpar);

typedef SQLRETURN(SQL_API * SQLParamDataFun )(
	SQLHSTMT 		hstmt,
	SQLPOINTER 		*prgbValue);

typedef SQLRETURN(SQL_API * SQLParamOptionsFun )(
	SQLHSTMT		hstmt,
	SQLULEN		  	crow,
	SQLULEN 		*pirow);

typedef SQLRETURN(SQL_API * SQLPrepareFun )(
	SQLHSTMT 		hstmt,
    SQLCHAR 		*szSqlStr,
    SQLINTEGER 		cbSqlStr);

typedef SQLRETURN(SQL_API * SQLPrepareAFun )(
	SQLHSTMT 		hstmt,
    SQLCHAR 		*szSqlStr,
    SQLINTEGER 		cbSqlStr);

typedef SQLRETURN(SQL_API * SQLPrepareWFun )(
	SQLHSTMT 		hstmt,
    SQLWCHAR 		*szSqlStr,
    SQLINTEGER 		cbSqlStr);

typedef SQLRETURN(SQL_API * SQLPrimaryKeysFun )(
	SQLHSTMT		hstmt,
    SQLCHAR 		*szTableQualifier,
    SQLSMALLINT		cbTableQualifier,
    SQLCHAR 		*szTableOwner,
    SQLSMALLINT		cbTableOwner,
    SQLCHAR 		*szTableName,
    SQLSMALLINT		cbTableName);

typedef SQLRETURN(SQL_API * SQLPrimaryKeysAFun )(
	SQLHSTMT		hstmt,
    SQLCHAR			*szTableQualifier,
    SQLSMALLINT		cbTableQualifier,
    SQLCHAR			*szTableOwner,
    SQLSMALLINT		cbTableOwner,
    SQLCHAR			*szTableName,
    SQLSMALLINT		cbTableName);

typedef SQLRETURN(SQL_API * SQLPrimaryKeysWFun )(
	SQLHSTMT		hstmt,
    SQLWCHAR		*szTableQualifier,
    SQLSMALLINT		cbTableQualifier,
    SQLWCHAR		*szTableOwner,
    SQLSMALLINT		cbTableOwner,
    SQLWCHAR 		*szTableName,
    SQLSMALLINT		cbTableName);

typedef SQLRETURN(SQL_API * SQLProcedureColumnsFun )(
	SQLHSTMT		hstmt,
    SQLCHAR			*szProcQualifier,
    SQLSMALLINT		cbProcQualifier,
    SQLCHAR			*szProcOwner,
    SQLSMALLINT		cbProcOwner,
    SQLCHAR			*szProcName,
    SQLSMALLINT		cbProcName,
    SQLCHAR			*szColumnName,
    SQLSMALLINT		cbColumnName);

typedef SQLRETURN(SQL_API * SQLProcedureColumnsAFun )(
	SQLHSTMT 		hstmt,
    SQLCHAR 		*szProcQualifier,
    SQLSMALLINT 	cbProcQualifier,
    SQLCHAR 		*szProcOwner,
    SQLSMALLINT 	cbProcOwner,
    SQLCHAR 		*szProcName,
    SQLSMALLINT		cbProcName,
    SQLCHAR 		*szColumnName,
    SQLSMALLINT 	cbColumnName);

typedef SQLRETURN(SQL_API * SQLProcedureColumnsWFun )(
	SQLHSTMT 		hstmt,
    SQLWCHAR 		*szProcQualifier,
    SQLSMALLINT 	cbProcQualifier,
    SQLWCHAR 		*szProcOwner,
    SQLSMALLINT		cbProcOwner,
    SQLWCHAR 		*szProcName,
    SQLSMALLINT		cbProcName,
    SQLWCHAR 		*szColumnName,
    SQLSMALLINT 	cbColumnName);

typedef SQLRETURN(SQL_API * SQLProceduresFun )(
	SQLHSTMT		hstmt,
    SQLCHAR 		*szProcQualifier,
    SQLSMALLINT		cbProcQualifier,
    SQLCHAR 		*szProcOwner,
    SQLSMALLINT		cbProcOwner,
    SQLCHAR 		*szProcName,
    SQLSMALLINT 	cbProcName);

typedef SQLRETURN(SQL_API * SQLProceduresAFun )(
	SQLHSTMT		hstmt,
    SQLCHAR 		*szProcQualifier,
    SQLSMALLINT 	cbProcQualifier,
    SQLCHAR 		*szProcOwner,
    SQLSMALLINT 	cbProcOwner,
    SQLCHAR 		*szProcName,
    SQLSMALLINT		cbProcName);

typedef SQLRETURN(SQL_API * SQLProceduresWFun )(
	SQLHSTMT		hstmt,
    SQLWCHAR		*szProcQualifier,
    SQLSMALLINT		cbProcQualifier,
    SQLWCHAR		*szProcOwner,
    SQLSMALLINT		cbProcOwner,
    SQLWCHAR		*szProcName,
    SQLSMALLINT		cbProcName);

typedef SQLRETURN(SQL_API * SQLPutDataFun )(
	SQLHSTMT		hstmt,
	SQLPOINTER		rgbValue,
	SQLLEN		  	cbValue);

typedef SQLRETURN(SQL_API * SQLRowCountFun )(
	SQLHSTMT		hstmt,
	SQLLEN 			*pcrow);

typedef SQLRETURN(SQL_API * SQLSetConnectAttrFun )(
	SQLHDBC		  	connectionHandle,
	SQLINTEGER		Attribute,
	SQLPOINTER		ValuePtr,
	SQLINTEGER		StringLength);

typedef SQLRETURN(SQL_API * SQLSetConnectAttrAFun )(
	SQLHDBC		  	connectionHandle,
	SQLINTEGER		Attribute,
	SQLPOINTER		ValuePtr,
	SQLINTEGER		StringLength);

typedef SQLRETURN(SQL_API * SQLSetConnectAttrWFun )(
	SQLHDBC		  	connectionHandle,
	SQLINTEGER		Attribute,
	SQLPOINTER		ValuePtr,
	SQLINTEGER		StringLength);

typedef SQLRETURN(SQL_API * SQLSetConnectOptionFun )(
	SQLHDBC		  	hdbc,
	SQLUSMALLINT	fOption,
	SQLULEN		  	vParam);

typedef SQLRETURN(SQL_API * SQLSetConnectOptionAFun )(
	SQLHDBC		  	hdbc,
	SQLUSMALLINT	fOption,
	SQLULEN		  	vParam);

typedef SQLRETURN(SQL_API * SQLSetConnectOptionWFun )(
	SQLHDBC		  	hdbc,
	SQLUSMALLINT	fOption,
	SQLULEN		  	vParam);

typedef SQLRETURN(SQL_API * SQLSetCursorNameFun )(
    SQLHSTMT		hstmt,
    SQLCHAR			*szCursor,
    SQLSMALLINT		cbCursor);

typedef SQLRETURN(SQL_API * SQLSetCursorNameAFun )(
    SQLHSTMT		hstmt,
    SQLCHAR			*szCursor,
    SQLSMALLINT		cbCursor);

typedef SQLRETURN(SQL_API * SQLSetCursorNameWFun )(
    SQLHSTMT		hstmt,
    SQLWCHAR		*szCursor,
    SQLSMALLINT		cbCursor);

typedef SQLRETURN(SQL_API * SQLSetParamFun )(
	SQLHSTMT		hstmt,
	SQLUSMALLINT	ipar,
	SQLSMALLINT		fCType,
	SQLSMALLINT		fSqlType,
	SQLULEN		  	cbColDef,
	SQLSMALLINT		ibScale,
	SQLPOINTER		rgbValue,
	SQLLEN 			*pcbValue);

typedef SQLRETURN(SQL_API * SQLSetPosFun )(
	SQLHSTMT		hstmt,
	SQLSETPOSIROW	irow,
	SQLUSMALLINT	fOption,
	SQLUSMALLINT	fLock);

typedef SQLRETURN(SQL_API * SQLSetScrollOptionsFun )(
	SQLHSTMT		hstmt,
	SQLUSMALLINT	fConcurrency,
	SQLLEN		  	crowKeyset,
	SQLUSMALLINT	crowRowset);

typedef SQLRETURN(SQL_API * SQLSetStmtAttrFun )(
  	SQLHSTMT 		statementHandle,
	SQLINTEGER		Attribute,
	SQLPOINTER		ValuePtr,
	SQLINTEGER		StringLength);

typedef SQLRETURN(SQL_API * SQLSetStmtAttrAFun )(
	SQLHSTMT 		statementHandle,
	SQLINTEGER		Attribute,
	SQLPOINTER		ValuePtr,
	SQLINTEGER		StringLength);

typedef SQLRETURN(SQL_API * SQLSetStmtAttrWFun )(
	SQLHSTMT 		statementHandle,
	SQLINTEGER		Attribute,
	SQLPOINTER		ValuePtr,
	SQLINTEGER		StringLength);

typedef SQLRETURN(SQL_API * SQLSetStmtOptionFun )(
	SQLHSTMT		hstmt,
	SQLUSMALLINT	fOption,
	SQLULEN		  	vParam);

typedef SQLRETURN(SQL_API * SQLSpecialColumnsFun )(
	SQLHSTMT 		hstmt,
    SQLUSMALLINT 	fColType,
    SQLCHAR 		*szTableQualifier,
    SQLSMALLINT 	cbTableQualifier,
    SQLCHAR 		*szTableOwner,
    SQLSMALLINT 	cbTableOwner,
    SQLCHAR 		*szTableName,
    SQLSMALLINT 	cbTableName,
    SQLUSMALLINT 	fScope,
    SQLUSMALLINT 	fNullable);

typedef SQLRETURN(SQL_API * SQLSpecialColumnsAFun )(
	SQLHSTMT 		hstmt,
    SQLUSMALLINT	fColType,
    SQLCHAR			*szTableQualifier,
    SQLSMALLINT		cbTableQualifier,
    SQLCHAR			*szTableOwner,
    SQLSMALLINT		cbTableOwner,
    SQLCHAR			*szTableName,
    SQLSMALLINT		cbTableName,
    SQLUSMALLINT	fScope,
    SQLUSMALLINT	fNullable);

typedef SQLRETURN(SQL_API * SQLSpecialColumnsWFun )(
	SQLHSTMT		hstmt,
    SQLUSMALLINT	fColType,
    SQLWCHAR 		*szTableQualifier,
    SQLSMALLINT		cbTableQualifier,
    SQLWCHAR 		*szTableOwner,
    SQLSMALLINT		cbTableOwner,
    SQLWCHAR 		*szTableName,
    SQLSMALLINT 	cbTableName,
    SQLUSMALLINT 	fScope,
    SQLUSMALLINT 	fNullable);

typedef SQLRETURN(SQL_API * SQLStatisticsFun )(
	SQLHSTMT		hstmt,
    SQLCHAR 		*szTableQualifier,
    SQLSMALLINT 	cbTableQualifier,
    SQLCHAR 		*szTableOwner,
    SQLSMALLINT 	cbTableOwner,
    SQLCHAR 		*szTableName,
    SQLSMALLINT 	cbTableName,
    SQLUSMALLINT 	fUnique,
    SQLUSMALLINT 	fAccuracy);

typedef SQLRETURN(SQL_API * SQLStatisticsAFun )(
	SQLHSTMT		hstmt,
    SQLCHAR 		*szTableQualifier,
    SQLSMALLINT 	cbTableQualifier,
    SQLCHAR 		*szTableOwner,
    SQLSMALLINT 	cbTableOwner,
    SQLCHAR 		*szTableName,
    SQLSMALLINT 	cbTableName,
    SQLUSMALLINT 	fUnique,
    SQLUSMALLINT 	fAccuracy);

typedef SQLRETURN(SQL_API * SQLStatisticsWFun )(
	SQLHSTMT 		hstmt,
    SQLWCHAR 		*szTableQualifier,
    SQLSMALLINT 	cbTableQualifier,
    SQLWCHAR 		*szTableOwner,
    SQLSMALLINT 	cbTableOwner,
    SQLWCHAR 		*szTableName,
    SQLSMALLINT 	cbTableName,
    SQLUSMALLINT 	fUnique,
    SQLUSMALLINT 	fAccuracy);

typedef SQLRETURN(SQL_API * SQLTablePrivilegesFun )(
	SQLHSTMT 		hstmt,
    SQLCHAR 		*szTableQualifier,
    SQLSMALLINT 	cbTableQualifier,
    SQLCHAR 		*szTableOwner,
    SQLSMALLINT		cbTableOwner,
    SQLCHAR			*szTableName,
    SQLSMALLINT		cbTableName);

typedef SQLRETURN(SQL_API * SQLTablePrivilegesAFun )(
	SQLHSTMT 		hstmt,
    SQLCHAR 		*szTableQualifier,
    SQLSMALLINT 	cbTableQualifier,
    SQLCHAR 		*szTableOwner,
    SQLSMALLINT 	cbTableOwner,
    SQLCHAR 		*szTableName,
    SQLSMALLINT 	cbTableName);

typedef SQLRETURN(SQL_API * SQLTablePrivilegesWFun )(
	SQLHSTMT 		hstmt,
    SQLWCHAR 		*szTableQualifier,
    SQLSMALLINT 	cbTableQualifier,
    SQLWCHAR 		*szTableOwner,
    SQLSMALLINT 	cbTableOwner,
    SQLWCHAR 		*szTableName,
    SQLSMALLINT 	cbTableName);

typedef SQLRETURN(SQL_API * SQLTablesFun )(
	SQLHSTMT 		hstmt,
    SQLCHAR 		*szTableQualifier,
    SQLSMALLINT 	cbTableQualifier,
    SQLCHAR 		*szTableOwner,
    SQLSMALLINT 	cbTableOwner,
    SQLCHAR 		*szTableName,
    SQLSMALLINT 	cbTableName,
    SQLCHAR 		*szTableType,
    SQLSMALLINT 	cbTableType);

typedef SQLRETURN(SQL_API * SQLTablesAFun )(
	SQLHSTMT 		hstmt,
    SQLCHAR 		*szTableQualifier,
    SQLSMALLINT 	cbTableQualifier,
    SQLCHAR 		*szTableOwner,
    SQLSMALLINT		cbTableOwner,
    SQLCHAR 		*szTableName,
    SQLSMALLINT		cbTableName,
    SQLCHAR 		*szTableType,
    SQLSMALLINT		cbTableType);

typedef SQLRETURN(SQL_API * SQLTablesWFun )(
	SQLHSTMT		hstmt,
    SQLWCHAR		*szTableQualifier,
    SQLSMALLINT		cbTableQualifier,
    SQLWCHAR 		*szTableOwner,
    SQLSMALLINT		cbTableOwner,
    SQLWCHAR		*szTableName,
    SQLSMALLINT		cbTableName,
    SQLWCHAR		*szTableType,
    SQLSMALLINT		cbTableType);

typedef SQLRETURN(SQL_API * SQLTransactFun )(
	SQLHENV		  	henv,
	SQLHDBC		  	hdbc,
	SQLUSMALLINT	fType);

//add by jeff 19-04-24
// typedef SQLRETURN (SQL_API* 
// SQLCancelFun )(SQLHSTMT hstmt);
// typedef SQLRETURN (SQL_API* 
// SQLCancelFun )(SQLHSTMT hstmt);
typedef SQLRETURN(SQL_API* SQLCancelFun )(
	SQLHSTMT 		hstmt);

typedef SQLRETURN(SQL_API* SQLCopyDescFun) (
	SQLHDESC		SourceDescHandle,
	SQLHDESC		TargetDescHandle);

typedef SQLRETURN(SQL_API* SQLDataSourcesFun) (
	SQLHENV		  	henv,
	SQLUSMALLINT	fDir,
	SQLCHAR 		*szDSN,
	SQLSMALLINT		cbDSNMax,
	SQLSMALLINT 	*pcbDSN,
	SQLCHAR 		*szDesc,
	SQLSMALLINT		cbDescMax,
	SQLSMALLINT 	*pcbDesc);

typedef SQLRETURN(SQL_API* SQLDataSourcesAFun) (
	SQLHENV		  	henv,
	SQLUSMALLINT	fDir,
  	SQLCHAR 		*szDSN,
  	SQLSMALLINT		cbDSNMax,
  	SQLSMALLINT 	*pcbDSN,
  	SQLCHAR 		*szDesc,
  	SQLSMALLINT		cbDescMax,
	SQLSMALLINT		*pcbDesc);

typedef SQLRETURN(SQL_API* SQLDataSourcesWFun) (
	SQLHENV		  	henv,
	SQLUSMALLINT	fDir,
	SQLWCHAR 		*szDSN,
	SQLSMALLINT		cbDSNMax,
	SQLSMALLINT 	*pcbDSN,
	SQLWCHAR 		*szDesc,
	SQLSMALLINT		cbDescMax,
	SQLSMALLINT 	*pcbDesc);

typedef SQLRETURN (SQL_API* SQLDriversFun) (
	SQLHENV		  	henv,
	SQLUSMALLINT	fDir,
	SQLCHAR 		*szDrvDesc,
	SQLSMALLINT		cbDrvDescMax,
	SQLSMALLINT 	*pcbDrvDesc,
	SQLCHAR 		*szDrvAttr,
	SQLSMALLINT		cbDrvAttrMax,
	SQLSMALLINT 	*pcbDrvAttr);

typedef SQLRETURN (SQL_API* SQLDriversAFun )(
	SQLHENV		  	henv,
	SQLUSMALLINT	fDir,
	SQLCHAR  		*szDrvDesc,
	SQLSMALLINT		cbDrvDescMax,
	SQLSMALLINT 	*pcbDrvDesc,
	SQLCHAR  		*szDrvAttr,
	SQLSMALLINT		cbDrvAttrMax,
	SQLSMALLINT 	*pcbDrvAttr);

typedef SQLRETURN(SQL_API* SQLDriversWFun) (
	SQLHENV 		henv,
    SQLUSMALLINT 	fDir,
    SQLWCHAR		*szDrvDesc,
    SQLSMALLINT		cbDrvDescMax,
    SQLSMALLINT		*pcbDrvDesc,
    SQLWCHAR		*szDrvAttr,
    SQLSMALLINT		cbDrvAttrMax,
    SQLSMALLINT		*pcbDrvAttr);

typedef SQLRETURN (SQL_API* SQLFreeEnvFun) (
	SQLHENV 		henv);

typedef SQLRETURN (SQL_API* SQLGetDescFieldFun) (
	SQLHDESC		descriptorHandle,
	SQLSMALLINT		RecNumber,
	SQLSMALLINT		FieldIdentifier,
	SQLPOINTER		ValuePtr,
	SQLINTEGER		BufferLength,
	SQLINTEGER		*StringLengthPtr);

typedef SQLRETURN (SQL_API* SQLGetDescFieldAFun )(
	SQLHDESC		descriptorHandle,
	SQLSMALLINT		RecNumber,
	SQLSMALLINT		FieldIdentifier,
	SQLPOINTER		ValuePtr,
	SQLINTEGER		BufferLength,
	SQLINTEGER		*StringLengthPtr);

typedef SQLRETURN (SQL_API* SQLGetDescFieldWFun) (
	SQLHDESC		descriptorHandle,
	SQLSMALLINT		RecNumber,
	SQLSMALLINT		FieldIdentifier,
	SQLPOINTER		ValuePtr,
	SQLINTEGER		BufferLength,
	SQLINTEGER		*StringLengthPtr);

typedef SQLRETURN (SQL_API* SQLGetDescRecFun )(
	SQLHDESC		descriptorHandle,
	SQLSMALLINT		RecNumber,
  	SQLCHAR			*Name,
  	SQLSMALLINT		BufferLength,
  	SQLSMALLINT		*StringLengthPtr,
  	SQLSMALLINT		*TypePtr,
  	SQLSMALLINT		*SubTypePtr,
  	SQLLEN			*LengthPtr,
  	SQLSMALLINT		*PrecisionPtr,
  	SQLSMALLINT		*ScalePtr,
  	SQLSMALLINT		*NullablePtr);

typedef SQLRETURN (SQL_API* SQLGetDescRecAFun) (
	SQLHDESC		descriptorHandle,
	SQLSMALLINT		RecNumber,
	SQLCHAR			*Name,
	SQLSMALLINT		BufferLength,
	SQLSMALLINT		*StringLengthPtr,
	SQLSMALLINT		*TypePtr,
	SQLSMALLINT		*SubTypePtr,
	SQLLEN			*LengthPtr,
	SQLSMALLINT		*PrecisionPtr,
	SQLSMALLINT		*ScalePtr,
	SQLSMALLINT		*NullablePtr);

typedef SQLRETURN (SQL_API* SQLGetDescRecWFun )(
	SQLHDESC		descriptorHandle,
  	SQLSMALLINT		RecNumber,
  	SQLWCHAR		*Name,
  	SQLSMALLINT		BufferLength,
  	SQLSMALLINT		*StringLengthPtr,
  	SQLSMALLINT		*TypePtr,
  	SQLSMALLINT		*SubTypePtr,
  	SQLLEN			*LengthPtr,
  	SQLSMALLINT		*PrecisionPtr,
  	SQLSMALLINT		*ScalePtr,
  	SQLSMALLINT		*NullablePtr);

typedef SQLRETURN (SQL_API* SQLSetDescFieldFun) (
	SQLHDESC		descriptorHandle,
	SQLSMALLINT		RecNumber,
	SQLSMALLINT		FieldIdentifier,
	SQLPOINTER		ValuePtr,
	SQLINTEGER		BufferLength);

typedef SQLRETURN (SQL_API* SQLSetDescFieldAFun) (
	SQLHDESC		descriptorHandle,
	SQLSMALLINT		RecNumber,
	SQLSMALLINT		FieldIdentifier,
	SQLPOINTER		ValuePtr,
	SQLINTEGER		BufferLength);

typedef SQLRETURN (SQL_API* SQLSetDescFieldWFun )(
	SQLHDESC		descriptorHandle,
	SQLSMALLINT		RecNumber,
	SQLSMALLINT		FieldIdentifier,
	SQLPOINTER		ValuePtr,
	SQLINTEGER		BufferLength);

typedef SQLRETURN (SQL_API* SQLSetDescRecFun) (
	SQLHDESC		DescriptorHandle,
  	SQLSMALLINT		RecNumber,
  	SQLSMALLINT		Type,
  	SQLSMALLINT		SubType,
  	SQLLEN		  	Length,
  	SQLSMALLINT		Precision,
  	SQLSMALLINT		Scale,
  	SQLPOINTER		Data,
  	SQLLEN			*StringLength,
  	SQLLEN			*Indicator);

#endif