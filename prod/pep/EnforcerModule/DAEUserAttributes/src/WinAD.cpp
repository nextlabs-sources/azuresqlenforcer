//#include "stdafx.h"
#include "WinAD.h"
#include <vector>
#include "commfun.h"



CWinAD::CWinAD(const std::string & attrs)
{
	CoInitialize(NULL);

    _attr_names.insert(L"mail");
    _attr_names.insert(L"title");
    _attr_names.insert(L"displayName");
    _attr_names.insert(L"department");
    _attr_names.insert(L"cn");
    _attr_names.insert(L"st");
    _attr_names.insert(L"objectSid");
    std::vector<std::string> output;
    CommonFun::SplitString(attrs,",",output);
    for(auto it:output){
        std::wstring val;
        CommonFun::FromUTF8(it,val);
        _attr_names.insert(val);
    }

}

CWinAD::~CWinAD(void)
{
	::CoUninitialize();
}

// // search current user's manager's email
bool CWinAD::SearchUserInfo(const wchar_t* wstrkeyWord)
{

	// Initialize COM.
	HRESULT hr = S_OK;
	// Get rootDSE and the current user domain container distinguished name.
	CComPtr<IADs> pObject = NULL;
	CComPtr<IDirectorySearch> pContainerToSearch = NULL;
	LPOLESTR szPath = new OLECHAR[MAX_PATH];
	VARIANT var;
	hr = ADsOpenObject(L"LDAP://rootDSE",
		NULL,
		NULL,
		ADS_SECURE_AUTHENTICATION, // Use Secure Authentication.
		IID_IADs,
		(void**)&pObject);
	if (FAILED(hr))
	{
		delete[] szPath;
		return false;
	}
	if (SUCCEEDED(hr))
	{
		hr = pObject->Get(_bstr_t("defaultNamingContext"),&var);
		if (SUCCEEDED(hr))
		{
			// Build path to the domain container.
			wcsncpy_s(szPath,MAX_PATH/sizeof(OLECHAR),L"LDAP://", _TRUNCATE);
			wcsncat_s(szPath,MAX_PATH/sizeof(OLECHAR),var.bstrVal, _TRUNCATE);
			hr = ADsOpenObject(szPath,
				NULL,
				NULL,
				ADS_SECURE_AUTHENTICATION, // Use Secure Authentication.
				IID_IDirectorySearch,
				(void**)&pContainerToSearch);

			if (SUCCEEDED(hr))
			{
				// Convert to a char*
				if(wstrkeyWord == NULL)
					wstrkeyWord = m_strFileterKeyWords.c_str();
				hr = FindUsers(pContainerToSearch, // IDirectorySearch pointer to domainDNS container.
					(LPOLESTR)wstrkeyWord,  
					NULL, // Return all properties.
					TRUE 
					);
			}
		}
		VariantClear(&var);
	}
	delete [] szPath;	

	if (S_FALSE==hr || !SUCCEEDED(hr)) return false;

	return true;
}



