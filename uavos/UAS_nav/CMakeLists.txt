cmake_minimum_required(VERSION 2.6)
project(UAS_nav)

add_library(UAS_nav UAS_nav.cpp)

set(UAS_nav_LIBS UAS_nav PARENT_SCOPE)