
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
SUBTARGET = DAECCPolicyMgr

OFFICIALCERT=0
DYNAMICCRT=yes
OTHERSRC_DIR=third_party/jsoncpp/src

SRC = $(wildcard ./src/*.cpp)
SRC += third_party/jsoncpp/src/json_reader.cpp \
  third_party/jsoncpp/src/json_value.cpp \
  third_party/jsoncpp/src/json_writer.cpp

RCSRC =	src/use_default.rc

INCPATH += \
  ./include \
  ./ \
  ./third_party/jsoncpp/include \
  ../DAECommon/include \
  $(NLEXTERNALDIR2)/boost/boost_1_67_0 \
  $(NLEXTERNALDIR)/openssl/OpenSSL-1.0.1e-Win64/include

LIBPATH += \
  $(NLEXTERNALDIR2)/boost/boost_1_67_0/lib64-msvc-14.0 \
  $(NLEXTERNALDIR)/openssl/OpenSSL-1.0.1e-Win64/lib

ifeq ($(BUILDTYPE), release)
LIBPATH +=
else
LIBPATH +=
endif

ifeq ($(BUILDTYPE), release)
LIBS +=
else
LIBS +=
endif

LIBS += ssleay32 libeay32 odbc32

ifeq ($(TARGETENVARCH), x64)
	DFLAGS += -D_M_X64 -D_ATL_STATIC_REGISTRY
else
	DFLAGS += -D_M_IX86 -D_ATL_STATIC_REGISTRY
endif

DFLAGS += -DHDB_POLICYMGR_IMP