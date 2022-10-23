#include "CertificateHelper.h"
#include "Config.h"
#include <stdio.h>
#include "ProxyManager.h"
#include <tchar.h>



SECURITY_STATUS CertificateHelper::CertFindCertificateByName(PCCERT_CONTEXT & pCertContext, LPCTSTR pszSubjectName)
{
	HCERTSTORE  hMyCertStore = NULL;
    TCHAR pszFriendlyNameString[128] = {0};
    TCHAR pszNameString[128] = {0};

	if (pszSubjectName == NULL || _tcslen(pszSubjectName) == 0)
	{
		PROXYLOG(CELOG_EMERG, L"CertFindCertificateByName No subject name specified!");
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
            PROXYLOG(CELOG_EMERG, L"**** CertOpenStore failed with 'access denied'");
        else
            PROXYLOG(CELOG_EMERG, L"**** Error %d returned by CertOpenStore", err);

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
            PROXYLOG(CELOG_WARNING, L"CertGetNameString failed getting friendly name.");
			continue;
		}

		if (_tcsicmp(pszFriendlyNameString, pszSubjectName) == 0)
		{
			//printf("Certificate has right subject name.\n");
			break;
		}
#if 0
	//	printf("Certificate '%s' is allowed to be used for server authentication.", pszFriendlyNameString);
		if (!CertGetNameString(pCertContext, CERT_NAME_SIMPLE_DISPLAY_TYPE, 0, NULL, pszNameString, sizeof(pszNameString)))
			printf("CertGetNameString failed getting subject name.");
		else if (stricmp(pszNameString, pszSubjectName))  //  (_tcscmp(pszNameString, pszSubjectName))
		{
			printf("Certificate has wrong subject name.");
		}

		else if (CertCompareCertificateName(X509_ASN_ENCODING, &pCertContext->pCertInfo->Subject, &pCertContext->pCertInfo->Issuer))
		{
			if (!pCertContextSaved)
			{
				printf("A self-signed certificate was found and saved in case it is needed.");
				pCertContextSaved = CertDuplicateCertificateContext(pCertContext);
			}
		}
		else
		{
			printf("Certificate is acceptable.");
			if (pCertContextSaved)	// We have a saved self signed certificate context we no longer need, so free it
				CertFreeCertificateContext(pCertContextSaved);
			pCertContextSaved = NULL;
			break;
		}
#endif 
	}

#if 0
	if (pCertContextSaved && !pCertContext)
	{	// We have a saved self-signed certificate and nothing better 
		printf("A self-signed certificate was the best we had.");
		pCertContext = pCertContextSaved;
		pCertContextSaved = NULL;
	}
#endif 

	if (!pCertContext)
	{
		DWORD LastError = GetLastError();
        PROXYLOG(CELOG_EMERG, L"**** Error 0x%x returned by CertFindCertificateInStore", LastError);
        CertCloseStore(hMyCertStore, CERT_CLOSE_STORE_FORCE_FLAG);
        return HRESULT_FROM_WIN32(LastError);
	}

    CertCloseStore(hMyCertStore, 0);
	return SEC_E_OK;
}


// Create credentials (a handle to a certificate) by selecting an appropriate certificate
// We take a best guess at a certificate to be used as the SSL certificate for this server 
SECURITY_STATUS CertificateHelper::CreateCredentialsFromCertificate(PCredHandle phCreds, PCCERT_CONTEXT pCertContext)
{
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
	Status = ProxyManager::GetInstance()->SecurityFunTable->AcquireCredentialsHandle(
		NULL,                   // Name of principal
		UNISP_NAME,           // Name of package
		SECPKG_CRED_INBOUND,    // Flags indicating use
		NULL,                   // Pointer to logon ID
		&SchannelCred,          // Package specific data
		NULL,                   // Pointer to GetKey() func
		NULL,                   // Value to pass to GetKey()
		phCreds,                // (out) Cred Handle
		&tsExpiry);             // (out) Lifetime (optional)

	if (Status != SEC_E_OK)
	{
		DWORD dw = GetLastError();
        if (Status == SEC_E_UNKNOWN_CREDENTIALS)
            PROXYLOG(CELOG_EMERG, L"**** Error: 'Unknown Credentials' returned by AcquireCredentialsHandle. Be sure app has administrator rights. LastError=%d", dw);
        else
            PROXYLOG(CELOG_EMERG, L"**** Error 0x%x returned by AcquireCredentialsHandle. LastError=%d.", Status, dw);
		return Status;
	}
	else
	{
        PROXYLOG(CELOG_INFO, L"Create Server credential success.\n");
	}

	return SEC_E_OK;
}

