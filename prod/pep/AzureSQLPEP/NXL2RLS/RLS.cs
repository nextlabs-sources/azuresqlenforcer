using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Data;

namespace NXL2RLS
{
    class RLS
    {
        public static readonly string m_strSchemaName = "NXLSQLEnforce";
        public static readonly string FILTER_PREDICATE = "FILTER PREDICATE";
        public static readonly string BLOCK_PREDICATE = "BLOCK PREDICATE";
        public static readonly string AFTER_INSERT =  "AFTER INSERT";
        public static readonly string BEFORE_UPDATE = "BEFORE UPDATE";
        public static readonly string AFTER_UPDATE =  "AFTER UPDATE";
        public static readonly string BEFORE_DELETE = "BEFORE DELETE";


        public static void ClearNxlInfrastructure(DBConnection dbConn, RLSBackupTable rlsTable)
        {
            //drop security policy
            if(!string.IsNullOrWhiteSpace(rlsTable.m_strSecurityPolicy))
            {
                string sqlCmd = "DROP SECURITY POLICY " + rlsTable.m_strSecurityPolicy;
                bool bDropPolicy = dbConn.ExecuteCommand(sqlCmd);
                Log.Instance.WriteLog("Drop Security policy:{0}, result:{1}, sql:{2}\n", rlsTable.m_strSecurityPolicy, bDropPolicy, sqlCmd);
            }

            //drop function
            if(!string.IsNullOrWhiteSpace(rlsTable.m_strViewFun))
            {
                DropFunction(dbConn, rlsTable.m_strViewFun);
            }

            if (!string.IsNullOrWhiteSpace(rlsTable.m_strEditFun))
            {
                DropFunction(dbConn, rlsTable.m_strEditFun);
            }

            if(!string.IsNullOrWhiteSpace(rlsTable.m_strCreateFun))
            {
                DropFunction(dbConn, rlsTable.m_strCreateFun);
            }

            if(!string.IsNullOrWhiteSpace(rlsTable.m_strDeleteFun))
            {
                DropFunction(dbConn, rlsTable.m_strDeleteFun);
            }
        }

        public static void ClearNxlInfrastructure(DBConnection dbConn, bool bDropSchema)
        {
            Log.Instance.WriteLog("Begin ClearNxlInfrastructure.\n");

            //get schema id
            int nSchemaID = 0;
            if(!RetrieveSchemaIDByName(dbConn, m_strSchemaName, out nSchemaID))
            {
                Log.Instance.WriteLog("RetrieveSchemaIDByName failed, ClearNxlInfrastructure return, schema name:{0}\n", m_strSchemaName);
                return;
            }

            //clear SECURITY policy under this schema
            DropAllSecurityPolicyUnderSchema(dbConn, nSchemaID);

            //Drop Functions under this schema
            DropAllFunctionsUnderSchema(dbConn, nSchemaID);

            //Drop Schema
            if(bDropSchema)
            {
                DropNxlSchema(dbConn, m_strSchemaName);
            }

            Log.Instance.WriteLog("End ClearNxlInfrastructure.\n");
        }

        public static bool DropFunction(DBConnection dbConn, string strFun)
        {
            string sqlText = "DROP FUNCTION " + strFun;
            bool bDrop = dbConn.ExecuteCommand(sqlText);

            Log.Instance.WriteLog("Drop function result:{0}, sql:{1}\n", bDrop, sqlText);
            return bDrop;
        }


        public static bool CreateNxlSchema(DBConnection dbConn)
        {
            string sqlText = "";
            try
            {
                 sqlText = @"
                    if not exists(
                      select name from sys.schemas where name = '{0}'
                    )
                    begin
                      EXEC sp_executesql N'CREATE SCHEMA {1}'
                    end";
                return dbConn.ExecuteCommand(String.Format(sqlText, m_strSchemaName, m_strSchemaName));
            }
            catch (Exception ex)
            {
                Log.Instance.WriteLog("Exception on CreateNxlSchema,{0} sql:{1}\n", ex.Message, sqlText);
                return false;
            }
        }

        public static bool CreateSecurityPolicy(DBConnection dbConn, string strPolicyName, int nSchemaID)
        {
            string sqlText = @"if not exists( select name from sys.objects where name = '{0}' and schema_id={1})
                    begin
                    EXEC sp_executesql N'CREATE SECURITY POLICY {2}.{0} WITH (STATE=ON,SCHEMABINDING=OFF)'
                    end";
            bool bCreate = dbConn.ExecuteCommand(String.Format(sqlText, strPolicyName, nSchemaID, m_strSchemaName));

            Log.Instance.WriteLog("CreateSecurityPolicy result:{0}, PolicyName:{1}, schemaID:{2}\n", bCreate, strPolicyName, nSchemaID);
            return bCreate;
        }

        public static void DropSecurityPolicyForSpecialPredicate(DBConnection dbConn, string strPolicyName,
            TableInfo table, string strPredicateType, string strEventType)
        {
            string sqlDrop = string.Format(@"ALTER SECURITY POLICY {0} DROP {1} on {2}.{3} {4}",
                                      strPolicyName, strPredicateType, table.Schema, table.Name, strEventType);
            bool bDrop = dbConn.ExecuteCommand(sqlDrop);

            Log.Instance.WriteLog("DropSecurityPolicyForSpecialPredicate Result:{0}, sql:{1}\n", bDrop, sqlDrop);
        }

