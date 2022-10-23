using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Data.SqlClient;
using System.Data;
using System.IO;
using System.Reflection;

namespace NXL2RLS
{
    enum EnforceTableResult
    {
        Success,
        FailedQueryPolicy,
        FailedNoCondition,
        FailedCreateFunException,
    }

    class DBInfo
    {
        private string m_strName;
        private List<TableInfo> m_lstTables;
        private DBConnection m_dbConn = null;

        private readonly string m_strFunTemplateDirName = "funtemplate\\";
        private readonly string m_strFunNameStringCompare = "StringCompare";
        private readonly string m_strFunNameNumberCompare = "IntCompare";
        private readonly string m_strFunNameGetUserAttrStr = "GetUserAttributeString";

        public DBInfo(string strDBName, List<TableInfo> lstTableInfo)
        {
            m_strName = strDBName;
            m_lstTables = lstTableInfo;
        }

        public DBInfo(string strDBName)
        {
            m_strName = strDBName;
            m_lstTables = new List<TableInfo>();
        }

        public void AddedTable(string strTableName, string strSchema)
        {
            TableInfo table = new TableInfo(strTableName, strSchema);
            m_lstTables.Add(table);
        }

        public string Name
        {
            get { return m_strName; }
        }

        public bool Connect()
        {

            if (m_dbConn == null)
            {
                try
                {
                    m_dbConn = new DBConnection();
                    m_dbConn.ConnectToSqlServer(Program.g_Config.RemoteSqlServer, Program.g_Config.RemoteSqlPort,
                        m_strName, Program.g_Config.SQLAccount, Program.g_Config.SQLPwd);
                    return true;
                }
                catch (Exception)
                {
                    return false;
                }

            }

            return true;
        }

        public void DisConnect()
        {
            if (m_dbConn == null)
            {
                m_dbConn.Close();
            }
        }

        public bool GetUserInfoTableColumnInfo()
        {
            //query column info for each table
            try
            {
                QueryColumnInfoForTable(Program.g_UserInfoTable);
            }
            catch (Exception)
            {
                //log out
            }

            return true;
        }

        public bool QueryColumnInfo()
        {
            //query column info for each table
            try
            {
                foreach (TableInfo table in m_lstTables)
                {
                    bool bColumnInfo = QueryColumnInfoForTable(table);
                    if(!bColumnInfo)
                    {
                        table.IsValid = false;
                        Log.Instance.WriteLog("QueryColumnInfoForTable failed for[{0}.{1}.{2}], ignore it\n", Name,table.Schema, table.Name);
                    }
                }
            }
            catch (Exception)
            {
                //log out
            }

            return true;
        }

