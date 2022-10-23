using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Threading;

namespace SQLTestTool
{
    public partial class FormMain : Form
    {
        private TestCfg m_TestCfg = new TestCfg();
        private List<TestExecuter> m_lstExecuter = new List<TestExecuter>();
 
        public FormMain()
        {
            InitializeComponent();

            Log.Init(m_textBoxLog);

            m_textBoxCfgFile.Text = CommonFun.ApplicationDir() + "\\config\\conf.xml";

        }

        public void ExecuteTest()
        {
           //connect to server
           //we must connect server one by one or it will failed for sometime.
            foreach(TestInstance testIns in m_TestCfg.TestInstance)
            {
                TestExecuter executer = new TestExecuter(testIns);
                executer.Connect();

                m_lstExecuter.Add(executer);

                Thread.Sleep(1000);
            }

            //execute
            foreach(TestExecuter executer in m_lstExecuter)
            {
                Thread thExecute = new Thread(executer.Execute);
                executer.ExecuteThread = thExecute;
                thExecute.Start();
            }
        }

        private void button_start_Click(object sender, EventArgs e)
        {
            button_start.Enabled = false;

            m_TestCfg.ReadTestConfig(m_textBoxCfgFile.Text);
            ExecuteTest();  
        }

        private void FormMain_FormClosed(object sender, FormClosedEventArgs e)
        {
            //terminate all thread
            foreach (TestExecuter executer in m_lstExecuter)
            {
                executer.ExecuteThread.Abort();
                executer.ExecuteThread = null;
            }
        }
    }
}
