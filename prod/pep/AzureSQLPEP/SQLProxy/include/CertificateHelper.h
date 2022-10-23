#ifndef CERTIFICATE_HELPER_H
#define CERTIFICATE_HELPER_H

#define SECURITY_WIN32

#include <windows.h>
#include <wincrypt.h>
#include <Security.h>
#include <schannel.h>
#include <cryptuiapi.h>

#ifndef SCH_USE_STRONG_CRYPTO // Needs KB 2868725 which is only in Windows 7+
#define SCH_USE_STRONG_CRYPTO                        0x00400000
#endif

namespace CertificateHelper
{
    // TLS cred
	SECURITY_STATUS CertFindCertificateByName(PCCERT_CONTEXT & pCertContext, LPCTSTR pszSubjectName);
	SECURITY_STATUS CreateCredentialsFromCertificate(PCredHandle phCreds, PCCERT_CONTEXT pCertContext);
	SECURITY_STATUS CreateCredentialsForClient(LPSTR pszUser, PCredHandle phCreds );

    // Kerberos cred
    SECURITY_STATUS CreateKrbCredForServer(PCredHandle phCred);
    SECURITY_STATUS CreateKrbCredForClient(PCredHandle phCred);
};

#endif