        private bool QueryColumnInfoForTable(TableInfo table)
        {
            DataSet dataSet = m_dbConn.ExecuteWithResult("EXEC sp_help " + string.Format("'[{0}].[{1}]'", table.Schema,table.Name));
            if (dataSet != null)
            {
                int nTableCount = dataSet.Tables.Count;
                DataTable dt = dataSet.Tables[1];

                string strLog = string.Format("QueryColumnInfoForTable,table:{2}.{0}.{1}, columns:", table.Schema, table.Name, Name);
                foreach (DataRow row in dt.Rows)
                {
                    table.AddedColumn(row.ItemArray[0].ToString(), row.ItemArray[1].ToString());
                    strLog += string.Format("{0}({1}),", row.ItemArray[0].ToString(), row.ItemArray[1].ToString());
                }

                Log.Instance.WriteLog(strLog + "\n");
                return true;
            }
            else
            {
                Log.Instance.WriteLog("QueryColumnInfoForTable return null, table:{0}.{1}, db:{2}\n",table.Schema, table.Name, Name);
                return false;
            }
        }

    
        //here we convert NXL policy to RLS policy.
        //first we must clear the old RLS policy and functions
        public bool DoEnforcement(RLSBackup rlsOld, RLSBackup rlsNew)
        {
            //first we must clear the function we create before
           // RLS.ClearNxlInfrastructure(m_dbConn, false);
          

            //create schema, all function, security policy create by us will within this schema
            if(!RLS.CreateNxlSchema(m_dbConn))
            {
                Log.Instance.WriteLog("CreateNxlSchema failed for db:{0}, return.\n", Name);
                return false;
            }

            //get schema id
            int nSchemaID = 0;
            if (!RLS.RetrieveSchemaIDByName(m_dbConn, RLS.m_strSchemaName, out nSchemaID))
            {
                Log.Instance.WriteLog("RetrieveSchemaIDByName failed, ClearNxlInfrastructure return, schema name:{0}\n", RLS.m_strSchemaName);
                return false;
            }

            //create common function that will be use in all RLS policy
            if (!CreateCommonUserDefinedFunction(nSchemaID))
            {
                return false;
            }

            //create get user attribute function
            if(!CreateGetUserAttributeFunction(nSchemaID))
            {
                return false;
            }


            //query policy and create RLS policy/ function for it
            foreach(TableInfo table in m_lstTables)
            {
                if(table.IsValid)
                {
                    DoEnforcementForTable(table, rlsOld, rlsNew, nSchemaID);
                } 
            }

            //remove RLS on tables that is not be enforced
            List<RLSBackupTable> rlsBackupTables = rlsOld.FindBackupTablesByDB(Name);
            foreach(RLSBackupTable backupTable in rlsBackupTables)
            {
                Log.Instance.WriteLog("table:{0}.{1}.{2} is not being enforcered, clear all Infrastructure under it.\n", Name, backupTable.m_strSchema, backupTable.m_strName);
                RLS.ClearNxlInfrastructure(m_dbConn, backupTable);

                rlsOld.RemoveBackupTable(backupTable.m_strDBName, backupTable.m_strSchema, backupTable.m_strName);
            }

            return true;
        }

        private bool CreateConditionWithRecordFilterObligation(ObRecordFilter recordFilter, TableInfo table, Dictionary<string, ColumnInfo> dicRelateCols, out string strCondition)
        {
            strCondition = "";

            List<ObCondition> lstCondition = recordFilter.ListConditions;

            foreach (ObCondition con in lstCondition)
            {

                //get user column infor
                ColumnInfo RecordCol = table.GetColumnByName(con.Value1);
                if (null == RecordCol)
                {
                    Log.Instance.WriteLog("within record filter,can't find the column:{0} within talbe:{1}, ignore the whole policy\n", con.Value1, table.Name);
                    strCondition = "";
                    return false;
                }
                else
                {
                    dicRelateCols[con.Value1] = RecordCol;
                }

                if (!string.IsNullOrWhiteSpace(strCondition))
                {
                    strCondition += " AND "; //"AND" within obligation
                }

                //calculate variable value for value2
                string strValue2 = con.Value2;
                bool isReferValue = false;
                if (Policy.IsReferToUserAttribute(strValue2))
                {
                    string strResourceAttrName = Policy.GetReferToAttributeName(strValue2);

                    ColumnInfo col = Program.g_UserInfoTable.GetColumnByName(strResourceAttrName);
                    if (null != col)
                    {
                        strValue2 = string.Format("{0}.{1}('{2}')", RLS.m_strSchemaName, m_strFunNameGetUserAttrStr, strResourceAttrName); //String.Format("(select {0} from {1} where {2}=suser_sname())", strResourceAttrName, Program.g_Config.UserInfoTableName, Program.g_Config.UserInfoPriKey);
                        //"(SELECT " + strResourceAttrName + " FROM " + Program.g_Config.UserInfoTableName + " WHERE SUSER_SNAME()=" + Program.g_Config.UserInfoPriKey + ")";
                        isReferValue = true;
                    }
                    else
                    {
                        //Error!!!
                        Log.Instance.WriteLog("within record filter, can't find the refer column[{0}], ignore the whole policy.\n", strValue2);
                        strCondition = "";
                        return false;
                    }
                }

                if(!isReferValue)
                {
                    if (RecordCol.IsStringType() || con.Operator.Contains("like") )
                    {
                        strValue2 = "'" + strValue2 + "'";
                    }
                    else
                    {
                        if (con.Operator.Contains("null"))
                        {
                            //for 'is null' or 'is not null' we ignore the check for value2
                            //we set it to 0, we not realy use it
                            strValue2 = "0";

                        }
                        else
                        {
                            int nValue2 = 0;
                            bool isInt = int.TryParse(strValue2, out nValue2);
                            if (!isInt)
                            {
                                Log.Instance.WriteLog("within record filter, the Column[{0}] type is int, but the compare value is not int[{1}], ignore the whole policy\n", RecordCol.Name, strValue2);
                                strCondition = "";
                                return false;
                            }
                        }
                    }
                }

                //select compare function
                string strFunction = m_strFunNameNumberCompare;
                if(RecordCol.IsStringType() || con.Operator.Contains("like"))
                {
                    strFunction = m_strFunNameStringCompare;
                }



                strCondition += String.Format(
                    "({0}.{1}(@{2}, {3}, '{4}')=1)",
                    RLS.m_strSchemaName,
                    strFunction,
                    con.Value1,
                    strValue2,
                    con.Operator                
                );
                    //"(" + RLS.m_strSchemaName + "." +
                    //(RecordCol.IsStringType() ? m_strFunNameStringCompare : m_strFunNameNumberCompare) +
                    // "(@" + con.Value1 + "," +
                    // ((RecordCol.IsStringType() && !isReferValue) ? "'" + strValue2 + "'" : strValue2) +
                    // "," + "'" + con.Operator + "')=1)";

            }

            //return 
            if(!string.IsNullOrWhiteSpace(strCondition))
            {
                strCondition = "(" + strCondition + ")";
            }

            return true; 
        }

