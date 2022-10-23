
VERSION_PRODUCT		= $(VERSION_PRODUCT_DAE)
VERSION_MAJOR		= $(VERSION_MAJOR_DAE)
VERSION_MINOR		= $(VERSION_MINOR_DAE)
VERSION_MAINTENANCE = $(VERSION_MAINTENANCE_DAE)
VERSION_PATCH		= $(VERSION_PATCH_DAE)

COMPILER=VS2015
PLATFORMSDK=8.1

SUBDIRS   = 
SUBTARGET = SQLTask

OFFICIALCERT=0
DYNAMICCRT=yes

SRC = $(wildcard ./src/*.cpp)

RCSRC =	src/use_default.rc

INCPATH += \
  ./include \
  ./ \
  ../SQLTDS/include \
  ../ProxyCommon/include \
  $(MSENFORCECOMMON)/prod/celog/celog/include \
  $(MSENFORCECOMMON)/thirdpart/include \
  $(NLEXTERNALDIR2)/boost/boost_1_67_0 \
  $(NLBUILDROOT)/prod/pep/EnforcerModule/DAESQLEnforcer/include


LIBPATH += \
  $(BUILDOUTPUTDIR)/$(LIBDIR) \
  $(MSENFORCECOMMON)/bin/$(LIBDIR) \
  $(NLEXTERNALDIR2)/boost/boost_1_67_0/lib64-msvc-14.0

ifeq ($(BUILDTYPE), release)
LIBPATH += \
   $(MSENFORCECOMMON)/thirdpart/x64-windows/release/lib
else
LIBPATH += \
   $(MSENFORCECOMMON)/thirdpart/x64-windows/debug/lib
endif


LIBS += SQLTDS_static ProxyCommon_static celog

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
