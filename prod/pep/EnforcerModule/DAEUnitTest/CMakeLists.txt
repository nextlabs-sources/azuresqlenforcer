project(DAEUnittest)

add_executable (DAEUnittest "enforcerwrapper.cpp" "DAESQLEnforcer.cpp")

target_link_libraries(${PROJECT_NAME} PRIVATE
	DAECommon
	DAECCPolicyMgr
	sqlenforcer
)
