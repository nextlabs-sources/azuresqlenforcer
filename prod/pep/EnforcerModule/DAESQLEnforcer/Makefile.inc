
VERSION_PRODUCT		= $(VERSION_PRODUCT_DAE)
VERSION_MAJOR		= $(VERSION_MAJOR_DAE)
VERSION_MINOR		= $(VERSION_MINOR_DAE)
VERSION_MAINTENANCE = $(VERSION_MAINTENANCE_DAE)
VERSION_PATCH		= $(VERSION_PATCH_DAE)

COMPILER=VS2015
PLATFORMSDK=8.1
CHARSET=ANSI

SUBDIRS   = 
SUBTARGET_STATIC = no
SUBTARGET = SQLEnforcer

OFFICIALCERT=0
DYNAMICCRT=yes
OTHERSRC_DIR=third_party/nanodbc

SRC = $(wildcard ./src/*.cpp)
SRC += third_party/nanodbc/nanodbc.cpp

RCSRC =	src/use_default.rc

INCPATH += \
  ./include \
  ./ \
  ./third_party/nanodbc/include \
  ../DAESQLParser/include \
  ../DAECCpolicymgr/include \
  ../DAEuserattributes/include \
  ../DAECommon/include \
  $(MSENFORCECOMMON)/prod/QueryCloudAZSDK_C++/QueryCloudAZSDKCpp/include \
  $(NLEXTERNALDIR2)/boost/boost_1_67_0

LIBPATH += \
  $(NLEXTERNALDIR2)/boost/boost_1_67_0/lib64-msvc-14.0

ifeq ($(BUILDTYPE), release)
LIBPATH += $(NLBUILDROOT)/bin/release_win_x64
else
LIBPATH += $(NLBUILDROOT)/bin/debug_win_x64
endif

LIBS += DAECommon_static Ole32 odbc32

ifeq ($(BUILDTYPE), release)
LIBS +=
else
LIBS +=
endif


ifeq ($(TARGETENVARCH), x64)
	DFLAGS += -D_M_X64 -D_ATL_STATIC_REGISTRY
else
	DFLAGS += -D_M_IX86 -D_ATL_STATIC_REGISTRY
endif

DFLAGS += -DDAE_SDK_EXPORT