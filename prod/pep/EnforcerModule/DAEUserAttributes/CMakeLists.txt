project(userattribute)

add_definitions(-DENFORCER_EXPORT)

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