using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace NXL2RLS
{
    class ColumnInfo
    {
        private string m_strColName;
        private string m_strType;

        public ColumnInfo(string colName, string dataType)
        {
            m_strColName = colName;
            m_strType = dataType;
        }

        public string Name
        {
            get { return m_strColName; }
        }

        public string Type
        {
            get { return m_strType; }
        }

        public bool IsStringType()
        {
            return m_strType.Contains("char") || 
                m_strType.Equals("sysname", StringComparison.OrdinalIgnoreCase) ||
                m_strType.Contains("text");
        }
    }
}
