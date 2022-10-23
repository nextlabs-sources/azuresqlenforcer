project(sqlenforcer)

add_definitions(-DDAE_SDK_EXPORT)

aux_source_directory(./src SRCS)
aux_source_directory(./third_party/nanodbc SRCS_NANODBC)

add_library(${PROJECT_NAME} SHARED
	${SRCS}
	${SRCS_NANODBC}
)

target_include_directories(${PROJECT_NAME} PUBLIC
	include
	third_party
)

find_package(Boost 1.67 REQUIRED COMPONENTS log)
find_package(ODBC REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE
	DAECommon
	DAECCPolicyMgr
	userattribute
	sql2003
	Boost::log
	ODBC::ODBC
)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/include/DAESqlServerSDK.h DESTINATION ${OUT_PUT_SO}) #output handler file
