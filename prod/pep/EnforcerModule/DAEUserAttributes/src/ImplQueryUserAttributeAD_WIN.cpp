#include <windows.h>
#include "log.h"
#include "ImplQueryUserAttributeAD.h"
#include "userattr_mgr.h"
#include "commfun.h"
#include <ole2.h>
#include <math.h>
#include <wchar.h>
#include <objbase.h>
#include <activeds.h>
#include <atlbase.h>

//  Ensure that you define UNICODE
//  Define version 5 for Windows 2000
#define _WIN32_WINNT 0x0500
//  For SID conversion API.
#include <sddl.h>


void FORMAT_VALUE(std::wstring& value, BSTR item, int i, const std::wstring& sep) {
    if (i == 0) {
        value = item;
    }
    else {
        value += sep;
        value += item;
    }
}

class ImplQueryUserAttributeAD : public IQueryUserAttribute {
public:
    ImplQueryUserAttributeAD(const char *ldap, const char *u_name, const char *pwd);
    virtual bool QueryUserAttribute(UserAttr* pUser) override;
private:
    bool ad_user_name_convert(const std::string& src, std::string& dst);
    std::wstring m_ldap;
    std::wstring m_user_name;
    std::wstring m_pwd;
    IDirectorySearch *m_obj;
    bool m_inited;
};



HRESULT EnumeratePropertyValue(IADsPropertyEntry *pEntry, std::wstring& value);
HRESULT GetUserProperties(IADs *pObj, UserAttr *pUser);
HRESULT FindUserByName(IDirectorySearch *pSearchBase, //  Container to search.
	LPWSTR szLoginName,
	LPWSTR szPwd,
	LPOLESTR szFindUser, //  Name of user to find.
	IADs **ppUser); //  Return a pointer to the user.

ImplQueryUserAttributeAD::ImplQueryUserAttributeAD(const char *ldap, const char *u_name, const char *pwd) : 
     m_obj(nullptr), m_inited(false) {
	CommonFun::FromUTF8(ldap, m_ldap);
	CommonFun::FromUTF8(pwd, m_pwd);
	CommonFun::FromUTF8(u_name, m_user_name);
}

bool GetLocalUserNameAddIP()
{
    char buf[256]="";
    WSADATA w;
    WSAStartup(0x0101, &w);
    struct hostent *ph = 0;
    gethostname(buf, 256);
    std::string hostNmae = buf;

    ph = gethostbyname(buf);
    const char *IP =inet_ntoa(*((struct in_addr *)ph->h_addr_list[0]));
    std::string strIp =IP;
    WSACleanup();
    return true;
}

bool ImplQueryUserAttributeAD::QueryUserAttribute(UserAttr* pUser) {
	Log::WriteLog(log_trace, "ENTER ImplQueryUserAttributeAD::QueryUserAttribute");

	GetLocalUserNameAddIP();

	CoInitialize(NULL);

    HRESULT hr = ADsOpenObject(m_ldap.c_str(),
    m_user_name.c_str(),
    m_pwd.c_str(),
    ADS_SECURE_AUTHENTICATION,
    IID_IDirectorySearch,
    (void**)&m_obj);

	if (SUCCEEDED(hr) && (m_obj != NULL)) {
        m_inited = true;
	}

    if (!m_inited)
        return false;
    IADs* pUser_ads = NULL;
    std::string searchA = "";
    bool ret = ad_user_name_convert(pUser->GetUserNameAttr(), searchA);
    if (!ret)
        return false;
    std::wstring serachW ;
	CommonFun::FromUTF8(searchA, serachW);
    FindUserByName(m_obj, (LPWSTR)m_user_name.c_str(), (LPWSTR)m_pwd.c_str(), (LPWSTR)serachW.c_str(), &pUser_ads);
    if (pUser_ads) {
		Log::WriteLog(log_trace, "FindUserByName FOUND %s", searchA.c_str());
        GetUserProperties(pUser_ads, pUser);
	}
	else {
		Log::WriteLog(log_error, "FindUserByName NOT FOUND %s", searchA.c_str());
		return false;
	}
	return true;
}