        private bool CreateConditionWithUserFilterObligation(TableInfo table, ObUserFilter userFilter, Dictionary<string, ColumnInfo> dicRelateCols, out string strCondition)
        {
            strCondition = "";
            List<ObCondition> lstCondition = userFilter.ListConditions;

            foreach(ObCondition con in lstCondition)
            {

                //get user column infor
                ColumnInfo userCol = Program.g_UserInfoTable.GetColumnByName(con.Value1);
                if(null==userCol)
                {
                    Log.Instance.WriteLog("within user filter,can't find the column:{0}, ignore the whole policy.\n", con.Value1);
                    strCondition = "";
                    return false;
                }

                string strUserInfoValue = string.Format("{0}.{1}('{2}')", RLS.m_strSchemaName, m_strFunNameGetUserAttrStr, con.Value1); //String.Format("(select {0} from {1} where {2}=suser_sname())", con.Value1, Program.g_Config.UserInfoTableName, Program.g_Config.UserInfoPriKey);
                //"(SELECT " + con.Value1 + " FROM " + Program.g_Config.UserInfoTableName + " WHERE SUSER_SNAME()=" + Program.g_Config.UserInfoPriKey + ")";


                if(!string.IsNullOrWhiteSpace(strCondition))
                {
                    strCondition += " AND "; //"AND" within obligation
                }

                //calculate variable value for value2
                string strValue2 = con.Value2;
                bool isReferValue = false;
                if(Policy.IsReferToResourceAttribute(strValue2))
                {
                    string strResourceAttrName = Policy.GetReferToAttributeName(strValue2);

                    ColumnInfo col = table.GetColumnByName(strResourceAttrName);
                    if (null != col)
                    {
                        dicRelateCols[strResourceAttrName] = col;
                        strValue2 = "@" + strResourceAttrName;
                        isReferValue = true;
                    }
                    else
                    {
                        //Error!!!
                        Log.Instance.WriteLog("within user filter, can't find the refer column[{0}], ignore the whole policy.\n", strValue2);
                        strCondition = "";
                        return false;
                    } 
                }


                if (!isReferValue)
                {
                    if (userCol.IsStringType())
                    {
                        strValue2 = "'" + strValue2 + "'";
                    }
                    else
                    {
                        if (con.Operator.Contains("null"))
                        {
                            //for 'is null' or 'is not null' we ignore the check for value2
                            //we set it to 0, we not realy use it
                            strValue2 = "0";

                        }
                        else
                        {
                            int nValue2 = 0;
                            bool isInt = int.TryParse(strValue2, out nValue2);
                            if (!isInt)
                            {
                                Log.Instance.WriteLog("within user filter, the Column[{0}] type is int, but the compare value is not int[{1}], ignore the whole policy\n", userCol.Name, strValue2);
                                strCondition = "";
                                return false;
                            }
                        }
                    }
                }

                strCondition += String.Format(
                    "({0}.{1}({2}, {3}, '{4}')=1)",
                    RLS.m_strSchemaName,
                    userCol.IsStringType() ? m_strFunNameStringCompare : m_strFunNameNumberCompare,
                    strUserInfoValue,
                    strValue2,
                    con.Operator
                );
                //strCondition += "(" + RLS.m_strSchemaName + "."  +
                //    (userCol.IsStringType() ? m_strFunNameStringCompare : m_strFunNameNumberCompare) +    
                //     "(" + strUserInfoValue + "," +
                //     ((userCol.IsStringType() && !isReferValue) ?  "'" + strValue2 + "'" : strValue2) + 
                //     "," + "'" + con.Operator +"')=1)"; 
              
            }


            //
            if(!string.IsNullOrWhiteSpace(strCondition))
            {
                strCondition = "(" + strCondition + ")";
            }

            return true;
        }

