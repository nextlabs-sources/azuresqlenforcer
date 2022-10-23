using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml;

namespace SQLTestTool
{
    class TestCfg
    {
        private List<TestInstance> m_lstInstance = new List<TestInstance>();


        public List<TestInstance> TestInstance
        {
            get { return m_lstInstance; }
        }

        public bool ReadTestConfig(string strCfgFile)
        {
            //read config
            XmlDocument doc = new XmlDocument();
            try
            {
                doc.Load(strCfgFile);

                XmlNodeList nodeList = doc.SelectNodes("//Instance");

                if(null==nodeList){
                    return false;
                }

                foreach(XmlNode InsNode in nodeList)
                {
                    string strServer = InsNode.Attributes["Server"].Value;
                    string strDB = InsNode.Attributes["Database"].Value;
                    string strUser = InsNode.Attributes["Username"].Value;
                    string strPasswd = InsNode.Attributes["Passwd"].Value;

                    string strSqlText = "";
                    string strOutput = "";
                    int nRunTime = 1;
                    XmlNode xmlTextNode = InsNode.SelectSingleNode("SQLText");
                    if(null!=xmlTextNode)
                    {
                        strSqlText = xmlTextNode.InnerText;
                        strOutput = xmlTextNode.Attributes["output"].Value;
                        nRunTime = int.Parse(xmlTextNode.Attributes["runtime"].Value);
                    }

                    if(string.IsNullOrWhiteSpace(strServer) || 
                        string.IsNullOrWhiteSpace(strDB) ||
                        string.IsNullOrWhiteSpace(strUser) ||
                        string.IsNullOrWhiteSpace(strPasswd) ||
                        string.IsNullOrWhiteSpace(strSqlText))
                    {
                        continue;
                    }

                    TestInstance testIns = new TestInstance(strServer, strDB, strUser, strPasswd, strSqlText, strOutput, nRunTime);
                    m_lstInstance.Add(testIns);
                }

            }
            catch(Exception ex)
            {
                Log.WriteLog("Read config file failed.ex={0}\n", ex.ToString());
            }


            return true;
        }

    }
}