bool ImplQueryUserAttributeAD::ad_user_name_convert(const std::string& src1, std::string& dst) {
    auto src = src1.c_str();
    size_t len = src1.length();
	size_t i = 0;
    std::string domain = "", name = "";
    while (i < len) {
        size_t s = i;
        while (i < len && src[i] != '\\') 
            i++;
        if (i >= len)
            return false;
        domain = std::string(src+s, src+i);
        ++i;
        if (i >= len)
            return false;
        name = std::string(src+i, src+len);
        break;
    }
    dst = name + "@" + domain + "*";
    return true;
}


HRESULT EnumeratePropertyValue(IADsPropertyEntry *pEntry, std::wstring& value)
{
	HRESULT hr = E_FAIL;
	IADsPropertyValue *pValue = NULL;
	IADsLargeInteger *pLargeInt = NULL;
	long lType, lValue;
	BSTR bstr, szString;
	VARIANT var;
	const CHAR *pszBOOL = NULL;
	FILETIME filetime;
	SYSTEMTIME systemtime;
	IDispatch *pDisp = NULL;
	DATE date;

	//  For Octet Strings
	void HUGEP *pArray = NULL;
	ULONG dwSLBound;
	ULONG dwSUBound;

	VariantInit(&var);
	hr = pEntry->get_Values(&var);
	if (SUCCEEDED(hr))
	{
		//  Should be a safe array that contains variants
		if (var.vt == (VT_VARIANT | VT_ARRAY))
		{
			VARIANT *pVar;
			long lLBound, lUBound;

			hr = SafeArrayAccessData((SAFEARRAY*)(var.pparray), (void HUGEP* FAR*)&pVar);

			//  One-dimensional array. Get the bounds for the array.
			hr = SafeArrayGetLBound((SAFEARRAY*)(var.pparray), 1, &lLBound);
			hr = SafeArrayGetUBound((SAFEARRAY*)(var.pparray), 1, &lUBound);

			//  Get the count of elements.
			long cElements = lUBound - lLBound + 1;

			//  Get the array elements.
			if (SUCCEEDED(hr))
			{
				for (int i = 0; i < cElements; i++)
				{
					switch (pVar[i].vt)
					{
					case VT_BSTR:
						//wprintf(L"%s ", pVar[i].bstrVal);
                        FORMAT_VALUE(value, pVar[i].bstrVal, i, L" ");
						break;
					case VT_DISPATCH:
						hr = V_DISPATCH(&pVar[i])->QueryInterface(IID_IADsPropertyValue, (void**)&pValue);
						if (SUCCEEDED(hr))
						{
							hr = pValue->get_ADsType(&lType);
							switch (lType)
							{
							case ADSTYPE_DN_STRING:
								hr = pValue->get_DNString(&bstr);
								//wprintf(L"%s ", bstr);
                                FORMAT_VALUE(value, bstr, i, L" ");
								SysFreeString(bstr);
								break;

							case ADSTYPE_CASE_IGNORE_STRING:
								hr = pValue->get_CaseIgnoreString(&bstr);
								//wprintf(L"%s ", bstr);
                                FORMAT_VALUE(value, bstr, i, L" ");
								SysFreeString(bstr);
								break;

							case ADSTYPE_BOOLEAN:
								hr = pValue->get_Boolean(&lValue);
								pszBOOL = lValue ? "TRUE" : "FALSE";
								//wprintf(L"%s ", pszBOOL);
                                //FORMAT_VALUE(value, pszBOOL, i, L" ");
								break;

							case ADSTYPE_INTEGER:
                            {
                                hr = pValue->get_Integer(&lValue);
								//wprintf(L"%d ", lValue);
                                auto l_str = std::to_wstring(lValue);
                                FORMAT_VALUE(value, (BSTR)(l_str.c_str()), i, L" ");
                            }
								
								break;
							case ADSTYPE_OCTET_STRING:
							{
								VARIANT varOS;

								VariantInit(&varOS);

								//  Get the name of the property to handle
								//  the required properties.
								pEntry->get_Name(&szString);
								hr = pValue->get_OctetString(&varOS);

								//  Get a pointer to the bytes in the octet string.
								if (SUCCEEDED(hr))
								{
									hr = SafeArrayGetLBound(V_ARRAY(&varOS),
										1,
										(long FAR *) &dwSLBound);

									hr = SafeArrayGetUBound(V_ARRAY(&varOS),
										1,
										(long FAR *) &dwSUBound);

									if (SUCCEEDED(hr))
									{
										hr = SafeArrayAccessData(V_ARRAY(&varOS), &pArray);
									}

									if (0 == wcscmp(L"objectGUID", szString))
									{
										LPOLESTR szDSGUID = new WCHAR[39];

										//  Cast to LPGUID
										LPGUID pObjectGUID = (LPGUID)pArray;

										//  Convert GUID to string.
										::StringFromGUID2(*pObjectGUID, szDSGUID, 39);

										//  Print the GUID
										//wprintf(L"%s ", szDSGUID);
									}
									else if (0 == wcscmp(L"objectSid", szString))
									{
										PSID pObjectSID = (PSID)pArray;
										//  Convert SID to string.
										LPOLESTR szSID = NULL;
										ConvertSidToStringSid(pObjectSID, (LPSTR*)&szSID);
										//wprintf(L"%s ", szSID);
										LocalFree(szSID);
									}
									else
									{
										//wprintf(L"Value of type Octet String. No Conversion.");
									}
									SafeArrayUnaccessData(V_ARRAY(&varOS));
									VariantClear(&varOS);
								}

								SysFreeString(szString);
							}
							break;

							case ADSTYPE_UTC_TIME:
								//  wprintf(L"Value of type UTC_TIME\n");
								hr = pValue->get_UTCTime(&date);
								if (SUCCEEDED(hr))
								{
									VARIANT varDate;

									//  Pack in variant.vt
									varDate.vt = VT_DATE;
									varDate.date = date;

									VariantChangeType(&varDate, &varDate, VARIANT_NOVALUEPROP, VT_BSTR);
									//wprintf(L"%s ", varDate.bstrVal);
									VariantClear(&varDate);
								}
								break;

							case ADSTYPE_LARGE_INTEGER:
								//  wprintf(L"Value of type Large Integer\n");
								//  Get the name of the property to handle
								//  the required properties.
								pEntry->get_Name(&szString);
								hr = pValue->get_LargeInteger(&pDisp);
								if (SUCCEEDED(hr))
								{
									hr = pDisp->QueryInterface(IID_IADsLargeInteger, (void**)&pLargeInt);
									if (SUCCEEDED(hr))
									{
										hr = pLargeInt->get_HighPart((long*)&filetime.dwHighDateTime);
										hr = pLargeInt->get_LowPart((long*)&filetime.dwLowDateTime);
										if ((filetime.dwHighDateTime == 0) && (filetime.dwLowDateTime == 0))
										{
											//wprintf(L"No Value ");
										}
										else
										{
											//  Verify properties of type LargeInteger that represent time
											//  if TRUE, then convert to variant time.
											if ((0 == wcscmp(L"accountExpires", szString)) ||
												(0 == wcscmp(L"badPasswordTime", szString)) ||
												(0 == wcscmp(L"lastLogon", szString)) ||
												(0 == wcscmp(L"lastLogoff", szString)) ||
												(0 == wcscmp(L"lockoutTime", szString)) ||
												(0 == wcscmp(L"pwdLastSet", szString))
												)
											{
												//  Handle special case for Never Expires where low part is -1.
												if (filetime.dwLowDateTime == -1)
												{
													//wprintf(L"Never Expires ");
												}
												else
												{
													if (FileTimeToLocalFileTime(&filetime, &filetime) != 0)
													{
														if (FileTimeToSystemTime(&filetime, &systemtime) != 0)
														{
															if (SystemTimeToVariantTime(&systemtime, &date) != 0)
															{
																VARIANT varDate;

																//  Pack in variant.vt
																varDate.vt = VT_DATE;
																varDate.date = date;

																VariantChangeType(&varDate, &varDate, VARIANT_NOVALUEPROP, VT_BSTR);

																//wprintf(L"%s ", varDate.bstrVal);

																VariantClear(&varDate);
															}
															else
															{
																//wprintf(L"FileTimeToVariantTime failed ");
															}
														}
														else
														{
															//wprintf(L"FileTimeToSystemTime failed ");
														}

													}
													else
													{
														//wprintf(L"FileTimeToLocalFileTime failed ");
													}
												}
											}
											//  Print the LargeInteger.
											else
											{
												//wprintf(L"Large Integer: high: %d low: %d ", filetime.dwHighDateTime, filetime.dwLowDateTime);
											}
										}
									}
									if (pLargeInt)
										pLargeInt->Release();
								}
								else
								{
									//wprintf(L"Cannot get Large Integer");
								}

								if (pDisp)
									pDisp->Release();

								break;

							case ADSTYPE_NT_SECURITY_DESCRIPTOR:
								//wprintf(L"Value of type NT Security Descriptor ");
								break;

							case ADSTYPE_PROV_SPECIFIC:
								//wprintf(L"Value of type Provider Specific ");
								break;

							default:
								//wprintf(L"Unhandled ADSTYPE for property value: %d ", lType);
								break;
							}
						}
						else
						{
							//wprintf(L"QueryInterface failed for IADsPropertyValue. HR: %x\n", hr);
						}

						if (pValue)
						{
							pValue->Release();
						}
						break;

					default:
						//wprintf(L"Unhandled Variant type for property value array: %d\n", pVar[i].vt);
						break;
					}
				}
				//wprintf(L"\n");
			}

			//  Decrement the access count for the array.
			SafeArrayUnaccessData((SAFEARRAY*)(var.pparray));
		}

		VariantClear(&var);
	}

	return hr;
}