        public static void AlterSecurityPolicyForSpecialPredicate(DBConnection dbConn, string strPolicyName,
            string strFunction, TableInfo table, Dictionary<string, ColumnInfo> dicCols,
            string strPredicateType, string strEventType)
        {
            //drop first, if it is no exit, it will return false, let it.
            DropSecurityPolicyForSpecialPredicate(dbConn, strPolicyName, table, strPredicateType, strEventType);

            //added
            if (!string.IsNullOrWhiteSpace(strFunction))
            {
                string addText = ConstructAddPredicateSql(strPredicateType, strFunction, table, dicCols, strEventType);
                string sqlAdd = string.Format("ALTER SECURITY POLICY {0} {1}", strPolicyName, addText);
                bool bAdd = dbConn.ExecuteCommand(sqlAdd);

                Log.Instance.WriteLog("AlterSecurityPolicyForFilterPred Result:{0}, sql:{1}\n", bAdd, sqlAdd);
            } 
        }

        public  static bool FunctionExist(DBConnection dbConn, string strFunName, int nSchemaID)
        {
            string sqlText = string.Format("SELECT name from sys.objects where name='{0}' and schema_id={1}", strFunName, nSchemaID);
            DataSet ds = dbConn.ExecuteWithResult(sqlText);
            return (ds!=null) && (ds.Tables.Count != 0) && (ds.Tables[0].Rows.Count != 0);
        }

        private static void DropNxlSchema(DBConnection dbConn, string strSchema)
        {
            try
            {
                string sqlText = @"
                    if exists(
                      select name from sys.schemas where name = '{0}'
                    )
                    begin
                      EXEC sp_executesql N'DROP SCHEMA {1}'
                    end";
                dbConn.ExecuteCommand(String.Format(sqlText, strSchema, strSchema));
            }
            catch (Exception)
            {
            }
        }

        private static void DropAllFunctionsUnderSchema(DBConnection dbConn, int nSchemaID)
        {
            try
            {
                string sqlText = "SELECT name FROM sys.objects WHERE RIGHT(type_desc,8)='FUNCTION' and schema_id=" + nSchemaID.ToString();
                DataSet dataSet = dbConn.ExecuteWithResult(sqlText);
                if (dataSet != null)
                {
                    DataRowCollection rows = dataSet.Tables[0].Rows;
                    foreach (DataRow row in rows)
                    {
                        sqlText = "DROP FUNCTION " + m_strSchemaName + "." + row.ItemArray[0].ToString();
                        bool bDropFunction = dbConn.ExecuteCommand(sqlText);
                        Log.Instance.WriteLog("Drop function result:{0}, sql:{1}\n", bDropFunction, sqlText);
                    }
                }
            }
            catch (Exception ex)
            {
                Log.Instance.WriteLog("Exception on DropAllFunctionsUnderSchema,{0}\n", ex.Message);
            }
        }

        private static void DropAllSecurityPolicyUnderSchema(DBConnection dbConn, int nSchemaID)
        {
            //query security policy
            try
            {
                string sqlText = "SELECT name FROM sys.objects WHERE type_desc='SECURITY_POLICY' and schema_id=" + nSchemaID.ToString();
                DataSet dataSet = dbConn.ExecuteWithResult(sqlText);
                if (dataSet != null)
                {
                    DataRowCollection rows = dataSet.Tables[0].Rows;
                    foreach(DataRow row in rows)
                    {
                        sqlText = "DROP  SECURITY POLICY " + m_strSchemaName + "." + row.ItemArray[0].ToString();
                        bool bDropPolicy = dbConn.ExecuteCommand(sqlText);
                        Log.Instance.WriteLog("Drop policy result:{0}, sql:{1}\n", bDropPolicy, sqlText);
                    }
                }
                else
                {
                    Log.Instance.WriteLog("Select security policy with no data returned, sql:{0}\n", sqlText);
                }
            }
            catch(Exception ex)
            {
                Log.Instance.WriteLog("Exception on DropAllSecurityPolicyUnderSchema,{0}\n", ex.Message);
            }
            
        }


        public static bool RetrieveSchemaIDByName(DBConnection dbConn, string strSchemaName, out int nID)
        {
            nID = 0;
            try
            {
                string sqlText = string.Format("Select top 1 schema_id from sys.schemas where name = '{0}';", m_strSchemaName);
                DataSet dataSet = dbConn.ExecuteWithResult(sqlText);
                if (dataSet != null)
                {
                    nID = int.Parse(dataSet.Tables[0].Rows[0].ItemArray[0].ToString());
                }
                return true;
            }
            catch(Exception)
            {
                return false;
            }
           
        }

        public static string ConstructAddPredicateSql(string strPredType, string strPredFun, TableInfo table, Dictionary<string,ColumnInfo> dicCols, string strEventType)
        {
            //construct paramaters
            string strParamaers = "";
            foreach (KeyValuePair<string,ColumnInfo> colData in dicCols)
            {
                if(!string.IsNullOrWhiteSpace(strParamaers))
                {
                    strParamaers += ",";
                }

                strParamaers += colData.Key;
            }

            //format sql
            string strSqlText  = string.Format("ADD {0} {5}.{1}({2}) ON {3} {4}",
                strPredType, strPredFun, strParamaers, table.Schema+"."+table.Name, strEventType,
                m_strSchemaName);


            return strSqlText;
        }
    }
}
