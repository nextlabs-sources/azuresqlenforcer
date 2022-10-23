using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using System.Diagnostics;

namespace NXL2RLSSrv
{
    class Log
    {
        private string m_strLogFilePath;

        private StreamWriter m_LogWriter;

        private static Log m_theLogInstance = null;

        private Log() { }
        private Log(Log l) {}


        public static Log Instance
        {
            get
            {
                if (m_theLogInstance == null)
                {
                    m_theLogInstance = new Log();
                }

                return m_theLogInstance;
            }
        }


        public static bool InitInstance()
        {
            return Instance.Init();
        }

        public void WriteLog(string strFmt, params object[] args)
        {
            m_LogWriter.Write(GetCurrentTimeString() + ":" + string.Format(strFmt, args));
        }

        public void Close()
        {
            m_LogWriter.Close();
        }

        private bool Init()
        {
            //get log dir 
            string strLogDir = Environment.GetFolderPath(Environment.SpecialFolder.CommonApplicationData);
            strLogDir += "\\Nextlabs\\AzureSqlEnforcer\\NXL2RLS\\log";

            if(!Directory.Exists(strLogDir))
            {
                Directory.CreateDirectory(strLogDir);
            }

            //clear log
            ClearEarlyLog(strLogDir);

            //caculate log file name
            string strLogFile = strLogDir + "\\Srv" + GetCurrentTimeString() + ".txt";

            //open log file
            m_LogWriter = new StreamWriter(strLogFile);
            m_LogWriter.AutoFlush = true;

            return true;
        }

        public string GetCurrentTimeString()
        {
            DateTime dt = DateTime.Now;
            return GetTimeString(dt);
        }

        public string GetTimeString(DateTime dt)
        {
            return dt.ToString("yyyyMMdd-HHmmss");
        }

        private void ClearEarlyLog(string strLogDir)
        {
            DateTime dtNow = DateTime.Now;
            TimeSpan timeSpan = new TimeSpan(24 * 7, 0, 0);
            DateTime dtLast = dtNow - timeSpan;

            string strLogLast = "Srv" + GetTimeString(dtLast) + ".txt";

            DirectoryInfo dirInfo = new DirectoryInfo(strLogDir);
            foreach (FileInfo fileInfo in dirInfo.GetFiles("Srv*.txt"))
            {
                if (fileInfo.Name.CompareTo(strLogLast) < 0)
                {
                    Trace.WriteLine("Delete file:" + fileInfo.FullName);
                    fileInfo.Delete();
                }
            }
        }

    }
}