HRESULT GetUserProperties(IADs *pObj, UserAttr *pUser)
{
	Log::WriteLog(log_trace, "ENTER GetUserProperties");

	HRESULT hr = E_FAIL;
	LPOLESTR szDSPath = new OLECHAR[MAX_PATH];
	long lCount = 0L;
	long lCountTotal = 0L;
	long lPType = 0L;

	if (!pObj)
	{
		return E_INVALIDARG;
	}

	//  Call GetInfo to load all object properties into the cache
	//  because IADsPropertyList methods read from the cache.
	hr = pObj->GetInfo();
	if (SUCCEEDED(hr))
	{
		IADsPropertyList *pObjProps = NULL;

		//  QueryInterface for an IADsPropertyList pointer.
		hr = pObj->QueryInterface(IID_IADsPropertyList, (void**)&pObjProps);
		if (SUCCEEDED(hr))
		{
			VARIANT var;

			//  Enumerate the properties of the object.
			hr = pObjProps->get_PropertyCount(&lCountTotal);
			//wprintf(L"Property Count: %d\n", lCountTotal);
			Log::WriteLog(log_trace, "Property Count: %d", lCountTotal);

			VariantInit(&var);
			hr = pObjProps->Next(&var);
			if (SUCCEEDED(hr))
			{
				lCount = 1L;
				while (hr == S_OK)
				{
					if (var.vt == VT_DISPATCH)
					{
						IADsPropertyEntry *pEntry = NULL;

						hr = V_DISPATCH(&var)->QueryInterface(IID_IADsPropertyEntry, (void**)&pEntry);
						if (SUCCEEDED(hr))
						{
							BSTR bstrString;

							hr = pEntry->get_Name(&bstrString);
							//wprintf(L"%s: ", bstrString);
                            std::string keyA ;
							CommonFun::ToUTF8(bstrString, keyA);
							SysFreeString(bstrString);

							hr = pEntry->get_ADsType(&lPType);
							if (lPType != ADSTYPE_INVALID)
							{
                                std::wstring value = L"";
								hr = EnumeratePropertyValue(pEntry, value);
								if (FAILED(hr))
								{
									//printf("EnumeratePropertyValue failed. hr: %x\n", hr);
									Log::WriteLog(log_trace, "EnumeratePropertyValue failed. hr: %x", hr);
								}
                                else {
                                    std::string valueA ;
									CommonFun::ToUTF8(value, valueA);
								    pUser->AddAttr(keyA.c_str(), valueA.c_str());
                                    Log::WriteLog(log_info, "Read user attribute for %s, %s=%s", pUser->GetUserNameAttr().c_str(), keyA.c_str(),
                                    valueA.c_str() );
                                }
							}
							else
							{
								//wprintf(L"Invalid type\n");
								Log::WriteLog(log_trace, "Invalid type");
							}
						}
						else
						{
							//printf("IADsPropertyEntry QueryInterface call failed. hr: %x\n", hr);
							Log::WriteLog(log_trace, "IADsPropertyEntry QueryInterface call failed. hr: %x", hr);
						}

						// Cleanup.
						if (pEntry)
						{
							pEntry->Release();
						}
					}
					else
					{
						//printf("Unexpected returned type for VARIANT: %d", var.vt);
						Log::WriteLog(log_trace, "Unexpected returned type for VARIANT: %d", var.vt);
					}
					VariantClear(&var);
					hr = pObjProps->Next(&var);
					if (SUCCEEDED(hr))
					{
						lCount++;
					}
				}
			}
			//  Cleanup.
			pObjProps->Release();
		}

		//wprintf(L"Total properties retrieved: %d\n", lCount);
	}

	//  Return S_OK if all properties were retrieved.
	if (lCountTotal == lCount)
	{
		hr = S_OK;
	}

	return hr;
}

