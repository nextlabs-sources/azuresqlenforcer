project(bootstrap)

# LINK_DIRECTORIES(${OUT_PUT_SO} ) #sqlenforcer.lib
# LINK_DIRECTORIES(${OPENSSL_LIB_64_PATH}) 
# LINK_DIRECTORIES(${BOOST_LIB_PATH})

add_executable(${PROJECT_NAME}
	"src/main.cpp"
	"src/nanodbc.cpp"
	"src/DAEServiceMgr.cpp"
	"src/query_cloudaz_sdk_cpp_wrapper_class.cpp"
	"src/UdpSocket.cpp"
	"src/TDSConfigCheck.cpp"
	"src/SQLServerInstanceHelper.cpp"
	"src/CertificateHelper.cpp"
	"src/daeccpolicymgr_dll_wrapper_class.cpp"
	"src/WorkerPolicyCacheCheck.cpp")

set_target_properties(${PROJECT_NAME}  PROPERTIES LINK_FLAGS " /MANIFESTUAC:\"level='requireAdministrator' uiAccess='false'\" ")

target_include_directories(${PROJECT_NAME} PRIVATE
	"include"
)

target_link_libraries(${PROJECT_NAME} PRIVATE
	DAECommon
	sql2003
	DAECCPolicyMgr
	userattribute
)

find_package(Boost 1.67 REQUIRED COMPONENTS filesystem date_time regex)
find_package(OpenSSL 1.0 REQUIRED)
find_package(ODBC REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Boost::boost
	Boost::filesystem
	Boost::date_time
	Boost::regex
    OpenSSL::SSL
    ODBC::ODBC
	Crypt32
)
