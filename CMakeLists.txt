cmake_minimum_required(VERSION 2.8)

# ALL OF THE SPECIFIED NAMES IN THIS CMAKE ARE REQUIRED.
# Set the project name variable to hw2.
# THIS IS REQUIRED
project(hw2)

# Add your CMAKE_C_FLAGS AND CMAKE_CXX_FLAGS here
# Make SURE to use -std=c11 and -std=c++11 where relevant.
# The options specified below are required, but additional options
#  may be used.
set(CMAKE_C_FLAGS "-std=c11 -Wall -Wextra -Wshadow -Werror")
set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wextra -Wshadow -Werror")

# Add our include directory to CMake's search paths.
# THIS IS REQUIRED
include_directories( include )

message(STATUS "Binding 'source' directory")
set( LEARN_CMAKE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
message(STATUS "Binding 'include' directory")
set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/include/" )



# Create library from dyn_array so we can use it later.
add_library(dyn_array SHARED src/dyn_array.c)


# Compile the analysis executable.

add_executable(analysis ${CMAKE_SOURCE_DIR}/src/analysis.c ${CMAKE_SOURCE_DIR}/include/processing_scheduling.h ${CMAKE_SOURCE_DIR}/src/process_scheduling.c)


# link the dyn_array library we compiled against our analysis executable.
target_link_libraries(analysis dyn_array)


# Compile the the tester executable.
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
add_executable(${PROJECT_NAME}_test ${CMAKE_SOURCE_DIR}/test/tests.cpp ${CMAKE_SOURCE_DIR}/include/processing_scheduling.h ${CMAKE_SOURCE_DIR}/src/process_scheduling.c)


target_link_libraries(analysis dyn_array)

target_compile_definitions(${PROJECT_NAME}_test PRIVATE)

# Link ${PROJECT_NAME}_test with dyn_array and gtest and pthread libraries
target_link_libraries(${PROJECT_NAME}_test gtest pthread dyn_array)
