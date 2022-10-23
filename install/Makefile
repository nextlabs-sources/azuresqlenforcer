include $(NLBUILDROOT)/build/Makefile.ver

VERSION_PRODUCT				= $(VERSION_PRODUCT_DAE)
VERSION_MAJOR					= $(VERSION_MAJOR_DAE)
VERSION_MINOR					= $(VERSION_MINOR_DAE)
VERSION_MAINTENANCE 	= $(VERSION_MAINTENANCE_DAE)
VERSION_PATCH					= $(VERSION_PATCH_DAE)

CURRENT_MKFILE_PATH    := $(abspath $(lastword $(MAKEFILE_LIST)))
CURRENT_MKFILE_DIRPATH := $(dir $(CURRENT_MKFILE_PATH))

ifeq ($(ProgramW6432), )
	ENV_OS=x86
	PROGRAM_FILES_X86=C:/Program Files
else
	ENV_OS=x64
	PROGRAM_FILES_X86=C:/Program Files (x86)
endif

PROGRAM_FILES_X86_DOS=$(subst /,\,$(PROGRAM_FILES_X86))

PERL=/usr/bin/perl
ISCMDBLD=$(PROGRAM_FILES_X86)/InstallShield/2014 SAB/System/IsCmdBld.exe

UNIX_NLBUILDROOT=$(subst \,/,$(NLBUILDROOT))
INSTALL_DIR=$(UNIX_NLBUILDROOT)/install
SCRIPT_DIR=$(UNIX_NLBUILDROOT)/install
INSTALL_BUILD_DIR=$(INSTALL_DIR)/build
ASSEMBLY_DIR=$(INSTALL_BUILD_DIR)/data

EMPTY:=
SPACE:= $(EMPTY) $(EMPTY)
VERSION_PRODUCT_WITHOUT_SPACE=$(subst $(SPACE),$(EMPTY),$(VERSION_PRODUCT))
MSI_FILE64=$(VERSION_PRODUCT).msi
MSI_FILE64_WITHOUT_SPACE=$(VERSION_PRODUCT_WITHOUT_SPACE).msi
PACKAGE64_DIR=$(INSTALL_BUILD_DIR)/package64
VERSION_STR=$(VERSION_MAJOR).$(VERSION_MINOR).$(VERSION_MAINTENANCE)
PACKAGE_FILE64:=$(basename $(MSI_FILE64_WITHOUT_SPACE))-$(VERSION_STR).$(BUILD_NUMBER)-$(shell date +"%Y%m%d%H%M").zip
ISM_FILE_DIR=$(NLBUILDROOT)/install
ISM_FILE_DIR_DOS=$(subst /,\,$(ISM_FILE_DIR))
ISM_TEMPLATE_FILE_NAME_X64=DAEforSQLServer.ism
ISM_OUTPUT_DIR=$(ISM_FILE_DIR)/output
# Ensure ISM_DEPLOY_DIR is a Windows-style file path
ISM_DEPLOY_DIR=$(subst /,\,$(ISM_OUTPUT_DIR))
PUBLISH_DIR=$(ISM_FILE_DIR)/publish
MSI_FOLDER_PATH=$(ISM_OUTPUT_DIR)/PROJECT_ASSISTANT/SINGLE_MSI_IMAGE/DiskImages/DISK1
MSI_FILE_PATH=$(MSI_FOLDER_PATH)/$(MSI_FILE64)
ARCHIVE_FILE64:=$(basename $(MSI_FILE64_WITHOUT_SPACE))-$(VERSION_STR).$(BUILD_NUMBER)-$(shell date +"%Y%m%d%H%M")-bin.zip
PACKAGE_ARCHIVE_PATH=s:/build/release_candidate/$(VERSION_PRODUCT_WITHOUT_SPACE)/$(VERSION_STR)/$(VERSION_BUILD_SHORT)/
BIN_ARCHIVE_PATH=s:/build/release_candidate/artifacts/$(VERSION_PRODUCT_WITHOUT_SPACE)/$(VERSION_BUILD_SHORT)/
BIN_ARCHIVE_PATH_DOS=$(subst /,\,$(BIN_ARCHIVE_PATH))

