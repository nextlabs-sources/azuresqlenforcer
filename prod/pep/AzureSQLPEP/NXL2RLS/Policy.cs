using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace NXL2RLS
{
    class Policy
    {
        private string m_strName;
        private List<Obligation> m_lstObligations = new List<Obligation>();


        public Policy(string strName)
        {
            m_strName = strName ;
        }

        public string Name
        {
            get { return m_strName; }
        }


        public void AddObligation(Obligation ob)
        {
            m_lstObligations.Add(ob);
        }


        public ObUserFilter GetUserFilterObligation()
        {
            List<Obligation> lstObs = GetObligationsByName(QueryPolicy.m_strObNameUserFilter);
            if(null!=lstObs && lstObs.Count>0)
            {
                return lstObs[0] as ObUserFilter;
            }
            return null;
        }

        public List<Obligation> GetRecordFilterObligation()
        {
            List<Obligation> lstObs = GetObligationsByName(QueryPolicy.m_strObNameRecordFilter);
            return lstObs;
        }

        private List<Obligation> GetObligationsByName(string strName)
        {
            List<Obligation> lstObs = null;

            foreach(Obligation ob in m_lstObligations)
            {
                if(string.Compare(ob.Name, strName, StringComparison.OrdinalIgnoreCase)==0)
                {
                    if(null==lstObs)
                    {
                        lstObs = new List<Obligation>();
                    }

                    lstObs.Add(ob);
                }
            }


            return lstObs;
        }


        public static bool IsReferToResourceAttribute(string strAttr)
        {
            return strAttr.IndexOf("$from", StringComparison.OrdinalIgnoreCase)==0;
        }

        public static bool IsReferToUserAttribute(string strAttr)
        {
            return strAttr.IndexOf("$user", StringComparison.OrdinalIgnoreCase) == 0;
        }

        public static string GetReferToAttributeName(string strAttr)
        {
            int nPos = strAttr.IndexOf('.');
            if(nPos>0)
            {
                return strAttr.Substring(nPos + 1);
            }
            return strAttr;
        }




    }
}
