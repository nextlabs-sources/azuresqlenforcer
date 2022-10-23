using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using QueryCloudAZSDK;
using QueryCloudAZSDK.CEModel;

namespace NXL2RLS
{
    enum OBType
    {
       MaskField,
       UserFilter,
       RecordFilter,
    }

    class Obligation
    {
        protected string m_strName;

        public string Name
        {
            get { return m_strName; }
        }

        public void GetConditionsFromCEAttrs(List<ObCondition> lstConditions, CEAttres ceAttrs)
        {
            string strKey1, strValue1;
            string strKey2, strValue2;
            string strKey3, strValue3;
            CEAttributeType attrType;

            for (int iAttr = 1; iAttr < ceAttrs.get_count(); iAttr++)
            {
                ceAttrs.Get_Attre(iAttr, out strKey1, out strValue1, out attrType);

                iAttr++;
                if (iAttr >= ceAttrs.get_count())
                {
                    break;
                }
                ceAttrs.Get_Attre(iAttr, out strKey2, out strValue2, out attrType);

                iAttr++;
                if (iAttr >= ceAttrs.get_count())
                {
                    break;
                }
                ceAttrs.Get_Attre(iAttr, out strKey3, out strValue3, out attrType);

                //
                if (!string.IsNullOrWhiteSpace(strValue1) &&
                    !string.IsNullOrWhiteSpace(strValue2)/* &&
                    !string.IsNullOrWhiteSpace(strValue3)*/ /*don't check value3 for "is null" or "is not null" operator*/ )
                {
                    ObCondition obCon = new ObCondition(strValue1, strValue2, strValue3, attrType.ToString() );
                    lstConditions.Add(obCon);
                }
            }
        }


    }
}
