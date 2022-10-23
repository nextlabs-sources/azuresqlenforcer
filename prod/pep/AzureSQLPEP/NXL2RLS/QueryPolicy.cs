using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using QueryCloudAZSDK;
using QueryCloudAZSDK.CEModel;

namespace NXL2RLS
{

    class QueryPolicyResult
    {
        public PolicyResult emPolicyResult;
        public List<Policy> lstPolicy;

        public bool IsAllow() { return emPolicyResult == PolicyResult.Allow; }
        public bool IsDeny() { return emPolicyResult == PolicyResult.Deny; }
        public bool IsDontCare() { return emPolicyResult == PolicyResult.DontCare; }

    }

    class QueryPolicy
    {
        CEQuery m_PolicyQuery = null;

        #region Action define
        static public readonly string m_strActionQuery = "VIEW";
        static public readonly string m_strActionInsert = "CREATE";
        static public readonly string m_strActionUpdate = "EDIT";
        static public readonly string m_strActionDelete = "DELETE";
        #endregion

        #region resource attribute name
        private readonly string m_strResourceAttrServerName = "sqlserver";
        private readonly string m_strResourceAttrDataBase = "database";
        private readonly string m_strResourceAttrTable = "table";
        #endregion

        #region obligation name 
        public static readonly string m_strObNameUserFilter = "apply_user_filter";
        public static readonly string m_strObNameRecordFilter = "apply_security_filter";
        public static readonly string m_strObNameColumnMask = "mask_field";
        #endregion

        #region obligation field name
        private readonly string m_stgrObFieldNamePolicyName = "policy_name";
        #endregion



        public bool ConnectToServer(string JPCHost, string OAuthHost, string ClientID, string ClientSecret)
        {
            m_PolicyQuery = new CEQuery(DataType.Json, JPCHost, OAuthHost, ClientID, ClientSecret, true);

            return QueryStatus.S_OK == m_PolicyQuery.Authenticated;
        }

        public QueryPolicyResult QueryPolicyInfo(string strAction, string strSqlSrv, string strdb, string strTable)
        {
            // Prepare request info for query policy
            CERequest obCERequest = new CERequest();

            obCERequest.Set_Action(strAction);

            // Set user info, this is mandatory
            {
                CEAttres obCEUserAttres = new CEAttres();
                obCERequest.Set_User("S-1-5-21-310440588-250036847-580389505-500" , "nxl2rls@domain.com" , obCEUserAttres);
            }


            // Set source info, this is mandatory
            {
               CEAttres obCESourceAttres = new CEAttres();
               obCESourceAttres.Add_Attre(m_strResourceAttrServerName, strSqlSrv, CEAttributeType.XACML_String);
               obCESourceAttres.Add_Attre(m_strResourceAttrDataBase , strdb, CEAttributeType.XACML_String);
               obCESourceAttres.Add_Attre(m_strResourceAttrTable, strTable, CEAttributeType.XACML_String);
               obCERequest.Set_Source("C:/Temp/Source.txt", "1", obCESourceAttres);
            }


            // Set application info, this is mandatory
            {
                obCERequest.Set_App(CommonFun.ApplicationName() , CommonFun.ApplicationFullPath(), null, null);
            }

           

            // This method can set NameAttributes (Environmental)
            {
                obCERequest.Set_NameAttributes("current-dateTime", "1999-05-31T13:20:00-05:00", CEAttributeType.XACML_DateTime);
                obCERequest.Set_NameAttributes("dont-care-acceptable", "yes", CEAttributeType.XACML_String);
                obCERequest.Set_NameAttributes("envirAttr", "This is EnvirAttr", CEAttributeType.XACML_AnyURI);
            }

            // this method can set host 
            {
                obCERequest.Set_Host("HostName", "10.23.60.12", null);
            }



            // Query policy and get the result
            PolicyResult emPolicyResult = PolicyResult.DontCare;
            List<CEObligation> lsObligation = null;
            DateTime dtBefore = DateTime.Now;
            QueryStatus emQueryStatus = m_PolicyQuery.CheckResource(obCERequest, out emPolicyResult, out lsObligation);


            if (emQueryStatus == QueryStatus.S_OK)
            {
                Log.Instance.WriteLog("Query policy success for action:{0}, server:{1}, db:{2}, table:{3}, enforcement:{4}, obCount:{5}\n", strAction, strSqlSrv, strdb, strTable, emPolicyResult, lsObligation==null?0:lsObligation.Count);

                QueryPolicyResult queryPolicyResult = new QueryPolicyResult();
                queryPolicyResult.emPolicyResult = emPolicyResult;

                if (lsObligation != null && lsObligation.Count > 0)
                {
                    PrintObligations(lsObligation);
                    queryPolicyResult.lstPolicy = GroupObligationByPolicyName(lsObligation);
                }

                return queryPolicyResult;
            }
            else
            {
                Log.Instance.WriteLog("Query policy failed for action:{0}, server:{1}, db:{2}, table:{3}\n", strAction, strSqlSrv, strdb, strTable); 
            }


            return null;

        }

