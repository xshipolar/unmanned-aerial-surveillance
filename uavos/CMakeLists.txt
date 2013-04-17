# project: UAS operationg system
# author: Xichen Shi
#         University of Illinois at Urbana-Champaign
#         Mar 05, 2013
cmake_minimum_required(VERSION 2.6)
project(UAS_OS)

###############################################################################
#                   Find required packages                                    #
###############################################################################
find_package(OpenCV REQUIRED)
find_package(Armadillo REQUIRED)
set(CURSES_NEED_NCURSES TRUE)
find_package(Curses REQUIRED)

###############################################################################
#                   General settings for all sub projects                     #
###############################################################################
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

option(BUILD_OPENTLD "Build the faster opentld library source."      OFF)
option(BUILD_OLDTLD  "Build the older tld library from source."      OFF)
option(BUILD_NAV     "Build the navigation library from source."     OFF)
option(BUILD_ALL     "Build all library and executable from source." OFF)
option(BUILD_MAIN    "Build main library from source."               ON)

###############################################################################
#                   Set the include include_directories                       #
###############################################################################
include_directories (${PROJECT_SOURCE_DIR}/include  
                     #${PROJECT_SOURCE_DIR}/include/UAS_tld 
                     ${PROJECT_SOURCE_DIR}/include/UAS_nav  
                     ${CURSES_INCLUDE_DIR}          
                     ${OpenCV_INCLUDE_DIRS}                
                     ${ARMADILLO_INCLUDE_DIRS})

link_directories    (${LIBRARY_OUTPUT_PATH})
###############################################################################
#                   Some temporary settings for openmp and gomp library       #
###############################################################################
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fopenmp -fPIC")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp -fPIC")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lgomp")

###############################################################################
#                   Proceed to sub projects                                   #
###############################################################################
if(BUILD_ALL)
    set(BUILD_OPENTLD ON)
    set(BUILD_NAV ON)
    set(BUILD_MAIN ON)
endif()

add_subdirectory(UAS_opentld)
add_subdirectory(UAS_nav)
add_subdirectory(UAS_main) 

get_cmake_property(_variableNames VARIABLES)
foreach (_variableName ${_variableNames})
    message(STATUS "${_variableName}=${${_variableName}}")
endforeach()