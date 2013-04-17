cmake_minimum_required(VERSION 2.6)
project(UAS_nav)

if(BUILD_NAV)
    add_library(UAS_nav UAS_nav.cpp)
endif()

set(UAS_nav_LIBS UAS_nav PARENT_SCOPE)