# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.14)

project ("DAESqlServer")

set(CMAKE_CXX_FLAGS " -Wall  ${CMAKE_CXX_FLAGS}")

add_definitions(-DNANODBC_THROW_NO_SOURCE_LOCATION)

IF(NOT DEFINED OUT_PUT_SO)
    IF(CMAKE_CL_64)
		IF (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
			SET(OUT_PUT_SO ${CMAKE_CURRENT_SOURCE_DIR}/../../bin/debug_win_x64)
			
		ELSE()
			SET(OUT_PUT_SO ${CMAKE_CURRENT_SOURCE_DIR}/../../bin/release_win_x64)
		ENDIF()
    ELSE(CMAKE_CL_64)
		IF (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
			SET(OUT_PUT_SO ${CMAKE_CURRENT_SOURCE_DIR}/../../bin/debug_win_x86)
		ELSE()
			SET(OUT_PUT_SO ${CMAKE_CURRENT_SOURCE_DIR}/../../bin/release_win_x86)
		ENDIF()
    ENDIF(CMAKE_CL_64)
    file(MAKE_DIRECTORY ${OUT_PUT_SO})
    SET(OUT_PUT_SO_CONFIG ${OUT_PUT_SO}/Config)
    file(MAKE_DIRECTORY ${OUT_PUT_SO_CONFIG})
ENDIF(NOT DEFINED OUT_PUT_SO)

MESSAGE( ${OUT_PUT_SO})
MESSAGE( ${OUT_PUT_SO_CONFIG})

add_definitions(-DDAE_ORACLE) #Specific definition for  DAE for Oracle
add_definitions(-DCC_PREFILTER)
add_definitions(-D_CRT_SECURE_NO_WARNINGS)

SET(QAZJPC_INC_PATH "$ENV{MSENFORCECOMMON}/prod/QueryCloudAZSDK_C++/QueryCloudAZSDKCpp/include")

# Set hints to find Boost
set (BOOST_ROOT "$ENV{NLEXTERNALDIR}/boost/boost_1_67_0")
set (BOOST_LIBRARYDIR "$ENV{NLEXTERNALDIR}/boost/boost_1_67_0/lib64-msvc-14.0")
set (Boost_USE_STATIC_LIBS ON)

# Set hints to find OpenSSL
set (OPENSSL_ROOT_DIR "$ENV{NLEXTERNALDIR}/openssl/OpenSSL-1.0.1e-Win64")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUT_PUT_SO})

IF (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG 	${OUT_PUT_SO})   # output lib file
ELSE()
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE 	${OUT_PUT_SO})   # output lib file
ENDIF()

# Include sub-projects.
add_subdirectory (DAEBootstrap)
add_subdirectory (DAECommon)
# add_subdirectory (DAEEncryptTool) # We change back into use the old tool for the consistent between DAE products.
add_subdirectory (DAECCPolicyMgr)
add_subdirectory (DAESQLParser)
add_subdirectory (DAEUserAttributes)
add_subdirectory (DAESQLEnforcer)
# add_subdirectory (DAEUnittest)

# configure_file(./source/config.ini ${OUT_PUT_SO_CONFIG} COPYONLY)

IF (CMAKE_BUILD_TYPE MATCHES "Debug")
SET(QAZJPC_DLL_PATH "$ENV{MSENFORCECOMMON}/prod/QueryCloudAZSDK_C++/QueryCloudAZSDKCpp/debug_win_x64/QueryCloudAZSDKCpp.dll")
ELSE()
SET(QAZJPC_DLL_PATH "$ENV{MSENFORCECOMMON}/prod/QueryCloudAZSDK_C++/QueryCloudAZSDKCpp/release_win_x64/QueryCloudAZSDKCpp.dll")
ENDIF()

file(COPY ${QAZJPC_DLL_PATH} DESTINATION ${OUT_PUT_SO})