SECURITY_STATUS CertificateHelper::CreateCredentialsForClient( LPSTR pszUser, PCredHandle phCreds )   
{
	TimeStamp        tsExpiry;
	SECURITY_STATUS  Status;
	DWORD            cSupportedAlgs = 0;
	ALG_ID           rgbSupportedAlgs[16];
	PCCERT_CONTEXT   pCertContext = NULL;


#if 0
	// Open the "MY" certificate store, where IE stores client certificates.
	// Windows maintains 4 stores -- MY, CA, ROOT, SPC. 
	if(g_hMyCertStore == NULL)
	{
		g_hMyCertStore = CertOpenSystemStore(0, "MY");
		if(!g_hMyCertStore)
		{
			printf( "**** Error 0x%x returned by CertOpenSystemStore\n", GetLastError() );
			return SEC_E_NO_CREDENTIALS;
		}
	}


	// If a user name is specified, then attempt to find a client
	// certificate. Otherwise, just create a NULL credential.
	if(pszUser)
	{
		// Find client certificate. Note that this sample just searches for a 
		// certificate that contains the user name somewhere in the subject name.
		// A real application should be a bit less casual.
		pCertContext = CertFindCertificateInStore( g_hMyCertStore,                     // hCertStore
			X509_ASN_ENCODING,             // dwCertEncodingType
			0,                                             // dwFindFlags
			CERT_FIND_SUBJECT_STR_A,// dwFindType
			pszUser,                         // *pvFindPara
			NULL );                                 // pPrevCertContext


		if(pCertContext == NULL)
		{
			printf("**** Error 0x%x returned by CertFindCertificateInStore\n", GetLastError());
			if( GetLastError() == CRYPT_E_NOT_FOUND ) printf("CRYPT_E_NOT_FOUND - property doesn't exist\n");
			return SEC_E_NO_CREDENTIALS;
		}
	}

#endif 

	// Build Schannel credential structure. Currently, this sample only
	// specifies the protocol to be used (and optionally the certificate, 
	// of course). Real applications may wish to specify other parameters as well.
	SCHANNEL_CRED SchannelCred;
	ZeroMemory( &SchannelCred, sizeof(SchannelCred) );

	SchannelCred.dwVersion  = SCHANNEL_CRED_VERSION;
	if(pCertContext)
	{
		SchannelCred.cCreds     = 1;
		SchannelCred.paCred     = &pCertContext;
	}

	SchannelCred.grbitEnabledProtocols = SP_PROT_TLS1_0 | SP_PROT_TLS1_1 | SP_PROT_TLS1_2; //ProxyManager::GetInstance()->GetTLSProtocolVersion();

#if 0
	if(g_aiKeyExch) 
		rgbSupportedAlgs[cSupportedAlgs++] = g_aiKeyExch;
#endif

	if(cSupportedAlgs)
	{
		SchannelCred.cSupportedAlgs    = cSupportedAlgs;
		SchannelCred.palgSupportedAlgs = rgbSupportedAlgs;
	}

	SchannelCred.dwFlags |= SCH_CRED_NO_DEFAULT_CREDS;

	// The SCH_CRED_MANUAL_CRED_VALIDATION flag is specified because
	// this sample verifies the server certificate manually. 
	// Applications that expect to run on WinNT, Win9x, or WinME 
	// should specify this flag and also manually verify the server
	// certificate. Applications running on newer versions of Windows can
	// leave off this flag, in which case the InitializeSecurityContext
	// function will validate the server certificate automatically.
	SchannelCred.dwFlags |= SCH_CRED_MANUAL_CRED_VALIDATION;


	// Create an SSPI credential.
	Status = ProxyManager::GetInstance()->SecurityFunTable->AcquireCredentialsHandle( NULL,                 // Name of principal    
		UNISP_NAME,         // Name of package
		SECPKG_CRED_OUTBOUND, // Flags indicating use
		NULL,                 // Pointer to logon ID
		&SchannelCred,        // Package specific data
		NULL,                 // Pointer to GetKey() func
		NULL,                 // Value to pass to GetKey()
		phCreds,              // (out) Cred Handle
		&tsExpiry );          // (out) Lifetime (optional)

    if (Status != SEC_E_OK)
        PROXYLOG(CELOG_EMERG, L"**** Error 0x%x returned by AcquireCredentialsHandle\n", Status);
    else
        PROXYLOG(CELOG_INFO, L"Create Client credential success.\n");

	// cleanup: Free the certificate context. Schannel has already made its own copy.
	if(pCertContext) 
		CertFreeCertificateContext(pCertContext);

	return Status;
}

