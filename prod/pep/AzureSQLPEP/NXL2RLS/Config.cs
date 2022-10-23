using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net;
using System.IO;
using System.Runtime.Serialization.Json;
using System.Runtime.Serialization;

namespace NXL2RLS
{
    [DataContract]
    public class SQLServerInfo
    {
        [DataMember(Name = "name")]
        public string m_strRemoteServer;

        public int m_nRemoteSqlPort;
    }

    [DataContract]
    class CommonSettingInfo
    {

        [DataMember(Name = "jpcHttps")]
        public bool m_bJPCHttps;

        [DataMember(Name = "jpcHost")]
        public string m_strJPCHost;

        [DataMember(Name = "jpcPort")]
        public string m_strJPCPort;

        [DataMember(Name = "ccHost")]
        public string m_strAuthHost;

        [DataMember(Name = "ccPort")]
        public string m_strAuthPort;

        [DataMember(Name = "clientId")]
        public string m_strClientID;

        [DataMember(Name = "clientKey")]
        public string m_strClientKey;
    }

    [DataContract]
    class SQLAccountInfo
    {
        [DataMember(Name = "user")]
        public string m_strName;

        [DataMember(Name = "password")]
        public string m_strPasswd;

        public string m_strUserType;
    }

    class Config
    {  
        //config server info
        private string m_strConfServerUrl;
        private string m_strConfUID;
        private string m_strConfProxyIns;

        //security by default
        private bool m_bSecurityByDefault;

        //Remote Server Info
        private SQLServerInfo m_RemoteSqlServerInfo;

        //SQL Account 
        private SQLAccountInfo m_SQLAccount;

        private string m_strUserInfoTableName;
        private string m_strUserInfoPriKey;

        //common set
        private CommonSettingInfo m_commonSet; 

        #region get/set


        public bool SecurityByDefault
        {
            get { return m_bSecurityByDefault; }
        }

        public string ConfigServerUrl
        {
            get { return m_strConfServerUrl; }
        }

        public string ConfigUID
        {
            get { return m_strConfUID; }
        }

        public string ConfigIns
        {
            get { return m_strConfProxyIns; }
        }

        public string RemoteSqlServer{
            get { return m_RemoteSqlServerInfo.m_strRemoteServer; }
        }

        public int RemoteSqlPort{
            get { return m_RemoteSqlServerInfo.m_nRemoteSqlPort; }
        }

        public string SQLAccount{
            get { return m_SQLAccount.m_strName; }
        }

        public string SQLPwd{
            get { return m_SQLAccount.m_strPasswd; }
        }

        public string AccountAuthType{
            get { return m_SQLAccount.m_strUserType; }
        }


        public string JPCHost{
            get { return (m_commonSet.m_bJPCHttps ? "https" : "http") + "://" + m_commonSet.m_strJPCHost + ":" + m_commonSet.m_strJPCPort; }
        }

        public string JPCOAuthHost{
            get { return "https://" + m_commonSet.m_strAuthHost + ":" + m_commonSet.m_strAuthPort; }
        }

        public string JPCClientID{
            get { return m_commonSet.m_strClientID; }
        }

        public string JPCClientSecret{
            get { return m_commonSet.m_strClientKey; }
        }

        public string UserInfoTableName
        {
            get { return m_strUserInfoTableName; }
        }

        public string UserInfoPriKey
        {
            get { return m_strUserInfoPriKey;  }
        }


        #endregion

        private bool GetSQLAccountInfo()
        {
            string strAccountInfoUrl = m_strConfServerUrl + (m_strConfServerUrl.EndsWith("/") ? "ext_api/account/" : ("/" + "ext_api/account/"));
            strAccountInfoUrl += m_strConfUID + "?proxy=" + m_strConfProxyIns;
            try
            {
                HttpWebRequest httpReq = (HttpWebRequest)HttpWebRequest.Create(strAccountInfoUrl);
                HttpWebResponse httpRes = (HttpWebResponse)httpReq.GetResponse();
                DataContractJsonSerializer jsonSer = new DataContractJsonSerializer(typeof(SQLAccountInfo));
                m_SQLAccount = (SQLAccountInfo)jsonSer.ReadObject(httpRes.GetResponseStream());
                httpRes.Close();
            }
            catch (Exception ex)
            {
                Log.Instance.WriteLog("Exception on GetSQLAccountInfo:{0}\n", ex.Message);
                return false;
            }

            return true;
        }


