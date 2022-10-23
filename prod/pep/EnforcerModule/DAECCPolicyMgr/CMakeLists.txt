project (DAECCPolicyMgr)

add_definitions(-DHDB_POLICYMGR_IMP)


aux_source_directory(./src SRCS)
aux_source_directory(./third_party/jsoncpp/src SRCS_JSCPP)

add_library(DAECCPolicyMgr SHARED
    ${SRCS}
    ${SRCS_JSCPP}
)

target_include_directories(${PROJECT_NAME} PUBLIC
    include
    third_party/dirent/include
    third_party/jsoncpp/include
)

find_package(Boost 1.67 REQUIRED COMPONENTS system date_time)
find_package(OpenSSL 1.0 REQUIRED)
find_package(ODBC REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Boost::boost
    Boost::system
    Boost::date_time
    OpenSSL::SSL
    ODBC::ODBC
)
