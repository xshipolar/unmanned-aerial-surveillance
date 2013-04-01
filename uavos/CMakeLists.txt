# project: UAS operationg system
# author: Xichen Shi
#         University of Illinois at Urbana-Champaign
#         Mar 05, 2013
cmake_minimum_required(VERSION 2.6)
project(UAS_OS)

# find required packages
find_package(OpenCV REQUIRED)
find_package(Armadillo REQUIRED)
set(CURSES_NEED_NCURSES TRUE)
find_package(Curses REQUIRED)

# set build directory
set(CMAKE_CURRENT_BINARY_DIR ${PROJECT_SOURCE_DIR}/build)

# general settings for all sub projects
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

#set the include directories
include_directories (${PROJECT_SOURCE_DIR}/include  ${PROJECT_SOURCE_DIR}/include/UAS_tld ${PROJECT_SOURCE_DIR}/include/UAS_nav  
                     ${CURSES_INCLUDE_DIR}          ${OpenCV_INCLUDE_DIRS}                ${ARMADILLO_INCLUDE_DIRS})

# add sub projects
add_subdirectory(UAS_tld)
add_subdirectory(UAS_nav)
add_subdirectory(UAS_main)
