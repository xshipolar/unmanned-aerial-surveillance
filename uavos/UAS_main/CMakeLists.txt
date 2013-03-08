cmake_minimum_required(VERSION 2.6)
project(UAS_main)

# create exectubale for file

add_library(UAS_serial UAS_serial.cpp)
add_library(UAS_comm UAS_comm.cpp)
add_library(utility utility.cpp)
add_library(UAS_ui UAS_ui.cpp)
add_library(datasys datasys.cpp)
add_library(comsys comsys.cpp)
add_library(visionsys visionsys.cpp)
add_library(uisys uisys.cpp)
add_executable(mainsys mainsys.cpp)

# link 
target_link_libraries(mainsys comsys UAS_comm UAS_serial pthread visionsys uisys UAS_ui datasys utility ${UAS_tld_LIBS} ${OpenCV_LIBS})

set(CMAKE_BUILD_TYPE Release)