//  The FindUserByName function searches for users.
#define NUM_ATTRIBUTES 2

HRESULT FindUserByName(IDirectorySearch *pSearchBase, //  Container to search.
	LPWSTR szLoginName,
	LPWSTR szPwd,
	LPOLESTR szFindUser, //  Name of user to find.
	IADs **ppUser) //  Return a pointer to the user.
{
	if ((!pSearchBase) || (!szFindUser))
	{
		return E_INVALIDARG;
	}

	HRESULT hrObj = E_FAIL;
	HRESULT hr = E_FAIL;
	ADS_SEARCHPREF_INFO SearchPrefs;
	//  COL for iterations
	ADS_SEARCH_COLUMN col;
	//  Handle used for searching
	ADS_SEARCH_HANDLE hSearch;

	//  Search entire subtree from root.
	SearchPrefs.dwSearchPref = ADS_SEARCHPREF_SEARCH_SCOPE;
	SearchPrefs.vValue.dwType = ADSTYPE_INTEGER;
	SearchPrefs.vValue.Integer = ADS_SCOPE_SUBTREE;

	//  Set the search preference.
	DWORD dwNumPrefs = 1;
	hr = pSearchBase->SetSearchPreference(&SearchPrefs, dwNumPrefs);
	if (FAILED(hr))
	{
		return hr;
	}

	//  Create search filter.
	LPCWSTR pszFormat = L"(&(objectCategory=person)(objectClass=user)(userPrincipalName=%s))";
	LPWSTR pszSearchFilter = new WCHAR[wcslen(pszFormat) + wcslen(szFindUser) + 1];
	if (NULL == pszSearchFilter)
	{
		return E_OUTOFMEMORY;
	}

//#ifdef _MBCS
	swprintf_s(pszSearchFilter, wcslen(pszFormat) + wcslen(szFindUser)+1, pszFormat, szFindUser);
//#endif _MBCS

	//  Set attributes to return.
	LPWSTR pszAttribute[NUM_ATTRIBUTES] = { (LPWSTR)L"ADsPath", (LPWSTR)L"samAccountName" };

	//  Execute the search.
	hr = pSearchBase->ExecuteSearch(pszSearchFilter,
		pszAttribute,
		NUM_ATTRIBUTES,
		&hSearch);
	if (SUCCEEDED(hr))
	{
		//  Call IDirectorySearch::GetNextRow() to retrieve the next row of data.
		while (pSearchBase->GetNextRow(hSearch) != S_ADS_NOMORE_ROWS)
		{
			//  Loop through the array of passed column names and
			//  print the data for each column.
			for (DWORD x = 0; x < NUM_ATTRIBUTES; x++)
			{
				//  Get the data for this column.
				hr = pSearchBase->GetColumn(hSearch, pszAttribute[x], &col);
				if (SUCCEEDED(hr))
				{
					//  Print the data for the column and free the column.
					//  Be aware that the requested attribute is type CaseIgnoreString.
					if (ADSTYPE_CASE_IGNORE_STRING == col.dwADsType)
					{
						hr = ADsOpenObject(col.pADsValues->CaseIgnoreString,
							 szLoginName,
							 szPwd,
							ADS_SECURE_AUTHENTICATION,
							IID_IADs,
							(void**)ppUser);
						if (SUCCEEDED(hr))
						{
							//wprintf(L"Found User.\n", szFindUser);
							//wprintf(L"%s: %s\r\n", pszAttribute[x], col.pADsValues->CaseIgnoreString);
							hrObj = S_OK;
						}
					}

					pSearchBase->FreeColumn(&col);
				}
				else
				{
					hr = E_FAIL;
				}
			}
		}
		//  Close the search handle to cleanup.
		pSearchBase->CloseSearchHandle(hSearch);
	}

	delete[] pszSearchFilter;

	if (FAILED(hrObj))
	{
		hr = hrObj;
	}

	return hr;
}

