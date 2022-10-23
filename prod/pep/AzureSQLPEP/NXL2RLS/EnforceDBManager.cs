using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net;
using System.Runtime.Serialization.Json;
using System.Runtime.Serialization;
using System.IO;

namespace NXL2RLS
{
    [DataContract]
    class JSONTable
    {
        [DataMember(Name = "name")]
        public string m_strName;

        [DataMember(Name = "database")]
        public string m_strDBName;

        [DataMember(Name = "schema")]
        public string m_strSchema;
    }

    [DataContract]
    class JSONEnforceTables
    {
        [DataMember(Name = "tables")]
        public List<JSONTable> m_lstTables;
    }

    class EnforceDBManager
    {
        private List<DBInfo> m_lstEnforceDBInfo = new List<DBInfo>();

        public bool InitNeedEnforceTableInfo()
        {
            string strConfigServerUrl = Program.g_Config.ConfigServerUrl;
            string strConfigUID = Program.g_Config.ConfigUID;
            string strConfigIns = Program.g_Config.ConfigIns;

            string strSetingInfoUrl = strConfigServerUrl + (strConfigServerUrl.EndsWith("/") ? "ext_api/table/" : ("/" + "ext_api/table/"));
            strSetingInfoUrl += strConfigUID + "?proxy=" + strConfigIns;
            try
            {
                HttpWebRequest httpReq = (HttpWebRequest)HttpWebRequest.Create(strSetingInfoUrl);
                HttpWebResponse httpRes = (HttpWebResponse)httpReq.GetResponse();
                DataContractJsonSerializer jsonSer = new DataContractJsonSerializer(typeof(JSONEnforceTables));
                JSONEnforceTables jsonTables = (JSONEnforceTables)jsonSer.ReadObject(httpRes.GetResponseStream());
                httpRes.Close();

                foreach(JSONTable table in jsonTables.m_lstTables)
                {
                    DBInfo db = GetDBInfoByName(table.m_strDBName);
                    if(db==null)
                    {
                        db = AddDBInfo(table.m_strDBName);
                    }
                    db.AddedTable(table.m_strName, table.m_strSchema);

                    Log.Instance.WriteLog("Need enforce table:{2}.{0}.{1}\n",table.m_strSchema, table.m_strName, table.m_strDBName);
                }


            }
            catch (Exception e)
            {
               Log.Instance.WriteLog("InitNeedEnforceTableInfo failed, {0}", e.Message);
            }

            return true;
        }


        private DBInfo GetDBInfoByName(string strName)
        {
            foreach(DBInfo db in m_lstEnforceDBInfo)
            {
                if(db.Name.Equals(strName, StringComparison.OrdinalIgnoreCase))
                {
                    return db;
                }
            }
            return null;
        }

        private DBInfo AddDBInfo(string strName)
        {
            DBInfo old = GetDBInfoByName(strName);
            if(old==null)
            {
                DBInfo newDB = new DBInfo(strName);
                m_lstEnforceDBInfo.Add(newDB);
                return newDB;
            }
            return old;
        }

        public bool QueryColumnInfo()
        {
            foreach(DBInfo db in m_lstEnforceDBInfo)
            {
                db.Connect();
                db.QueryColumnInfo();

                if(!Program.g_UserInfoInited)
                {
                    db.GetUserInfoTableColumnInfo();
                    Program.g_UserInfoInited = true;
                }
                //db.DisConnect(); doesn't close the connection, because we will use the connection to set RLS policy
            }

            return true;
        }

        private string GetRLSBackupFile()
        {
            //create directory
            string strDir = CommonFun.GetDataFolder();
            if (!Directory.Exists(strDir))
            {
                Directory.CreateDirectory(strDir);
            }

            return strDir + "\\record.txt";
        }

        private RLSBackup ReadRlsBackUp(string strFilePath)
        {
            try
            {
                FileStream fs = new FileStream(strFilePath, FileMode.Open);

                DataContractJsonSerializer jsonSer = new DataContractJsonSerializer(typeof(RLSBackup));
                RLSBackup backup = (RLSBackup)jsonSer.ReadObject(fs);
                fs.Flush();
                fs.Close();

                return backup;
            }
            catch(Exception ex)
            {
                Log.Instance.WriteLog("ReadRlsBackUp exception:{0}\n", ex.Message);
                return new RLSBackup();
            }   
        }


        public bool DoEnforcement()
        {
            //read backup file, the backup file record which DB which table we have RLS funciton
            string strBackupFile = GetRLSBackupFile();
            RLSBackup rlsBackupOld = ReadRlsBackUp(strBackupFile);

            //create new backup object
            RLSBackup rlsBackupNew = new RLSBackup();

            //do enforcement
            foreach (DBInfo db in m_lstEnforceDBInfo)
            {
                db.DoEnforcement(rlsBackupOld, rlsBackupNew);
            }

            //remove RLS on DBs that is not be enforced
            RemoveNxlInfrastructureForDBs(rlsBackupOld);
            
            //write the backup file
            try
            {
                FileStream fs = new FileStream(strBackupFile, FileMode.Create);

                DataContractJsonSerializer jsonSer = new DataContractJsonSerializer(typeof(RLSBackup));
                jsonSer.WriteObject(fs, rlsBackupNew);

                fs.Flush();
                fs.Close();
            }
            catch(Exception ex)
            {
                Log.Instance.WriteLog("Exception on backup rls:{0}\n", ex.Message);
            }
    

            return true;
        }

        private void RemoveNxlInfrastructureForDBs(RLSBackup rlsBackup)
        {
            Dictionary<string, DBConnection> dicConn = new Dictionary<string, DBConnection>();

            foreach (RLSBackupTable backupTable in rlsBackup.RlsBackupTables)
            {
                Log.Instance.WriteLog("table:{0}.{1}.{2} is not being enforcered, clear all Infrastructure under it.\n", backupTable.m_strDBName, backupTable.m_strSchema, backupTable.m_strName);

                DBConnection dbConn = null;
                if (dicConn.ContainsKey(backupTable.m_strDBName))
                {
                    dbConn = dicConn[backupTable.m_strDBName];
                }
                else
                {
                    try
                    {
                        dbConn = new DBConnection();
                        dbConn.ConnectToSqlServer(Program.g_Config.RemoteSqlServer, Program.g_Config.RemoteSqlPort,
                           backupTable.m_strDBName, Program.g_Config.SQLAccount, Program.g_Config.SQLPwd);
                        dicConn[backupTable.m_strDBName] = dbConn;

                        RLS.ClearNxlInfrastructure(dbConn, true);
                    }
                    catch (Exception ex)
                    {
                        Log.Instance.WriteLog("Connect to server failed:{0}\n", ex.Message);
                        continue;
                    }

                }


               
            }

            //close connection
            foreach(KeyValuePair<string,DBConnection> dbConn in dicConn)
            {
                dbConn.Value.Close();
            }
        }
    }
}
