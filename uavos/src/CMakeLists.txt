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
add_library(comsys comsys.cpp)
add_executable(uavos uavos.cpp)

# link boost_system and pthread to prevent error
target_link_libraries(uavos comsys pthread ${OpenCV_LIBS})