        //(userfilter AND record filter 1) or(userfilter AND record filter 2).
        private string CreateConditionWithPolicy(Policy policy, TableInfo table, Dictionary<string, ColumnInfo> dicRelateCols)
        {
            string strConditions = "";

            //get user filter condition
            string strUserFiltercondition = "";
            ObUserFilter obUserFilter = policy.GetUserFilterObligation();
            if(obUserFilter != null)
            {
                if(!CreateConditionWithUserFilterObligation(table, obUserFilter, dicRelateCols, out strUserFiltercondition))
                {
                    Log.Instance.WriteLog("CreateConditionWithUserFilterObligation failed. ignore the whole policy.\n");
                    return "";
                }
            }


            //get record filter condition
            string strRecordFilterCondition = "";
            List<Obligation> lstObs = policy.GetRecordFilterObligation();
            if (null != lstObs && lstObs.Count > 0)
            {
                foreach (Obligation ob in lstObs)
                {
                    bool bCreateRecordFilter = CreateConditionWithRecordFilterObligation(ob as ObRecordFilter, table, dicRelateCols, out strRecordFilterCondition);
                    if(!bCreateRecordFilter)
                    {
                        Log.Instance.WriteLog("CreateConditionWithRecordFilterObligation failed. ignore the whole policy.\n");
                        return "";
                    }

                    //construct single condition
                    if (!string.IsNullOrWhiteSpace(strRecordFilterCondition))
                    {
                        string strSingleCondition = "(" + (string.IsNullOrWhiteSpace(strUserFiltercondition) ? "" :
                        strUserFiltercondition + " AND ") + strRecordFilterCondition + ")";

                        //combine condition
                        if (!string.IsNullOrWhiteSpace(strConditions))
                        {
                            strConditions += " OR ";
                        }
                        strConditions += strSingleCondition;
                    }

                }
            }

            //have no condition(1.no recording condition, 2. no valible record condition 
            //we set it to only user condition
            if (string.IsNullOrWhiteSpace(strConditions))
            {
                strConditions = strUserFiltercondition;
            }


            //return 
            if (!string.IsNullOrWhiteSpace(strConditions))
            {
                return "(" + strConditions + ")";
            }
            else
            {
                return "";
            }


        }

        private bool CreatePredicateFunction(string strFunName, TableInfo table, string strConditions, Dictionary<string, ColumnInfo> dicRelateColumns, int nSchemaID)
        {
            //construct function argument
            string strFunArgument = "";
            foreach(KeyValuePair<string,ColumnInfo> ColData in dicRelateColumns)
            {
                string strArg = "@" + ColData.Key + " ";
                strArg += ColData.Value.IsStringType() ? "varchar(255)" : "int";
                
                //combine argument
                if(!string.IsNullOrWhiteSpace(strFunArgument))
                {
                    strFunArgument += ", ";
                }

                strFunArgument += strArg;
            }

            //construct Sql
            string SqlFmt = "{0}.{1}({2})\r\n" +
                           "RETURNS TABLE\r\n" +
                           "AS\r\n" +
                           "RETURN (SELECT 1 AS'Result' WHERE {3});";

            string strSqlCmd = string.Format(SqlFmt, RLS.m_strSchemaName, strFunName, strFunArgument, strConditions);

            if (RLS.FunctionExist(m_dbConn, strFunName, nSchemaID))
            {
                strSqlCmd = "ALTER FUNCTION " + strSqlCmd;
            }
            else
            {
                strSqlCmd = "CREATE FUNCTION " + strSqlCmd;
            }

            bool bCreateFun = m_dbConn.ExecuteCommand(strSqlCmd);
            Log.Instance.WriteLog("Create function result:{0}, table:{1}, sql:{2}\n", bCreateFun,table.Name, strSqlCmd);

            return bCreateFun;
        }