/* ************************************************** */
HRESULT CWinAD::FindUsers(CComPtr<IDirectorySearch> pContainerToSearch,  // IDirectorySearch pointer to the container to search.
				  LPOLESTR szFilter, // Filter for finding specific users.
				  // NULL returns all user objects.
				  LPOLESTR *pszPropertiesToReturn, // Properties to return for user objects found.
				  // NULL returns all set properties.
				  BOOL bIsVerbose    // TRUE indicates that all properties for the found objects are displayed.
				  // FALSE indicates only the RDN.
				  )
{
	if (!pContainerToSearch)
		return E_POINTER;
	DWORD dwLength = MAX_PATH*2;
	// Create search filter.
	LPOLESTR pszSearchFilter = new OLECHAR[dwLength];

	// Add the filter.
	//swprintf_s(pszSearchFilter, dwLength, L"(&(objectClass=user)(objectCategory=person)%s)",szFilter);
	wnsprintfW(pszSearchFilter, dwLength, L"(&(objectClass=user)(objectCategory=person)%ws)",szFilter); //m_strFileterKeyWords.c_str());

	std::wstring strKey = szFilter;
	bool bGetManager=false;
	if(strKey.find(L"sAMAccountName=") != std::wstring::npos)
	{
		bGetManager = true;
	}
	// Specify subtree search.
	ADS_SEARCHPREF_INFO SearchPrefs;
	SearchPrefs.dwSearchPref = ADS_SEARCHPREF_SEARCH_SCOPE;
	SearchPrefs.vValue.dwType = ADSTYPE_INTEGER;
	SearchPrefs.vValue.Integer = ADS_SCOPE_SUBTREE;
	DWORD dwNumPrefs = 1;

	// COL for iterations.
	LPOLESTR pszColumn = NULL;  
	ADS_SEARCH_COLUMN col;
	HRESULT hr = S_OK;

	// Interface Pointers
//	CComPtr<IADs> pObj = NULL;
//	CComPtr<IADs> pIADs = NULL;

	// Search handle.
	ADS_SEARCH_HANDLE hSearch = NULL;

	// Set search preference.
	hr = pContainerToSearch->SetSearchPreference( &SearchPrefs, dwNumPrefs);
	if (FAILED(hr))
	{
		delete[] pszSearchFilter;
		return hr;
	}
#pragma warning(push)
#pragma warning(disable: 6211)
//	LPOLESTR pszBool = NULL;
//	DWORD dwBool = 0;
	PSID pObjectSID = NULL;
	LPOLESTR szSID = NULL;
	LPOLESTR szDSGUID = new WCHAR [39];
//	LPGUID pObjectGUID = NULL;
	VARIANT varDate;
	LPOLESTR *pszPropertyList = NULL;
	LPOLESTR pszNonVerboseList[] = {L"name",L"distinguishedName"};

	LPOLESTR szName = new OLECHAR[MAX_PATH];
	LPOLESTR szDN = new OLECHAR[MAX_PATH];
#pragma warning(pop)
	VariantInit(&varDate);

	int iCount = 0;
//	DWORD x = 0L;

	if (!bIsVerbose)
	{
		// Return non-verbose list properties only.
		hr = pContainerToSearch->ExecuteSearch(pszSearchFilter,
			pszNonVerboseList,
			sizeof(pszNonVerboseList)/sizeof(LPOLESTR),
			&hSearch
			);
	}
	else
	{
		if (!pszPropertiesToReturn)
		{
			// Return all properties.
			hr = pContainerToSearch->ExecuteSearch(pszSearchFilter,
				NULL,
				(DWORD)-1,
				&hSearch
				);
		}
		else
		{
			// Specified subset.
			pszPropertyList = pszPropertiesToReturn;
			// Return specified properties.
			hr = pContainerToSearch->ExecuteSearch(pszSearchFilter,
				pszPropertyList,
#pragma warning(push)
#pragma warning(disable: 6384)
				sizeof(pszPropertyList)/sizeof(LPOLESTR),
#pragma warning(pop)
				&hSearch
				);
		}
	}
	if ( SUCCEEDED(hr) )
	{  
		// Call IDirectorySearch::GetNextRow() to retrieve the next data row.
		hr = pContainerToSearch->GetFirstRow( hSearch);
		if (SUCCEEDED(hr))
		{
			while( hr != S_ADS_NOMORE_ROWS )
			{
				// Keep track of count.
				iCount++;
				// Loop through the array of passed column names,
				// print the data for each column.

				while( pContainerToSearch->GetNextColumnName( hSearch, &pszColumn ) != S_ADS_NOMORE_COLUMNS )
				{
					hr = pContainerToSearch->GetColumn( hSearch, pszColumn, &col );

					ReadAttribute(col);

// 					if(_wcsicmp(m_strMailName.c_str(),col.pszAttrName)==0)
// 					{
// 						switch(col.dwADsType)
// 						{
// 						case ADSTYPE_DN_STRING:
// 						case ADSTYPE_CASE_EXACT_STRING:    
// 						case ADSTYPE_CASE_IGNORE_STRING:    
// 						case ADSTYPE_PRINTABLE_STRING:    
// 						case ADSTYPE_NUMERIC_STRING:      
// 						case ADSTYPE_TYPEDNAME:        
// 						case ADSTYPE_FAXNUMBER:        
// 						case ADSTYPE_PATH:          
// 							if(col.dwNumValues>0)
// 							{
// 								m_theUserInfo.strEmail = col.pADsValues[0].CaseIgnoreString;
// 								if(!m_theUserInfo.strSID.empty() && !m_strManagerKey.empty())
// 								{
// 									pContainerToSearch->FreeColumn( &col );
// 									FreeADsMem( pszColumn );
// 									goto Return;
// 								}
// 							}
// 							break;
// 						case ADSTYPE_BOOLEAN:
// 							break;
// 						}
// 					}
// 					else if(_wcsicmp(m_strSidName.c_str(),col.pszAttrName) ==0)
// 					{
// 						switch(col.dwADsType)
// 						{
// 						case ADSTYPE_OCTET_STRING:
// 							{
// 								if(col.dwNumValues > 0)
// 								{
// 									pObjectSID = (PSID)(col.pADsValues[0].OctetString.lpValue);
// 									// Convert SID to string.
// 									ConvertSidToStringSid(pObjectSID, &szSID);
// 									m_theUserInfo.strSID = (wchar_t*)szSID;
// 									LocalFree(szSID);	
// 									if(!m_theUserInfo.strEmail.empty() && !m_strManagerKey.empty())
// 									{
// 										pContainerToSearch->FreeColumn( &col );
// 										FreeADsMem( pszColumn );
// 										goto Return;
// 									}
// 								}
// 							}
// 							break;
// 						default:
// 							break;
// 						}
// 					}
// 					else if(_wcsicmp(col.pszAttrName,L"manager")==0 && bGetManager)
// 					{
// 						switch(col.dwADsType)
// 						{
// 						case ADSTYPE_DN_STRING:
// 						case ADSTYPE_CASE_EXACT_STRING:    
// 						case ADSTYPE_CASE_IGNORE_STRING:    
// 						case ADSTYPE_PRINTABLE_STRING:    
// 						case ADSTYPE_NUMERIC_STRING:      
// 						case ADSTYPE_TYPEDNAME:        
// 						case ADSTYPE_FAXNUMBER:        
// 						case ADSTYPE_PATH:          
// 							if(col.dwNumValues>0)
// 							{
// 								m_strManagerKey = col.pADsValues[0].CaseIgnoreString;
// 								if(!m_theUserInfo.strEmail.empty() && !m_theUserInfo.strSID.empty())
// 								{
// 									pContainerToSearch->FreeColumn( &col );
// 									FreeADsMem( pszColumn );
// 									goto Return;
// 								}
// 							}
// 							break;
// 						case ADSTYPE_BOOLEAN:
// 							break;
// 						}
// 					}
					pContainerToSearch->FreeColumn( &col );
					FreeADsMem( pszColumn );
				}
				hr = pContainerToSearch->GetNextRow( hSearch);
			}

		}
//Return:
		// Close the search handle to cleanup.
		pContainerToSearch->CloseSearchHandle(hSearch);
	} 
	if (SUCCEEDED(hr) && 0==iCount)
		hr = S_FALSE;

	delete [] szName;
	delete [] szDN;
	delete [] szDSGUID;
	delete [] pszSearchFilter;
	return hr;
}