        private bool GetRemoteServerInfo()
        {
            //get remote server info
            string strServerInfoUrl = m_strConfServerUrl + (m_strConfServerUrl.EndsWith("/") ? "ext_api/server/" : ("/" + "ext_api/server/"));
            strServerInfoUrl += m_strConfUID + "?proxy=" + m_strConfProxyIns;
            try
            {
                HttpWebRequest httpReq = (HttpWebRequest)HttpWebRequest.Create(strServerInfoUrl);
                HttpWebResponse httpRes = (HttpWebResponse)httpReq.GetResponse();
                DataContractJsonSerializer jsonSer = new DataContractJsonSerializer(typeof(SQLServerInfo));
                m_RemoteSqlServerInfo = (SQLServerInfo)jsonSer.ReadObject(httpRes.GetResponseStream());

                int nPos = m_RemoteSqlServerInfo.m_strRemoteServer.LastIndexOf(':');
                if (nPos > 0)
                {
                    m_RemoteSqlServerInfo.m_nRemoteSqlPort = int.Parse(m_RemoteSqlServerInfo.m_strRemoteServer.Substring(nPos + 1));
                    m_RemoteSqlServerInfo.m_strRemoteServer = m_RemoteSqlServerInfo.m_strRemoteServer.Substring(0, nPos);
                }

                httpRes.Close();
            }
            catch (Exception ex)
            {
                Log.Instance.WriteLog("Exception on GetRemoteServerInfo:{0}\n", ex.Message);
                return false;
            }

            return true;
 
        }

        private bool GetCommonSetingInfo()
        {
            //get remote server info
            string strSetingInfoUrl = m_strConfServerUrl + (m_strConfServerUrl.EndsWith("/") ? "ext_api/setting/" : ("/" + "ext_api/setting/"));
            strSetingInfoUrl += m_strConfUID + "?proxy=" + m_strConfProxyIns;
            try
            {
                HttpWebRequest httpReq = (HttpWebRequest)HttpWebRequest.Create(strSetingInfoUrl);
                HttpWebResponse httpRes = (HttpWebResponse)httpReq.GetResponse();
                DataContractJsonSerializer jsonSer = new DataContractJsonSerializer(typeof(CommonSettingInfo));
                m_commonSet = (CommonSettingInfo)jsonSer.ReadObject(httpRes.GetResponseStream());
                httpRes.Close();
            }
            catch (Exception ex)
            {
                Log.Instance.WriteLog("Exception on GetCommonSetingInfo:{0}\n", ex.Message);
                return false;
            }

            return true;
        }

        private void GetConfigServerInfo(string strConfigFile)
        {
            string strSec = "ConfigServerInfo";
            m_strConfServerUrl = IniFileOperator.IniReadStringValue(strSec, "url", strConfigFile);
            m_strConfUID = IniFileOperator.IniReadStringValue(strSec, "uid", strConfigFile);
            m_strConfProxyIns = IniFileOperator.IniReadStringValue(strSec, "proxyinstance", strConfigFile);
        }

        public void Print()
        {
            Log.Instance.WriteLog("config server, url:{0}, uid:{1}, instance:{2}\n", ConfigServerUrl, ConfigUID, ConfigIns);
            Log.Instance.WriteLog("RemoteServerInfo, server:{0}, port:{1}\n", RemoteSqlServer, RemoteSqlPort);
            Log.Instance.WriteLog("jpchost:{0} AuthHost:{1}, clientID:{2}\n", JPCHost, JPCOAuthHost, JPCClientID);
            Log.Instance.WriteLog("user info table:{0}, key={1}\n", UserInfoTableName, UserInfoPriKey);
            Log.Instance.WriteLog("SecurityByDefault:{0}\n", SecurityByDefault);
        }


        public bool Init()
        {
            string strConfigFile = CommonFun.ApplicationDir() + "\\Config\\config.ini";

            //read config server info
            GetConfigServerInfo(strConfigFile);

            //get remote server info
            if (!GetRemoteServerInfo())
            {
                return false;
            }

            //get other settings
            if(!GetCommonSetingInfo())
            {
                return false;
            }

            //get sql account to query user info from database
            if(!GetSQLAccountInfo())
            {
                return false;
            }

            //get security by default
            m_bSecurityByDefault = true;
            string strSecurityByDefault = IniFileOperator.IniReadStringValue("Common", "SecurityByDefault", strConfigFile);
            m_bSecurityByDefault = !strSecurityByDefault.Equals("no", StringComparison.OrdinalIgnoreCase);

            //test config
            //m_RemoteSqlServerInfo.m_strRemoteServer = "sqltest002.database.windows.net";
            //m_RemoteSqlServerInfo.m_nRemoteSqlPort =1433;

            //m_SQLAccount.m_strName = "jxu@cloudazdev.onmicrosoft.com";
            //m_SQLAccount.m_strPasswd = "123blue!";

            m_strUserInfoTableName = "nxl_users";
            m_strUserInfoPriKey = "userPrincipalName";


            //m_commonSet.m_strJPCHost= "azure-dev-cc";
            //m_commonSet.m_strJPCPort = "58080";
            //m_commonSet.m_bJPCHttps = false;
            //m_commonSet.m_strAuthHost = "azure-dev-cc";
            //m_commonSet.m_strAuthPort = "443";
            //m_commonSet.m_strClientID = "apiclient";
            //m_commonSet.m_strClientKey = "123blue!";

            //m_strJPCHost = "https://sfjpc1.crm.nextlabs.solutions";
            //m_strOAuthHost = "https://sfcc1.crm.nextlabs.solutions";
            //m_strClientID = "apiclient";
            //m_strClientSecret = "123blue!";

            return true;
        }
    }
}
