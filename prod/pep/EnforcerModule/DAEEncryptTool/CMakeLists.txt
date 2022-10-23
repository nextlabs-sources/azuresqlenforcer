project (dae_encrypt)

add_executable(${PROJECT_NAME} src/main.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE
	include
	third_party/CLI11/include
)

target_link_libraries(${PROJECT_NAME} PRIVATE DAECommon)