//bool check_key(std::wstring key) {
//	const std::set<std::wstring> set_sttr{
//			L"mail",
//			L"title",
//			L"displayName",
//			L"department",
//			L"cn",
//			L"st",
//			L"objectSid"
//	};
//	return set_sttr.find(key) != set_sttr.end();
//}

bool CWinAD::ReadAttribute(ADS_SEARCH_COLUMN & col) {
	std::wstring key_, value_;
	
	key_ = col.pszAttrName;


	if (_attr_names.find(key_) == _attr_names.end()) {
		return false;
	}
	//DWORD dwnum = col.dwNumValues;
		
	//std::wstring value;// = col.pADsValues[0].CaseIgnoreString;
	//lType = col.dwADsType;
	ADSTYPE tp = col.pADsValues->dwType;
	switch (tp)
	{
	case ADSTYPE_DN_STRING: {
		value_  = col.pADsValues->DNString;
	}	break;
	case ADSTYPE_CASE_EXACT_STRING: {
		value_ = col.pADsValues->CaseExactString;
	}	break;
	case ADSTYPE_CASE_IGNORE_STRING: {
		value_ = col.pADsValues->CaseIgnoreString;
	}	break;
	case ADSTYPE_PRINTABLE_STRING: {
		value_ = col.pADsValues->PrintableString;
	}	break;
	case ADSTYPE_NUMERIC_STRING: {
		value_ = col.pADsValues->NumericString;
	}	break;
	case ADSTYPE_BOOLEAN: {
		DWORD c = col.pADsValues->Boolean;
		value_ = c ? L"TRUE" : L"FALSE";
	}	break;
	case ADSTYPE_INTEGER:
	{
		DWORD c = col.pADsValues->Integer;
		value_ = std::to_wstring(c);

	}   break;
	case ADSTYPE_OCTET_STRING:
	{
		ADS_OCTET_STRING c = col.pADsValues->OctetString;
		DWORD dw = c.dwLength;
		LPBYTE lpb = c.lpValue;

		if (col.dwNumValues > 0)
		{
			PSID pObjectSID = (PSID)(col.pADsValues[0].OctetString.lpValue);
			LPOLESTR szSID = NULL;
			// Convert SID to string.
			ConvertSidToStringSidW(pObjectSID, &szSID);
			if(szSID){
                value_ = (wchar_t*)szSID;
                LocalFree(szSID);
			}
		}
	}
	break;
	case ADSTYPE_UTC_TIME: {
		SYSTEMTIME st = col.pADsValues->UTCTime;
		const auto tm = CWinAD::ConvertIntoTm(st);
		const auto timeStemp = CWinAD::GetTimeStamp(tm);

		value_ = std::to_wstring(timeStemp * 1000);
	}	break;
	case ADSTYPE_LARGE_INTEGER: {
		LARGE_INTEGER ci = col.pADsValues->LargeInteger;
		const auto v = CWinAD::ToInteger(ci);

		value_ = std::to_wstring(v);
	}	break;
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

	if (key_.empty() || value_.empty()) {
		return false;
	}

	_attr[key_] = value_;
	if(key_.compare(L"objectSid") == 0){
        _attr[L"subject-id"] = value_;
	}
	return true;

}
