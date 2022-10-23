using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Data;
using System.IO;
using System.Diagnostics;
using System.Threading;

namespace SQLTestTool
{
    class TestExecuter
    {
        private TestInstance m_testInstance;
        private DBConnection m_dbConn = new DBConnection();
        private Thread m_Thread;
        private bool m_bConnect;

        public TestExecuter(TestInstance testIns)
        {
            m_testInstance = testIns;
            m_bConnect = false;
        }

        public Thread ExecuteThread
        {
            get { return m_Thread; }
            set { m_Thread = value;  }
        }

        public void Connect()
        {
            //connect to database
             m_bConnect = m_dbConn.ConnectToSqlServer(m_testInstance.StrServer, 1433, m_testInstance.StrDB, m_testInstance.StrUserName, m_testInstance.StrPasswd);
            if (!m_bConnect)
            {
                Log.WriteLog("Connect to SQL Server failed. Server:{0}, DB：{1}\r\n", m_testInstance.StrServer, m_testInstance.StrDB);
                return;
            }
            else
            {
                Log.WriteLog("Connect to SQL Server success. Server:{0}, DB：{1}\r\n", m_testInstance.StrServer, m_testInstance.StrDB);
            }
        }

        public void Execute()
        {
            if(!m_bConnect)
            {
                return;
            }

            //open output file
            FileStream fs = null;
            StreamWriter sw = null;
            try
            {
                fs = new FileStream(m_testInstance.StrOutput, FileMode.Create);
                sw = new StreamWriter(fs);
                sw.AutoFlush = true;
            }
            catch (Exception ex)
            {
                Log.WriteLog("Open output file failed.ex={0}\r\n", ex.ToString());
                return;
            }

            //run
            int nRunTime = 0;
            while(true)
            {
                if(m_testInstance.RunTimes!=0 && nRunTime>m_testInstance.RunTimes)
                {
                    break;
                }
                nRunTime++;

                //Excute
                Stopwatch stopwatch = new Stopwatch();
                stopwatch.Start();

                DataSet dataSet = m_dbConn.ExecuteWithResult(m_testInstance.StrSqL);

                stopwatch.Stop();
                TimeSpan timespan = stopwatch.Elapsed;


                if (dataSet != null)
                {
                    Log.WriteLog("Execute SQL:{0}({1})\r\n", m_testInstance.StrSqL, timespan.TotalMilliseconds);

                    //write sql text and time span
                    sw.WriteLine("SQL:" + m_testInstance.StrSqL);
                    sw.WriteLine(string.Format("Time:{0}ms", timespan.TotalMilliseconds));

                    if(m_testInstance.RunTimes==0 || m_testInstance.RunTimes>100)
                    {
                        System.Threading.Thread.Sleep(1 * 1000);
                        continue;
                    }

                    sw.WriteLine("Result:");
                    
                    foreach (DataTable dt in dataSet.Tables)
                    {
                        //output column name
                        string strCol = "";
                        foreach (DataColumn col in dt.Columns)
                        {
                            strCol += string.IsNullOrWhiteSpace(strCol) ? "" : ",";
                            strCol += col.ColumnName;
                        }
                        sw.WriteLine(strCol);

                        //output content
                        foreach (DataRow row in dt.Rows)
                        {
                            string strContent = "";

                            foreach (object colValue in row.ItemArray)
                            {
                                strContent += string.IsNullOrWhiteSpace(strContent) ? "" : ",";
                                strContent += colValue.ToString();
                            }

                            sw.WriteLine(strContent);
                        }

                        sw.WriteLine(",");
                    }
                }
                else
                {
                    Log.WriteLog("have not data for sql:{0}\r\n", m_testInstance.StrSqL);
                }
                
            }

            sw.Close();
            fs.Close();
        }
    }
}
