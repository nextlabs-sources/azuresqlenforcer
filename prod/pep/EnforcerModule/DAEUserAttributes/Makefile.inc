
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
SUBTARGET = UserAttribute

OFFICIALCERT=0
DYNAMICCRT=yes

SRC = $(wildcard ./src/*.cpp)

RCSRC =	src/use_default.rc

INCPATH += \
  ./include \
  ./ \
  ../DAECommon/include \
  ../DAESQLParser/include \
  $(MSENFORCECOMMON)/prod/QueryCloudAZSDK_C++/QueryCloudAZSDKCpp/include

LIBPATH += \
  $(NLEXTERNALDIR2)/boost/boost_1_67_0/lib64-msvc-14.0

ifeq ($(BUILDTYPE), release)
LIBPATH += $(NLBUILDROOT)/bin/release_win_x64
else
LIBPATH += $(NLBUILDROOT)/bin/debug_win_x64
endif

LIBS += DAECommon_static

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

DFLAGS += -DENFORCER_EXPORT