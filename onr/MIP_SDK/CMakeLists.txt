cmake_minimum_required(VERSION 2.6)
project(MIP_SDK)

# set include dir
include_directories(${PROJECT_SOURCE_DIR}/../include)

# add libraries
add_library(mip                mip.c)
add_library(mip_sdk_3dm        mip_sdk_3dm.c)
add_library(mip_sdk_ahrs       mip_sdk_ahrs.c)
add_library(mip_sdk_base       mip_sdk_base.c)
add_library(mip_sdk_gps        mip_sdk_gps.c)
add_library(mip_sdk_interface  mip_sdk_interface.c)
add_library(mip_sdk_nav        mip_sdk_nav.c)
add_library(mip_sdk_system     mip_sdk_system.c)
add_library(ring_buffer        ring_buffer.c)

# set macro
set{MIP_SDK_LIBS mip_sdk_gps     mip_sdk_ahrs  mip_sdk_3dm       mip_sdk_base 
                 mip_sdk_system  mip_sdk_nav   mip_sdk_interface mip ring_buffer PARENT_SCOPE}