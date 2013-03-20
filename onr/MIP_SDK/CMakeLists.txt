cmake_minimum_required(VERSION 2.6)
project(MIP_SDK)

# set include dir
include_directories(${PROJECT_SOURCE_DIR}/../include)

# add libraries
add_library(mip                mip.cxx)
add_library(mip_sdk_3dm        mip_sdk_3dm.cxx)
add_library(mip_sdk_ahrs       mip_sdk_ahrs.cxx)
add_library(mip_sdk_base       mip_sdk_base.cxx)
add_library(mip_sdk_gps        mip_sdk_gps.cxx)
add_library(mip_sdk_interface  mip_sdk_interface.cxx)
add_library(mip_sdk_nav        mip_sdk_nav.cxx)
add_library(mip_sdk_system     mip_sdk_system.cxx)
add_library(ring_buffer        ring_buffer.cxx)
add_library(byteswap_utilities byteswap_utilities.cxx)

# set macro
set(MIP_SDK_LIBS mip_sdk_gps     mip_sdk_ahrs  mip_sdk_3dm       mip_sdk_base    byteswap_utilities
                 mip_sdk_system  mip_sdk_nav   mip_sdk_interface mip ring_buffer PARENT_SCOPE)