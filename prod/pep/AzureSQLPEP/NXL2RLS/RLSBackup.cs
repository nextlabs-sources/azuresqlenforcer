using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.Serialization.Json;
using System.Runtime.Serialization;

namespace NXL2RLS
{
    [DataContract]
    class RLSBackupTable
    {
        [DataMember(Name = "DB")]
        public string m_strDBName;
        [DataMember(Name = "Schema")]
        public string m_strSchema;
        [DataMember(Name = "name")]
        public string m_strName;
        [DataMember(Name = "ViewFun")]
        public string m_strViewFun;
        [DataMember(Name = "CreateFun")]
        public string m_strCreateFun;
        [DataMember(Name = "EditFun")]
        public string m_strEditFun;
        [DataMember(Name = "DeleteFun")]
        public string m_strDeleteFun;
        [DataMember(Name = "SecurityPolicy")]
        public string m_strSecurityPolicy;



        public void AddedFunction(string strAction, string strFun)
        {
            if (string.Equals(strAction, QueryPolicy.m_strActionQuery, StringComparison.OrdinalIgnoreCase))
            {
                m_strViewFun = strFun;
            }
            else if (string.Equals(strAction, QueryPolicy.m_strActionUpdate, StringComparison.OrdinalIgnoreCase))
            {
                m_strEditFun = strFun;
            }
            else if (string.Equals(strAction, QueryPolicy.m_strActionInsert, StringComparison.OrdinalIgnoreCase))
            {
                 m_strCreateFun = strFun;
            }
            else if (string.Equals(strAction, QueryPolicy.m_strActionDelete, StringComparison.OrdinalIgnoreCase))
            {
                 m_strDeleteFun = strFun;
            }
        }

        public string FindFunction(string strAction)
        {
            if(string.Equals(strAction, QueryPolicy.m_strActionQuery, StringComparison.OrdinalIgnoreCase))
            {
                return m_strViewFun;
            }
            else if(string.Equals(strAction, QueryPolicy.m_strActionUpdate, StringComparison.OrdinalIgnoreCase))
            {
                return m_strEditFun;
            }
            else if(string.Equals(strAction, QueryPolicy.m_strActionInsert, StringComparison.OrdinalIgnoreCase))
            {
                return m_strCreateFun;
            }
            else if(string.Equals(strAction, QueryPolicy.m_strActionDelete, StringComparison.OrdinalIgnoreCase))
            {
                return m_strDeleteFun;
            }
            else
            {
                return "";
            }
        }


    };

    [DataContract]
    class RLSBackup
    {
        [DataMember(Name = "tables")]
        List<RLSBackupTable> m_lstTable = new List<RLSBackupTable>();

        public List<RLSBackupTable> RlsBackupTables
        {
            get { return m_lstTable; }
        }

        public string FindFunction(string strDB, string strSchema, string strTable, string strAction)
        {
            RLSBackupTable backupTable = FindBackupTable(strDB, strSchema, strTable);
            if(backupTable!=null)
            {
                return backupTable.FindFunction(strAction);
            }
            return "";
        }


        public void AddedSecurityPolicy(string strDB, string strSchema, string strTable, string strPolicy)
        {
            RLSBackupTable backupTable = FindBackupTable(strDB, strSchema, strTable);

            if (backupTable == null)
            {
                backupTable = AddedBackupTable(strDB, strSchema, strTable);
            }

            if (backupTable != null)
            {
                backupTable.m_strSecurityPolicy = strPolicy;
            }
        }

        public void AddedFunction(string strDB, string strSchema, string strTable, string strAction, string strFun)
        {
            RLSBackupTable backupTable = FindBackupTable(strDB, strSchema, strTable);

            if(backupTable==null)
            {
                backupTable = AddedBackupTable(strDB, strSchema, strTable);
            }

            if (backupTable != null)
            {
                backupTable.AddedFunction(strAction, strFun);
            }
        }

        public void RemoveBackupTable(string strDB, string strSchema, string strTable)
        {
            RLSBackupTable backup = FindBackupTable(strDB, strSchema, strTable);
            m_lstTable.Remove(backup);
        }

        private RLSBackupTable AddedBackupTable(string strDB, string strSchema, string strTable)
        {
            RLSBackupTable backupTable = new RLSBackupTable()
            {
                m_strDBName = strDB,
                m_strSchema = strSchema,
                m_strName = strTable
            };

            m_lstTable.Add(backupTable);

            return backupTable;
        }


        public List<RLSBackupTable> FindBackupTablesByDB(string strDB)
        {
            List<RLSBackupTable> lstTables = new List<RLSBackupTable>();

            foreach (RLSBackupTable rlsTable in m_lstTable)
            {
                if (string.Equals(strDB, rlsTable.m_strDBName, StringComparison.OrdinalIgnoreCase))
                {
                    lstTables.Add(rlsTable);
                }
            }

            return lstTables;
        }


        private RLSBackupTable FindBackupTable(string strDB, string strSchema, string strTable)
        {
            foreach(RLSBackupTable rlsTable in m_lstTable)
            {
                if(string.Equals(strDB, rlsTable.m_strDBName, StringComparison.OrdinalIgnoreCase) &&
                    string.Equals(strSchema, rlsTable.m_strSchema, StringComparison.OrdinalIgnoreCase) &&
                    string.Equals(strTable, rlsTable.m_strName, StringComparison.OrdinalIgnoreCase) )
                {
                    return rlsTable;
                }
            }

            return null;
        }
    }
}
