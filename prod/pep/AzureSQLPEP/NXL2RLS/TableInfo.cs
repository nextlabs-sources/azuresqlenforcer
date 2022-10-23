using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace NXL2RLS
{
    class TableInfo
    {
        private string m_strSchema;
        private string m_strTableName;
        private List<ColumnInfo> m_lstColumns = new List<ColumnInfo>();
        private bool m_bValid= true;

        public string Name
        {
            get { return m_strTableName; }
        }

        public string Schema
        {
            get { return m_strSchema; }
        }

        public  List<ColumnInfo> Columns
        {
            get { return m_lstColumns; }
        }

        public TableInfo(string strName, string strSchema)
        {
            m_strTableName = strName;
            m_strSchema = strSchema;
            m_bValid = true;
        }

        public bool IsValid
        {
            get { return m_bValid; }
            set { m_bValid = value; }
        }



        public void AddedColumn(string strName, string strType)
        {
            m_lstColumns.Add(new ColumnInfo(strName, strType));
        }

        public ColumnInfo GetColumnByName(string strName)
        {
            foreach(ColumnInfo col in m_lstColumns)
            {
                if(col.Name.Equals(strName, StringComparison.OrdinalIgnoreCase))
                {
                    return col;
                }
            }

            return null;
        }

    }
}
