# Open The CMake GUI
# specify the source directory and the binary directory
# press configure
# set CMAKE_INSTALL_PREFIX to the path where you want to install the program
# press configure
# check BUILD_WITH_QT if you want to build the program with a QT-Config GUI
# check GENERATE_DEB_PACKAGE if you want to build a debian package (only on Linux)
#
# UNIX Makefile:
# 1) go to the binary folder and type "make" to build the project
# 2) (optional) type "make install all" to install the files into the install
#    directory
# 3) (optional) type "make package" to create a package of the install folder
#    (.tgz file if GENERATE_DEB_PACKAGE=false, .deb file if GENERATE_DEB_PACKAGE=true)
#
# Microsoft Visual C++:
# 1) open the .sln file
# 2) change the mode to "Release" -> only Release is supported!
# 3) build the project "ALL_BUILD" to build the opentld project
# 4) build the project "INSTALL" to install the files into the install
#    directory
# 5) build the project "PACKAGE" to create an NSIS-installer (NSIS is required)


project(UAS_opentld)
cmake_minimum_required(VERSION 2.6)

find_package(OpenCV REQUIRED)
#------------------------------------------------------------------------------
#build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING
        "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
        FORCE)
endif(NOT CMAKE_BUILD_TYPE)

option(USE_SYSTEM_LIBS "Use the installed version of libconfig++." OFF)

if(WIN32)
    add_definitions(-DLIBCONFIGXX_STATIC -DLIBCONFIG_STATIC)
endif(WIN32)

if(APPLE)
    add_definitions(-DHAVE_XLOCALE_H)
endif(APPLE)

###############################################################################
#                            add subdirectories                               #
###############################################################################
if(BUILD_OPENTLD)
    add_subdirectory(src/3rdparty/cvblobs)
    if(NOT USE_SYSTEM_LIBS)
        add_subdirectory(src/3rdparty/libconfig)
    endif(NOT USE_SYSTEM_LIBS)
    add_subdirectory(src/libopentld)
    add_subdirectory(src/opentld)
endif()

###############################################################################
#                            Setting libs and includes                        #
###############################################################################

set(OPENTLD_LIBS main libopentld cvblobs config++ PARENT_SCOPE)
set(OPENTLD_INCLUDE_DIRS 
    ${PROJECT_SOURCE_DIR}/src/opentld/main
    ${PROJECT_SOURCE_DIR}/src/libopentld/imacq
    ${PROJECT_SOURCE_DIR}/src/libopentld/mftracker
    ${PROJECT_SOURCE_DIR}/src/libopentld/tld
    ${PROJECT_SOURCE_DIR}/src/3rdparty/cvblobs
    PARENT_SCOPE)

configure_file("${PROJECT_SOURCE_DIR}/OpenTLDConfig.cmake.in" "${PROJECT_BINARY_DIR}/OpenTLDConfig.cmake" @ONLY)
