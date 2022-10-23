project (sql2003)

add_definitions(-DYY_NO_UNISTD_H)
add_definitions(-DSQLPARSER_EXPORT)

aux_source_directory(./src SRCS)

add_library(${PROJECT_NAME} SHARED
	${SRCS}
)

target_include_directories(${PROJECT_NAME} PUBLIC
	include
)

target_link_libraries(${PROJECT_NAME} PRIVATE
	DAECommon
)