// 0 Pkg name: Negotiate
// 1 Pkg name: NegoExtender
// 2 Pkg name: Kerberos
// 3 Pkg name: NTLM
// 4 Pkg name: TSSSP
// 5 Pkg name: pku2u
// 6 Pkg name: CloudAP
// 7 Pkg name: WDigest
// 8 Pkg name: Schannel
// 9 Pkg name: Microsoft Unified Security Protocol Provider
// 10 Pkg name: Default TLS SSP
SECURITY_STATUS CertificateHelper::CreateKrbCredForServer(PCredHandle phCred)
{
    SECURITY_STATUS Status;
    TimeStamp       tsExpiry;

    // Get a handle to the SSPI credential
    Status = ProxyManager::GetInstance()->SecurityFunTable->AcquireCredentialsHandle(
        NULL,                   // Name of principal
        //MICROSOFT_KERBEROS_NAME, // Name of package
		L"Negotiate",
        SECPKG_CRED_BOTH,    // Flags indicating use
        NULL,                   // Pointer to logon ID
        NULL,                   // Package specific data
        NULL,                   // Pointer to GetKey() func
        NULL,                   // Value to pass to GetKey()
        phCred,                 // (out) Cred Handle
        &tsExpiry);             // (out) Lifetime (optional)

    if (Status != SEC_E_OK)
    {
        DWORD dw = GetLastError();
        if (Status == SEC_E_UNKNOWN_CREDENTIALS)
            PROXYLOG(CELOG_EMERG, L"**** Error: 'Unknown Credentials' returned by CreateKrbCredForServer AcquireCredentialsHandle. Be sure app has administrator rights.");
        else
            PROXYLOG(CELOG_EMERG, L"**** Error 0x%x returned by CreateKrbCredForServer AcquireCredentialsHandle.", Status);
        return Status;
    }
    else
    {
        PROXYLOG(CELOG_INFO, L"Create Kerberos Server cred success.\n");
    }

    return SEC_E_OK;
}


SECURITY_STATUS CertificateHelper::CreateKrbCredForClient(PCredHandle phCred)
{
    SECURITY_STATUS Status;
    TimeStamp       tsExpiry;

    if (!theConfig.kerberosAuthOpen())
        return SEC_E_UNSUPPORTED_FUNCTION;

    if (theConfig.GetKrbUsername().empty() || theConfig.GetKrbPassword().empty() || theConfig.GetKrbDomain().empty() || theConfig.DatabaseSPN().empty())
    {
        PROXYLOG(CELOG_ERR, "windows_auth_username, windows_auth_password, domain or database_spn can not be empty.");
        return ERROR_INVALID_PARAMETER;
    }

    SEC_WINNT_AUTH_IDENTITY_W sec_auth = { 0 };

    //static const WCHAR* user = L"administrator";
    //static const WCHAR* domain = L"QAPF1.QALAB01.NEXTLABS.COM";
    //static const WCHAR* passwd = L"123blue!";

    sec_auth.User = (USHORT*)theConfig.GetKrbUsername().c_str();
    sec_auth.UserLength = theConfig.GetKrbUsername().length();
    sec_auth.Domain = (USHORT*)theConfig.GetKrbDomain().c_str();
    sec_auth.DomainLength = theConfig.GetKrbDomain().length();
    sec_auth.Password = (USHORT*)theConfig.GetKrbPassword().c_str();
    sec_auth.PasswordLength = theConfig.GetKrbPassword().length();
    sec_auth.Flags = SEC_WINNT_AUTH_IDENTITY_UNICODE;

    // Get a handle to the SSPI credential
    Status = ProxyManager::GetInstance()->SecurityFunTable->AcquireCredentialsHandle(
        NULL,                   // Name of principal
        MICROSOFT_KERBEROS_NAME, // Name of package
        SECPKG_CRED_OUTBOUND,    // Flags indicating use
        NULL,                   // Pointer to logon ID
        &sec_auth,                   // Package specific data
        NULL,                   // Pointer to GetKey() func
        NULL,                   // Value to pass to GetKey()
        phCred,                 // (out) Cred Handle
        &tsExpiry);             // (out) Lifetime (optional)

    if (Status != SEC_E_OK)
    {
        DWORD dw = GetLastError();
        if (Status == SEC_E_UNKNOWN_CREDENTIALS)
            PROXYLOG(CELOG_EMERG, L"**** Error: 'Unknown Credentials' returned by CreateKrbCredForClient AcquireCredentialsHandle. Be sure app has administrator rights.");
        else
            PROXYLOG(CELOG_EMERG, L"**** Error 0x%x returned by CreateKrbCredForClient AcquireCredentialsHandle.", Status);
        return Status;
    }
    else
    {
        PROXYLOG(CELOG_INFO, L"Create Kerberos Client cred success.\n");
    }

    return SEC_E_OK;
}