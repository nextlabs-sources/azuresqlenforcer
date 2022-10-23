
VERSION_PRODUCT		= $(VERSION_PRODUCT_DAE)
VERSION_MAJOR		= $(VERSION_MAJOR_DAE)
VERSION_MINOR		= $(VERSION_MINOR_DAE)
VERSION_MAINTENANCE = $(VERSION_MAINTENANCE_DAE)
VERSION_PATCH		= $(VERSION_PATCH_DAE)

COMPILER=VS2015
PLATFORMSDK=8.1

SUBDIRS   = 
CSTARGET = NXL2RLS

OFFICIALCERT=0
DYNAMICCRT=yes

SRC = *.cs
      
CSFLAGS += -optimize
CSLIB += -lib:$(MSENFORCECOMMON)/bin/$(LIBDIR) -lib:$(NLBUILDROOT)/prod/pep/AzureSQLPEP

REFERENCE = /r:QueryCloudAZSDKCS.dll