        private void PrintObligations(List<CEObligation> lstObs)
        {
            foreach (CEObligation ceob in lstObs)
            {
                Log.Instance.WriteLog("Obligatin:{0}\n", ceob.Get_Nmae());

                CEAttres attrs = ceob.GetCEAttres();
                for (int i = 0; i < attrs.get_count(); i++)
                {
                    string strName, strValue;
                    CEAttributeType attrType;
                    attrs.Get_Attre(i, out strName, out strValue, out attrType);

                    Log.Instance.WriteLog("{0}：{1},{2}\n", strName, strValue, attrType);
                }
            }
        }


        private List<Policy> GroupObligationByPolicyName(List<CEObligation> lstObligation)
        {
            List<Policy> lstPolicy = new List<Policy>();

            Dictionary<string, Policy> dicPolicy = new Dictionary<string, Policy>();
            Policy curPolicy = null;

            foreach(CEObligation ceOb in  lstObligation)
            {
                //find policy name
                string strPolicyName = GetPolicyNameFromObligationAttr(ceOb.GetCEAttres());
                if(string.IsNullOrWhiteSpace(strPolicyName))
                {
                    Log.Instance.WriteLog("Can't find policy name for:{0}\n", ceOb.Get_Nmae());
                    continue;
                }

                //create obligation
                Obligation ob = CreateOblitgation(ceOb);
                if (ob == null)
                {
                    continue;
                }

                //find policy by name
                if (dicPolicy.ContainsKey(strPolicyName))
                {
                    curPolicy = dicPolicy[strPolicyName];
                }
                else
                {
                    curPolicy = new Policy(strPolicyName);
                    dicPolicy[strPolicyName] = curPolicy;
                    lstPolicy.Add(curPolicy);
                }
                
                //add obligation to policy
                curPolicy.AddObligation(ob);
            }

            return lstPolicy;
        }


        private string GetPolicyNameFromObligationAttr(CEAttres attres)
        {
            string strKey, strValue;
            CEAttributeType attrType;
            for(int i=0; i<attres.get_count(); i++)
            {
                attres.Get_Attre(i, out strKey, out strValue, out attrType);
                if(String.Compare(strKey, m_stgrObFieldNamePolicyName, StringComparison.OrdinalIgnoreCase)==0)
                {
                    return strValue.ToUpper();
                }
            }
            return "";
        }



        private Obligation CreateOblitgation(CEObligation ceOb)
        {
            Obligation ob = null;

            if (string.Compare(ceOb.Get_Nmae(), m_strObNameUserFilter, StringComparison.OrdinalIgnoreCase) == 0)
            {
                ob = new ObUserFilter(ceOb);
            }
            else if(string.Compare(ceOb.Get_Nmae(), m_strObNameRecordFilter, StringComparison.OrdinalIgnoreCase) == 0)
            {
                ob = new ObRecordFilter(ceOb);
            }
            else if(string.Compare(ceOb.Get_Nmae(), m_strObNameColumnMask, StringComparison.OrdinalIgnoreCase) == 0)
            {
                // ob = new ObMaskField(ceOb);
                Log.Instance.WriteLog("ignore mask obligation：{0}\n", ceOb.Get_Nmae());
            }
            else
            {
                //unsupport obligation
                Log.Instance.WriteLog("Unsupport obligation:{0}\n", ceOb.Get_Nmae());
            }

            return ob;
        }
    }
}
