project (DAECommon)

# Collects the names of all the source files in the specified directory
# and stores the list in the variable provided.
# Note: When the source files is just added to the directory without modifing this file,
# we should manually rerun CMake.
aux_source_directory(./src SRCS)

add_library(${PROJECT_NAME} STATIC ${SRCS})

# All the directories following PUBLIC will be used for the current target and the other targets
# that have dependencies on the current target.
target_include_directories(${PROJECT_NAME} PUBLIC 
    ./include
    ${QAZJPC_INC_PATH}
)

find_package(Boost 1.67 REQUIRED)

# All the linked libraries following PRIVATE will be used for the current target only.
target_link_libraries(${PROJECT_NAME} PRIVATE
    Boost::boost
)