        private EnforceTableResult DoEnforcerForAction(string strAction, TableInfo table, Dictionary<string, ColumnInfo> dicRelateCols, int nSchemaID, out string strFunName)
        {
            strFunName = "";
            string strConditions = "";

            QueryPolicyResult queryResult = Program.g_QueryPolicy.QueryPolicyInfo(strAction, Program.g_Config.RemoteSqlServer, m_strName, table.Schema + "." + table.Name);
            if(queryResult==null)
            {
                return EnforceTableResult.FailedQueryPolicy;
            }

            if(queryResult.IsAllow())
            {
                List<Policy> lstPolicy = queryResult.lstPolicy;

                if (null != lstPolicy && lstPolicy.Count > 0)
                {
                    foreach (Policy policy in lstPolicy)
                    {
                        string strCon = CreateConditionWithPolicy(policy, table, dicRelateCols);
                        if (string.IsNullOrWhiteSpace(strCon))
                        {
                            continue;
                        }

                        if (!string.IsNullOrWhiteSpace(strConditions))
                        {
                            strConditions += " OR ";  //use "OR" between policys
                        }

                        strConditions += strCon;
                    }
                }
                else
                {
                    Log.Instance.WriteLog("Policy result is Allow but there are no obligation(condition).Added true condition\n");
                    strConditions = "(1=1)";
                }
            }
            else
            {
               Log.Instance.WriteLog("Policy result is {0}, ignore the whole obligation.\n", queryResult.emPolicyResult);
            }

            //if configed "SecurityByDefault" and there are no conditions set by policy. we added a "False" condition to prevent the data by operated
            if (string.IsNullOrWhiteSpace(strConditions) && Program.g_Config.SecurityByDefault)
            {
                Log.Instance.WriteLog("No Policy is set and SecurityByDefault is true, Added False condition.\n");
                strConditions = "(1=0)";
            }


            //create function
            if (!string.IsNullOrWhiteSpace(strConditions))
            {
                //construct function name
                strFunName = table.Schema + "_" + table.Name + "_" + strAction + "Predicate";
                bool bCreate = CreatePredicateFunction(strFunName, table, strConditions, dicRelateCols, nSchemaID);
                return bCreate ? EnforceTableResult.Success : EnforceTableResult.FailedCreateFunException;
            }
            else
            {
                return EnforceTableResult.FailedNoCondition;
            }
        }


        private bool GrantPermission(string strObject, string strAction, string strToUser)
        {
            string strSqlGrant = string.Format("GRANT {0} ON {1} TO {2}", strAction, strObject, strToUser);
            bool bGrant = m_dbConn.ExecuteCommand(strSqlGrant);
            Log.Instance.WriteLog("GrantPermission result:{0}, sql:{1}\n", bGrant, strSqlGrant);
            return bGrant;
        }

        private string GetRLSPredicateTypeByAction(string strAction)
        {
            if(strAction.Equals(QueryPolicy.m_strActionQuery, StringComparison.OrdinalIgnoreCase)){
                return RLS.FILTER_PREDICATE;
            }
            else {
                return RLS.BLOCK_PREDICATE;
            }
        }

