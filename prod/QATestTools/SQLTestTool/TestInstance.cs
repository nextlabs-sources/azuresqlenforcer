using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SQLTestTool
{
    class TestInstance
    {
        private string m_strServer;
        private string m_strDB;
        private string m_strUserName;
        private string m_strPasswd;
        private string m_strSql;
        private string m_strOutput;
        private int m_nRunTime = 0;

        public TestInstance(string strServer, string strDB, string strUserName, string strPasswd, string strSql, string strOutput,
            int nRumtime)
        {
            m_strServer = strServer;
            m_strDB = strDB;
            m_strUserName = strUserName;
            m_strPasswd = strPasswd;
            m_strSql = strSql;
            m_strOutput = strOutput;
            m_nRunTime = nRumtime; 
        }

        public int RunTimes {  get { return m_nRunTime; } }

        public string StrServer {  get { return m_strServer; } }
        public string StrDB {  get { return m_strDB; } }
        public string StrUserName {  get { return m_strUserName; } }

        public string StrPasswd {  get { return m_strPasswd; } }

        public string StrSqL {  get { return m_strSql; } }

        public string StrOutput {  get { return m_strOutput; } }
    }
}