EETEMPDIR := $(shell mktemp -d)

#
# Print environment
#

$(info -----------------------------------------------------------------------------)
$(info BUILDTYPE                   = $(BUILDTYPE))
$(info VERSION_BUILD               = $(VERSION_BUILD))
$(info VERSION_BUILD_SHORT         = $(VERSION_BUILD_SHORT))
$(info BUILD_NUMBER                = $(BUILD_NUMBER))
$(info OFFICIALCERT                = $(OFFICIALCERT))
$(info Product directory name      = $(PRODUCT_DIR_NAME))
$(info Packaging list              = $(PACKAGE_LIST))
$(info Install directory           = $(INSTALL_DIR))
$(info Assembly directory          = $(ASSEMBLY_DIR))
$(info Output directory            = $(OUTPUT_DIR))
$(info Package directory (64-bit)  = $(PACKAGE64_DIR))
$(info Package file (64-bit)       = $(PACKAGE_FILE64))
$(info Publish directory           = $(PUBLISH_DIR))
$(info Current OS                  = $(ENV_OS))
$(info Program files (x86)         = $(PROGRAM_FILES_X86))
$(info Program files (x86) DOS     = $(PROGRAM_FILES_X86_DOS))
$(info ISM_FILE_DIR                = $(ISM_FILE_DIR))
$(info ISM_FILE_DIR_DOS            = $(ISM_FILE_DIR_DOS))
$(info ISM_TEMPLATE_FILE_NAME_X86  = $(ISM_TEMPLATE_FILE_NAME_X86))
$(info ISM_TEMPLATE_FILE_NAME_X64  = $(ISM_TEMPLATE_FILE_NAME_X64))
$(info ISM_OUTPUT_DIR              = $(ISM_OUTPUT_DIR))
$(info ISM_DEPLOY_DIR              = $(ISM_DEPLOY_DIR))
$(info ISM release                 = $(ISM_RELEASE))
$(info ISM product config (64-bit) = $(ISM_PRODUCT_CONFIG64))
$(info ISM output directory        = $(DEPLOY_DIR))
$(info Archive package (64-bit)    = $(ARCHIVE_FILE64))
$(info Archive package path        = $(PACKAGE_ARCHIVE_PATH))
$(info Archive bin path            = $(BIN_ARCHIVE_PATH))
$(info Archive bin path DOS        = $(BIN_ARCHIVE_PATH_DOS))
$(info -----------------------------------------------------------------------------)

# Ensure that INSTALL_SOURCE_DIR is a UINIX-style file path, which ends with a slash, i.e. /, here.
# Put it in single or double quotes when using it in shell commands because it contains a space
INSTALL_SOURCE_DIR=$(INSTALL_DIR)/source

BIN_RELEASE_WIN_X64=$(UNIX_NLBUILDROOT)/bin/release_win_x64

all: preparefile install64 publish archive

