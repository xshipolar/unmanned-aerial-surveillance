cmake_minimum_required(VERSION 2.6)
project(UAS_main)

# include directories
include_directories(${OPENTLD_INCLUDE_DIRS})

# create exectubale for file
add_library(UAS_serial UAS_serial.cpp)
add_library(UAS_comm UAS_comm.cpp)
add_library(utility utility.cpp)
add_library(UAS_ui UAS_ui.cpp)
add_library(datasys datasys.cpp)
add_library(comsys comsys.cpp)
add_library(visionsys visionsys.cpp)
add_library(uisys uisys.cpp)
add_library(navsys navsys.cpp)
add_executable(mainsys mainsys.cpp)


# link libraries to exectubale
target_link_libraries(mainsys comsys UAS_comm UAS_serial pthread uisys UAS_ui datasys navsys visionsys UAS_nav utility 
                      ${UAS_nav_LIBS} ${UAS_tld_LIBS} ${CURSES_LIBRARIES} ${OpenCV_LIBS} ${ARMADILLO_LIBRARIES} ${OPENTLD_LIBS})

set(CMAKE_BUILD_TYPE Release)