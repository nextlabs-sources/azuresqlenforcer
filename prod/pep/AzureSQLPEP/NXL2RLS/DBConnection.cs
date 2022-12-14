using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Data.SqlClient;
using System.Data;

namespace NXL2RLS
{
    enum SQL_LOGIN_TYPE
    {
        SQL_SERVER_AUTH,
        ACTIVE_DIRECTORY_PWD,
    }

    class DBConnection
    {
        private SqlConnection m_sqlConnect = null;


        public bool ConnectToSqlServer(string strRemoteServer, int nPort, string strDB, string strUserName, string strPasswd, SQL_LOGIN_TYPE loginType= SQL_LOGIN_TYPE.ACTIVE_DIRECTORY_PWD)
        {
            string strConnStr =  string.Format("Server=tcp:{0},{1};Initial Catalog={2};Persist Security Info=False;User ID={3};Password={4};MultipleActiveResultSets=False;Encrypt=True;TrustServerCertificate=False",
                strRemoteServer, nPort, strDB, strUserName, strPasswd);

            m_sqlConnect = new SqlConnection(strConnStr);

            try{
                m_sqlConnect.Open();
            }
            catch(Exception){
                return false;
            }

            return true;
        }

        public DataSet ExecuteWithResult(string strSql)
        {
            try
            {
                SqlDataAdapter sqlDa = new SqlDataAdapter(strSql, m_sqlConnect);
                DataSet dataSet = new DataSet();
                sqlDa.Fill(dataSet);

                return dataSet;
            }
            catch(Exception ex)
            {
                Log.Instance.WriteLog("Exception on ExecuteWithResult,sql:{0}, ex:{1}\n", strSql, ex.Message);
                return null;
            }
        }

        public bool ExecuteCommand(string strSql)
        {
            try
            {
                SqlCommand sqlCmd = new SqlCommand(strSql, m_sqlConnect);
                sqlCmd.ExecuteNonQuery();
                return true;
            }
            catch(Exception ex)
            {
                Log.Instance.WriteLog("Exception on ExecuteCommand,sql:{0}, ex:{1}\n", strSql, ex.Message);
                return false;
            }
        }

        public void Close()
        {
            if(m_sqlConnect!=null){
                m_sqlConnect.Close();
            }   
        }



    }
}
