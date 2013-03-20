# project: Code for ONR project
# author: Xichen Shi
#         University of Illinois at Urbana-Champaign
#         Mar 19, 2013
cmake_minimum_required(VERSION 2.6)
project(ONR)

# find required packages
find_package(OpenCV REQUIRED)

# set build directory
set(CMAKE_CURRENT_BINARY_DIR ${PROJECT_SOURCE_DIR}/build)

# general settings for all sub projects
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
    
# set the include directories
include_directories (${PROJECT_SOURCE_DIR}/include ${OpenCV_INCLUDE_DIRS})

# add sub projects
add_subdirectory(MIP_SDK)
add_subdirectory(src)