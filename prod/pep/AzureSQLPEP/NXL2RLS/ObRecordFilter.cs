using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using QueryCloudAZSDK;
using QueryCloudAZSDK.CEModel;

namespace NXL2RLS
{
    class ObRecordFilter : Obligation
    {
        private List<ObCondition> m_lstConditions = new List<ObCondition>();


        public ObRecordFilter(CEObligation ceOb)
        {
            m_strName = ceOb.Get_Nmae();

            //get conditions
            GetConditionsFromCEAttrs(m_lstConditions, ceOb.GetCEAttres());

        }

        public List<ObCondition> ListConditions
        {
            get { return m_lstConditions; }
        }

    }
}
