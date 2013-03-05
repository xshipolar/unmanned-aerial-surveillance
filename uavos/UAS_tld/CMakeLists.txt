#Set minimum version requered
cmake_minimum_required(VERSION 2.6)
#just to avoid the warning
if(COMMAND cmake_policy)
     cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)
#set project name
project(TLD)

#Append path to the module path
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR})

#libraries
add_library(tld_utils tld_utils.cpp)
add_library(LKTracker LKTracker.cpp)
add_library(ferNN FerNNClassifier.cpp)
add_library(tld TLD.cpp)
add_library(UAS_tld UAS_tld.cpp)

set(UAS_tld_LIBS UAS_tld ferNN LKTracker tld_utils tld_utils)