IQueryUserAttribute* make_ad_impl(const char *ldap, const char *u_name, const char *pwd) {
    return new ImplQueryUserAttributeAD(ldap, u_name, pwd);
}

#include "WinAD.h"
class ImplQueryUserAttributeADV2 :public IQueryUserAttribute {
public:
    ImplQueryUserAttributeADV2(){

	}
    ImplQueryUserAttributeADV2(const char *attrs){
			if(attrs)
				_conf_attrs = attrs;
	}
	virtual bool QueryUserAttribute(UserAttr * pUser) override {
		return query_ad_attr_2_user(pUser, _conf_attrs.c_str());
	}

private:
    std::string _conf_attrs;
	//WinAd
	//CWinAD _wad;
};

IQueryUserAttribute* make_ad_impl2(const char *attrs){
	return new ImplQueryUserAttributeADV2(attrs);
}

bool query_ad_attr_2_user(IUserAttr * pUser, const char * user_attrs){
		if(pUser==NULL)
			return false;

		const int nBufSize = 255;
		WCHAR chBuf[nBufSize];
		ZeroMemory(chBuf, nBufSize);
		DWORD dwRet = nBufSize;
		if (!::GetUserNameW(chBuf, &dwRet)){
			return false;
		}

		std::wstring strKeyWord = L"(sAMAccountName=";
		std::wstring name(chBuf);
		//printf("name:%ws\n", name.c_str());

		strKeyWord += name;
		//::SysFreeString(bstrName_l);
		strKeyWord += L")";
		CWinAD _wad(user_attrs);
		if (_wad.SearchUserInfo( strKeyWord.c_str()))
		{
			for(auto it = _wad._attr.begin(); it != _wad._attr.end(); ++it){
				std::string key_a,value_a;
				CommonFun::ToUTF8(it->first, key_a);
				CommonFun::ToUTF8(it->second, value_a);
				Log::WriteLog(log_info, "get ad user attr %s:%s)", key_a.c_str(),value_a.c_str());
				((UserAttr*)pUser)->AddAttr(key_a.c_str(), value_a.c_str());
			}
		}
		else {
			//printf("return false");
			return false;
		}
		return true;
}