preparefile:
	@echo "INFO: Prepare assembly for file info"
	mkdir -p '$(INSTALL_SOURCE_DIR)'

	cp -rpu '$(BIN_RELEASE_WIN_X64)/config' '$(INSTALL_SOURCE_DIR)'
	cp -pu '$(BIN_RELEASE_WIN_X64)/bootstrap.exe' '$(INSTALL_SOURCE_DIR)'
	cp -pu '$(BIN_RELEASE_WIN_X64)/celog.dll' '$(INSTALL_SOURCE_DIR)'
	cp -pu '$(BIN_RELEASE_WIN_X64)/DAECCPolicyMgr.dll' '$(INSTALL_SOURCE_DIR)'
	cp -pu '$(BIN_RELEASE_WIN_X64)/frame.dll' '$(INSTALL_SOURCE_DIR)'
	cp -pu '$(BIN_RELEASE_WIN_X64)/jsoncpp.dll' '$(INSTALL_SOURCE_DIR)'
	cp -pu '$(BIN_RELEASE_WIN_X64)/LIBEAY32.dll' '$(INSTALL_SOURCE_DIR)'
	cp -pu '$(BIN_RELEASE_WIN_X64)/ProxyMain.exe' '$(INSTALL_SOURCE_DIR)'
	cp -pu '$(BIN_RELEASE_WIN_X64)/ProxyWorker.exe' '$(INSTALL_SOURCE_DIR)'
	cp -pu '$(BIN_RELEASE_WIN_X64)/QueryCloudAZSDKcpp.dll' '$(INSTALL_SOURCE_DIR)'
	cp -pu '$(BIN_RELEASE_WIN_X64)/SQL2003.dll' '$(INSTALL_SOURCE_DIR)'
	cp -pu '$(BIN_RELEASE_WIN_X64)/SQLEnforcer.dll' '$(INSTALL_SOURCE_DIR)'
	cp -pu '$(BIN_RELEASE_WIN_X64)/SQLProxy.dll' '$(INSTALL_SOURCE_DIR)'
	cp -pu '$(BIN_RELEASE_WIN_X64)/SQLTask.dll' '$(INSTALL_SOURCE_DIR)'
	cp -pu '$(BIN_RELEASE_WIN_X64)/SSLEAY32.dll' '$(INSTALL_SOURCE_DIR)'
	cp -pu '$(BIN_RELEASE_WIN_X64)/UserAttribute.dll' '$(INSTALL_SOURCE_DIR)'
	cp -pu '$(BIN_RELEASE_WIN_X64)/zlib1.dll' '$(INSTALL_SOURCE_DIR)'

	cp -pu '$(INSTALL_DIR)/tool/dae_encrypt.exe' '$(INSTALL_SOURCE_DIR)'

# Sign all binary files (.dll, .exe)
	cd '$(INSTALL_SOURCE_DIR)' && cmd /C '$(NLBUILDROOT)/build/signModulesByServer256Only.bat'

install64: preparefile
	@echo "INFO: Create 64-bit install package"
	mkdir -p '$(ISM_OUTPUT_DIR)'
	'$(ISCMDBLD)' -x -p "$(ISM_FILE_DIR)/$(ISM_TEMPLATE_FILE_NAME_X64)" -b "$(ISM_DEPLOY_DIR)" -y "$(VERSION_STR).$(VERSION_BUILD_SHORT)"

# Sign msi file
	cd $(MSI_FOLDER_PATH) && cmd /C '$(NLBUILDROOT)/build/signModulesByServer256Only.bat'

publish:install64
	@echo "INFO: Publish install packages"
	@if [ "${MSI_FILE64}" != "" ]; then \
		mkdir -p '$(PUBLISH_DIR)' ; \
		cp -pf '$(MSI_FILE_PATH)' '$(PUBLISH_DIR)/$(MSI_FILE64_WITHOUT_SPACE)' ; \
		cp -pf '$(BIN_RELEASE_WIN_X64)/tdsproxy.pinta.com.pfx' '$(PUBLISH_DIR)' ; \
		cp -pf '$(INSTALL_SOURCE_DIR)/dae_encrypt.exe' '$(PUBLISH_DIR)' ; \
		cp -pf '$(INSTALL_SOURCE_DIR)/config/config.ini' '$(PUBLISH_DIR)' ; \
		cd '$(PUBLISH_DIR)'; zip '$(ISM_FILE_DIR_DOS)\$(PACKAGE_FILE64)' * ; \
	fi

archive:publish
	@echo "INFO: Archive install packages"
	cd '$(ISM_FILE_DIR)' ; \
	mkdir -p $(PACKAGE_ARCHIVE_PATH) ; \
	cp -pf '$(ISM_FILE_DIR)/$(PACKAGE_FILE64)' '$(PACKAGE_ARCHIVE_PATH)' ; \
	ls -l '$(PACKAGE_ARCHIVE_PATH)' ; \
	mkdir -p $(BIN_ARCHIVE_PATH) ; \
	cd '$(BIN_RELEASE_WIN_X64)' ; \
	zip '$(ISM_FILE_DIR_DOS)\$(ARCHIVE_FILE64)' * ; \
	xcopy '$(ISM_FILE_DIR_DOS)\$(ARCHIVE_FILE64)' '$(BIN_ARCHIVE_PATH_DOS)' || echo ![-_-]xcopy failed with $$? ; \
	ls -l '$(BIN_ARCHIVE_PATH)' ; \
