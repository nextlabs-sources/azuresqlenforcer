using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using QueryCloudAZSDK.CEModel;

namespace NXL2RLS
{
    class ObCondition
    {
        private string m_strValue1;
        private string m_strOperator;
        private string m_strValue2;
        private string m_strType;

        public ObCondition(string strValue1, string strOperator, string strValue2, string strType)
        {
            m_strValue1 = strValue1;
            m_strOperator = strOperator;
            m_strValue2 = strValue2;
            m_strType = strType;
        }


        public string Value1
        {
            get { return m_strValue1; }
        }

        public string Operator
        {
            get { return m_strOperator; }
        }

        public string Value2
        {
            get { return m_strValue2; }
        }


        public bool IsStringType()
        {
            return m_strType.Equals(CEAttributeType.XACML_String.ToString(), StringComparison.OrdinalIgnoreCase);
        }


    }
}