        private string GetRLSEventTypeByAction(string strAction)
        {
            if (strAction.Equals(QueryPolicy.m_strActionQuery, StringComparison.OrdinalIgnoreCase))
            {
                return "";
            }
            else if(strAction.Equals(QueryPolicy.m_strActionInsert,StringComparison.OrdinalIgnoreCase)){
                return RLS.AFTER_INSERT;
            }
            else if (strAction.Equals(QueryPolicy.m_strActionUpdate, StringComparison.OrdinalIgnoreCase))
            {
                return RLS.BEFORE_UPDATE;
            }
            else if (strAction.Equals(QueryPolicy.m_strActionDelete, StringComparison.OrdinalIgnoreCase))
            {
                return RLS.BEFORE_DELETE;
            }

            return "";
        }

        private void DoEnforcerForAction(string strAction, TableInfo table, RLSBackup rlsOld, RLSBackup rlsNew, int nSchemaID, string strSecurityPolicyName)
        {
            string strPredicateType = GetRLSPredicateTypeByAction(strAction);
            string strEventType = GetRLSEventTypeByAction(strAction);

            string strPredicateFun = "";
            Dictionary<string, ColumnInfo> dicRelateColumnsFilterPredicate = new Dictionary<string, ColumnInfo>();
            EnforceTableResult EnforceRes = DoEnforcerForAction(strAction, table, dicRelateColumnsFilterPredicate,
                nSchemaID, out strPredicateFun);

            if (EnforceRes == EnforceTableResult.Success)
            {
                //make the function been accessible for all user
                GrantPermission(RLS.m_strSchemaName + "." + strPredicateFun, "SELECT", "PUBLIC");

                //added to backup rls
                rlsNew.AddedFunction(Name, table.Schema, table.Name, strAction,
                    RLS.m_strSchemaName + "." + strPredicateFun);

                //added it to security policy
                RLS.AlterSecurityPolicyForSpecialPredicate(m_dbConn, strSecurityPolicyName,
                    strPredicateFun, table, dicRelateColumnsFilterPredicate,
                    strPredicateType, strEventType);
            }
            else if (EnforceRes == EnforceTableResult.FailedNoCondition)
            {
                //drop the previous function
                string strPreviousFun = rlsOld.FindFunction(Name, table.Schema, table.Name,
                    strAction);
                if (!string.IsNullOrWhiteSpace(strPreviousFun))
                {
                    RLS.DropSecurityPolicyForSpecialPredicate(m_dbConn, strSecurityPolicyName, table,
                        strPredicateType, strEventType);
                    RLS.DropFunction(m_dbConn, strPreviousFun);
                }
            }
            else
            {
                //other exceptions we didn't change anything， but we need to added the function to backup
                rlsNew.AddedFunction(Name, table.Schema, table.Name, strAction,
                    RLS.m_strSchemaName + "." + strPredicateFun);
            }
        }

        private void DoEnforcementForTable(TableInfo table, RLSBackup rlsOld, RLSBackup rlsNew,  int nSchemaID)
        {
            Log.Instance.WriteLog("Begin DoEnforcementForTable for:{0}.{1}.{2}\n", Name,table.Schema, table.Name);

            //create security policy first
            string strSecurityPolicyName = "Policy_" + table.Schema + "_" + table.Name;
            if(!RLS.CreateSecurityPolicy(m_dbConn, strSecurityPolicyName, nSchemaID))
            {
                return;
            }
            strSecurityPolicyName = RLS.m_strSchemaName + '.' + strSecurityPolicyName;
            rlsNew.AddedSecurityPolicy(Name, table.Schema, table.Name, strSecurityPolicyName);

            //first for FILTER PREDICATE
            DoEnforcerForAction(QueryPolicy.m_strActionQuery, table, rlsOld, rlsNew, nSchemaID, strSecurityPolicyName);

            //for BLOCK PREDICATE After Insert
            DoEnforcerForAction(QueryPolicy.m_strActionInsert, table, rlsOld, rlsNew, nSchemaID, strSecurityPolicyName);
            
            //for BLOCK PREDICATE before update
            DoEnforcerForAction(QueryPolicy.m_strActionUpdate, table, rlsOld, rlsNew, nSchemaID, strSecurityPolicyName);

            //for BLOCK PREDICATE before delete
            DoEnforcerForAction(QueryPolicy.m_strActionDelete, table, rlsOld, rlsNew, nSchemaID, strSecurityPolicyName);

            //
            rlsOld.RemoveBackupTable(Name, table.Schema, table.Name);
        }

