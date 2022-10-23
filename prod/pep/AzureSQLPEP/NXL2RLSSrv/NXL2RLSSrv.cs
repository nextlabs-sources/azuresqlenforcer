using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Linq;
using System.ServiceProcess;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using System.Reflection;
using System.Diagnostics;

namespace NXL2RLSSrv
{
    public partial class NXL2RLSSrv : ServiceBase
    {

        private int m_nConvertIntervalSecond = 24 * 60 * 60; //default one day
        private string m_strNXL2RLSFile;
        private System.Timers.Timer m_TimerNXL2RLS;

        private Process m_exeProcess;

        public NXL2RLSSrv()
        {
            InitializeComponent();
        }

        protected override void OnStart(string[] args)
        {
            Log.InitInstance();
            Log.Instance.WriteLog("OnStart.\n");

            //read interval from config file
            string strSrvDir = Path.GetDirectoryName(Assembly.GetExecutingAssembly().GetModules()[0].FullyQualifiedName);
            string strConfigFile = strSrvDir + "\\config\\config.ini";
            string strInterval = IniFileOperator.IniReadStringValue("Srv", "Interval", strConfigFile);
            try
            {
                int nInterval = int.Parse(strInterval);
                m_nConvertIntervalSecond = nInterval;
            }
            catch(Exception ex)
            {
                Log.Instance.WriteLog("Get Interval failed, default value is one day. ex:{0}\n", ex.Message);
            }
            Log.Instance.WriteLog("Interval:{0}second\n", m_nConvertIntervalSecond);


            //get nxl2rls.exe file name
            m_strNXL2RLSFile = strSrvDir + "\\NXL2RLS.exe";
            Log.Instance.WriteLog("EXE:{0}\n", m_strNXL2RLSFile);



            //set timer to execute policy convert
            m_TimerNXL2RLS = new System.Timers.Timer();
            m_TimerNXL2RLS.Interval = 5000;//first time to execute
            m_TimerNXL2RLS.Elapsed += new System.Timers.ElapsedEventHandler(TimerNXL2RLS_Elapsed);
            m_TimerNXL2RLS.Enabled = true;
        }

        protected override void OnStop()
        {
            m_TimerNXL2RLS.Enabled = false;
            
            Log.Instance.WriteLog("OnStop begin.\n");

            if(m_exeProcess!=null && (!m_exeProcess.HasExited))
            {
                Log.Instance.WriteLog("OnStop wait RLS convert finish.\n");
                m_exeProcess.WaitForExit();
            }

            Log.Instance.WriteLog("OnStop end.\n");
        }


        private void TimerNXL2RLS_Elapsed(object sender, System.Timers.ElapsedEventArgs e)
        {
            m_TimerNXL2RLS.Interval = m_nConvertIntervalSecond * 1000;

            if(m_exeProcess==null || m_exeProcess.HasExited)
            {
                if(m_exeProcess!=null)
                {
                    m_exeProcess.Close();
                    m_exeProcess = null;
                }
                //execute policy convert
                CallNxl2RLS(m_strNXL2RLSFile);
            }
            else
            {
                //we prevent two convert instance
                Log.Instance.WriteLog("the time is on to start RLS convert, but the previous convertion is not finished. the current interval:{0}\n", m_nConvertIntervalSecond);
            }
           
        }


        private void CallNxl2RLS(string strEXE)
        {
            Log.Instance.WriteLog("CallNxl2RLS begin...\n");

 
            //execute
            try
            {
                m_exeProcess = Process.Start(strEXE);
            }
            catch (Exception ex)
            {
                Log.Instance.WriteLog("Except on call NXL2RLS, ex:{0}\n", ex.Message);
            }

            Log.Instance.WriteLog("CallNxl2RLS end.\n");
        }
    }
}
