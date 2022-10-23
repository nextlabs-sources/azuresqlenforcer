#ifndef CERTIFICATE_HELPER_H
#define CERTIFICATE_HELPER_H

#define SECURITY_WIN32

#include <windows.h>
#include <wincrypt.h>
#include <Sspi.h>
#include <schannel.h>
#include <cryptuiapi.h>

#ifndef SCH_USE_STRONG_CRYPTO // Needs KB 2868725 which is only in Windows 7+
#define SCH_USE_STRONG_CRYPTO                        0x00400000
#endif

	BOOL LoadSecurityLibrary();
	SECURITY_STATUS CertFindCertificateByName(LPCTSTR pszSubjectName);
	SECURITY_STATUS CreateCredentialsFromCertificate();

#endif

