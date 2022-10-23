#include "CertificateHelper.h"
#include <stdio.h>
#include <tchar.h>
#include "logger_class.h"

PSecurityFunctionTable SecurityFunTable = NULL;
PCCERT_CONTEXT pCertContext = NULL;
CredHandle hCreds;

BOOL LoadSecurityLibrary() // load SSPI.DLL, set up a special table - PSecurityFunctionTable
{
	INIT_SECURITY_INTERFACE pInitSecurityInterface;
	//  QUERY_CREDENTIALS_ATTRIBUTES_FN pQueryCredentialsAttributes;
    OSVERSIONINFO VerInfo = {0};
    CHAR lpszDLL[MAX_PATH] = {0};

	//  Find out which security DLL to use, depending on
	//  whether we are on Win2K, NT or Win9x
	VerInfo.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
	if ( !GetVersionEx (&VerInfo) ) return FALSE;

	if ( VerInfo.dwPlatformId == VER_PLATFORM_WIN32_NT  &&  VerInfo.dwMajorVersion == 4 )
	{
		strcpy (lpszDLL, "Security.dll" ); 
	}
	else if ( VerInfo.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS ||
		VerInfo.dwPlatformId == VER_PLATFORM_WIN32_NT )
	{
		strcpy(lpszDLL, "Secur32.dll"); 
	}
	else 
	{
        daebootstrap::Logger::Instance().Error("Not supported system platform. Can't load SSPI interface");
        return FALSE;
	}

	//  Load Security DLL
	HMODULE hSecurity = LoadLibraryA(lpszDLL);
	if(hSecurity == NULL) 
	{
        daebootstrap::Logger::Instance().Error("Error 0x%x loading %s.\n", GetLastError(), lpszDLL);
		return FALSE;
	}

	pInitSecurityInterface = (INIT_SECURITY_INTERFACE)GetProcAddress( hSecurity, "InitSecurityInterfaceA" );
	if(pInitSecurityInterface == NULL) 
	{
        daebootstrap::Logger::Instance().Error("Error 0x%x reading InitSecurityInterface entry point.", GetLastError());
		return FALSE;
	}

	SecurityFunTable = pInitSecurityInterface(); 
	if(SecurityFunTable == NULL)
	{
        daebootstrap::Logger::Instance().Error("Error 0x%x reading security interface.", GetLastError());
		return FALSE;
	}

	return TRUE;
}

SECURITY_STATUS CertFindCertificateByName(LPCTSTR pszSubjectName)
{
	HCERTSTORE  hMyCertStore = NULL;
    TCHAR pszFriendlyNameString[128] = {0};
    TCHAR pszNameString[128] = {0};

	if (pszSubjectName == NULL || _tcslen(pszSubjectName) == 0)
	{
		daebootstrap::Logger::Instance().Error("CertFindCertificateByName No subject name specified!");
		return E_POINTER;
	}

    // Open the "MY" certificate store, where IE stores client certificates.
    // Windows maintains 4 stores -- MY, CA, ROOT, SPC. 
    hMyCertStore = CertOpenStore(CERT_STORE_PROV_SYSTEM,
                   X509_ASN_ENCODING,
                   NULL,
                   CERT_STORE_OPEN_EXISTING_FLAG | CERT_STORE_READONLY_FLAG | CERT_SYSTEM_STORE_LOCAL_MACHINE,
                   L"ROOT");

	if (!hMyCertStore)
	{
		int err = GetLastError();

        if (err == ERROR_ACCESS_DENIED)
            daebootstrap::Logger::Instance().Error("**** CertOpenStore failed with 'access denied'");
        else
            daebootstrap::Logger::Instance().Error("**** Error %d returned by CertOpenStore", err);

		return HRESULT_FROM_WIN32(err);
	}

	if (pCertContext)	// The caller passed in a certificate context we no longer need, so free it
		CertFreeCertificateContext(pCertContext);
	pCertContext = NULL;

	char * serverauth = szOID_PKIX_KP_SERVER_AUTH;
	CERT_ENHKEY_USAGE eku;
	PCCERT_CONTEXT pCertContextSaved = NULL;
	eku.cUsageIdentifier = 1;
	eku.rgpszUsageIdentifier = &serverauth;
	// Find a server certificate. Note that this code just searches for a 
	// certificate that has the required enhanced key usage for server authentication
	// it then selects the best one (ideally one that contains the server name
	// in the subject name).

	while (NULL != (pCertContext = CertFindCertificateInStore(hMyCertStore,
		X509_ASN_ENCODING,
		CERT_FIND_OPTIONAL_ENHKEY_USAGE_FLAG,
		CERT_FIND_ENHKEY_USAGE,
		&eku,
		pCertContext)))
	{
		if (!CertGetNameString(pCertContext, CERT_NAME_FRIENDLY_DISPLAY_TYPE, 0, NULL, pszFriendlyNameString, sizeof(pszFriendlyNameString)))
		{
			continue;
		}

		if (_tcsicmp(pszFriendlyNameString, pszSubjectName) == 0)
		{
			break;
		}
	}

	if (!pCertContext)
	{
		DWORD LastError = GetLastError();
        daebootstrap::Logger::Instance().Error("**** Error 0x%x returned by CertFindCertificateInStore", LastError);
        CertCloseStore(hMyCertStore, CERT_CLOSE_STORE_FORCE_FLAG);
        return HRESULT_FROM_WIN32(LastError);
	}

    CertCloseStore(hMyCertStore, 0);
	return SEC_E_OK;
}


// Create credentials (a handle to a certificate) by selecting an appropriate certificate
// We take a best guess at a certificate to be used as the SSL certificate for this server 
SECURITY_STATUS CreateCredentialsFromCertificate()
{
	memset(&hCreds, 0, sizeof(hCreds));

	// Build Schannel credential structure.
	SCHANNEL_CRED   SchannelCred = { 0 };
	SchannelCred.dwVersion = SCHANNEL_CRED_VERSION;
	SchannelCred.cCreds = 1;
	SchannelCred.paCred = &pCertContext;
	SchannelCred.grbitEnabledProtocols = SP_PROT_TLS1_0 | SP_PROT_TLS1_1 | SP_PROT_TLS1_2;
	SchannelCred.dwFlags = 0;

	SECURITY_STATUS Status;
	TimeStamp       tsExpiry;
	// Get a handle to the SSPI credential
	Status = SecurityFunTable->AcquireCredentialsHandle(
		NULL,                   // Name of principal
		UNISP_NAME,           // Name of package
		SECPKG_CRED_INBOUND,    // Flags indicating use
		NULL,                   // Pointer to logon ID
		&SchannelCred,          // Package specific data
		NULL,                   // Pointer to GetKey() func
		NULL,                   // Value to pass to GetKey()
		&hCreds,                // (out) Cred Handle
		&tsExpiry);             // (out) Lifetime (optional)

	if (Status != SEC_E_OK)
	{
		DWORD dw = GetLastError();
        if (Status == SEC_E_UNKNOWN_CREDENTIALS)
            daebootstrap::Logger::Instance().Error("**** Error: 'Unknown Credentials' returned by AcquireCredentialsHandle. Be sure app has administrator rights. LastError=%d", dw);
        else
            daebootstrap::Logger::Instance().Error("**** Error 0x%x returned by AcquireCredentialsHandle. LastError=%d.", Status, dw);
		return Status;
	}
	else
	{
        daebootstrap::Logger::Instance().Info("Create Server credential success.");
	}

	return SEC_E_OK;
}