        private string GetCommonFunctionName(string strFun)
        {
            int nPos = strFun.IndexOf('(');
            string strName = strFun.Substring(0, nPos);
            return strName.Trim();
        }


        private bool CreateGetUserAttributeFunction(int nSchemaID)
        {
            string strFunction = m_strFunNameGetUserAttrStr +
@"(@AttrName varchar(32)) 
Returns varchar(100) 
Begin
Declare @result varchar(100)
Declare @CurrentUserName  varchar(100)
SET @result = ''
SET @CurrentUserName = USER_NAME()
if( USER_NAME()='dbo')
  SET @CurrentUserName = SUSER_SNAME()

if(@AttrName='' or (@AttrName is null))
  SET @result = ''" + "\r\n";


            try
            {
                //loop for user attribute name
                TableInfo tbUserInfo = Program.g_UserInfoTable;
                foreach(ColumnInfo col in tbUserInfo.Columns)
                {
                    string strColSelect = string.Format("if(@AttrName='{0}')\r\nSET @result=(SELECT top 1 {0} FROM {1} where {2}=@CurrentUserName)\r\n", col.Name,
   Program.g_Config.UserInfoTableName, Program.g_Config.UserInfoPriKey);

                    strFunction += strColSelect;
                }

                strFunction += "return @result\r\nEND";

                string strSqlCmd = "";
                if (RLS.FunctionExist(m_dbConn, m_strFunNameGetUserAttrStr, nSchemaID))
                {
                    strSqlCmd = string.Format("ALTER FUNCTION {0}.{1}", RLS.m_strSchemaName, strFunction);
                }
                else
                {
                    strSqlCmd = string.Format("CREATE FUNCTION {0}.{1}", RLS.m_strSchemaName, strFunction);
                }


                bool bCreate = m_dbConn.ExecuteCommand(strSqlCmd);
                Log.Instance.WriteLog("CreateGetUserAttributeFunction result:{0},sql:{1}\n", bCreate, strSqlCmd);

            }
            catch(Exception ex)
            {
                Log.Instance.WriteLog("Exception on CreateGetUserAttributeFunction, ex:{0}\n", ex.Message);
                return false;
            }

            return true;
        }

        private bool CreateCommonUserDefinedFunction(int nSchemaID)
        {
            try
            {
                //load common function template
                string strAppDir = Path.GetDirectoryName(Assembly.GetExecutingAssembly().GetModules()[0].FullyQualifiedName);
                string strFunctionDir = strAppDir + "\\" + m_strFunTemplateDirName;

                DirectoryInfo dirInfo = new DirectoryInfo(strFunctionDir);
                FileInfo[] funFile = dirInfo.GetFiles();
                foreach (FileInfo fi in funFile)
                {
                    StreamReader sr = new StreamReader(fi.FullName);
                    String strFun = sr.ReadToEnd();
                    sr.Close();

                    //get function name
                    string strFunName = GetCommonFunctionName(strFun);

                    string strSqlCmd = "";
                    if(RLS.FunctionExist(m_dbConn, strFunName, nSchemaID))
                    {
                        strSqlCmd = string.Format("ALTER FUNCTION {0}.{1}", RLS.m_strSchemaName, strFun);
                    }
                    else
                    {
                        strSqlCmd = string.Format("CREATE FUNCTION {0}.{1}", RLS.m_strSchemaName, strFun);
                    }
                   

                    bool bCreate = m_dbConn.ExecuteCommand(strSqlCmd);

                    Log.Instance.WriteLog("CreateCommonUserDefinedFunction result:{0},sql:{1}\n", bCreate, strSqlCmd);


                    //grant execute permission for this function
                    GrantPermission(RLS.m_strSchemaName + "." + strFunName, "EXECUTE", "PUBLIC"); 

                }
            }
            catch(Exception ex)
            {
                Log.Instance.WriteLog("Exception on CreateCommonUserDefinedFunction, ex:{0}\n", ex.Message);
                return false;
            }
            
           
            return true;
        }

    }
}
