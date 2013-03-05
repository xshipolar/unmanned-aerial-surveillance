cmake_minimum_required(VERSION 2.6)
project(uavos)

find_package(Threads)
find_package(OpenCV REQUIRED)

# put executable in bin folder and library in lib folder
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../lib)

# project include dir
include_directories( ${PROJECT_SOURCE_DIR}/../include )

# create exectubale for file

add_library(UAS_serial ../include/UAS_serial/UAS_serial.cpp)
add_library(UAS_comm UAS_comm.cpp)
add_library(utility utility.cpp)
add_library(sensor sensor.cpp)
add_library(comsys comsys.cpp)
add_executable(mainsys mainsys.cpp)

# link 
target_link_libraries(mainsys comsys UAS_comm UAS_serial sensor utility pthread ${OpenCV_LIBS})
