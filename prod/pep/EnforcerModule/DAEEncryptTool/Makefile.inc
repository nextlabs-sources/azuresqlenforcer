VERSION_PRODUCT		= $(VERSION_PRODUCT_DAE)
VERSION_MAJOR		= $(VERSION_MAJOR_DAE)
VERSION_MINOR		= $(VERSION_MINOR_DAE)
VERSION_MAINTENANCE = $(VERSION_MAINTENANCE_DAE)
VERSION_PATCH		= $(VERSION_PATCH_DAE)

COMPILER=VS2015
PLATFORMSDK=8.1

SUBDIRS   = 
TARGET = dae_encrypt

OFFICIALCERT=0
DYNAMICCRT=yes

SRC = $(wildcard ./src/*.cpp)

RCSRC =	src/use_default.rc

INCPATH += \
  ./ \
  ./include \
  ./third_party/CLI11/include \
  ../DAECommon/include

ifeq ($(BUILDTYPE), release)
LIBPATH += $(NLBUILDROOT)/bin/release_win_x64
else
LIBPATH += $(NLBUILDROOT)/bin/debug_win_x64
endif

LIBS += DAECommon_static

ifeq ($(TARGETENVARCH), x64)
	DFLAGS += -D_M_X64 -D_ATL_STATIC_REGISTRY
else
	DFLAGS += -D_M_IX86 -D_ATL_STATIC_REGISTRY
endif
