using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;


namespace NXL2RLS
{
    class Program
    {
        static public Config g_Config = new Config();
        static public EnforceDBManager g_EnforceDBMgr = new EnforceDBManager();
        static public QueryPolicy g_QueryPolicy = new QueryPolicy();

        static public TableInfo g_UserInfoTable;
        static public bool g_UserInfoInited = false;
     

        static void Main(string[] args)
        {
            //log init
            Log.InitInstance();
            Log.Instance.WriteLog("NXL2RLS start.\n");

            //get config info.
            if(!g_Config.Init())
            {
                Log.Instance.WriteLog("Init config failed.\n");
                return;
            }
            g_Config.Print();


            g_UserInfoTable = new TableInfo(g_Config.UserInfoTableName, "dbo");

         
            //get tables that we need to do enforcement
            g_EnforceDBMgr.InitNeedEnforceTableInfo();

            //get column name for the table
            g_EnforceDBMgr.QueryColumnInfo();

            //connect to JPC
            bool bConnectPC = g_QueryPolicy.ConnectToServer(g_Config.JPCHost, g_Config.JPCOAuthHost, g_Config.JPCClientID, g_Config.JPCClientSecret);
            if(bConnectPC)
            {
                Log.Instance.WriteLog("Connect to PC success,begin policy convert.\n");

                //convert nxl policy to RLS policy
                g_EnforceDBMgr.DoEnforcement();

                Log.Instance.WriteLog("Finished to convert RLS.\n");
            }
            else
            {
                Log.Instance.WriteLog("Connect to PC failed, no RLS been changed.\n");
            }
        }



    }
}
