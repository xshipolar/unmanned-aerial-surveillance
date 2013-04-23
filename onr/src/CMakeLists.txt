cmake_minimum_required(VERSION 2.6)
project(video_sync)

#find_package(Threads)
#find_package(OpenCV REQUIRED)

# put executable in bin folder
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../lib)

# project include dir
include_directories( ${PROJECT_SOURCE_DIR}/../include )

# create exectubale for file
#add_executable(onr onr.cpp)
#add_executable(onr2 onr_systest.cpp)
add_executable(onr3 onr_3dm.cpp)
add_executable(gx3_25_port 3dm_gx3_25_sample_linux_driver.c)
#add_executable(gx3_25_test GX3_25_Test.c)
#add_executable(sift_test sift_test.cpp)
#add_executable(surf_test surf_test.cpp)
add_library(UAS_serial UAS_serial.cpp)

# link boost_system and pthread to prevent error
#target_link_libraries(onr pthread ${OpenCV_LIBS})
#target_link_libraries(onr2 UAS_serial pthread ${OpenCV_LIBS})
target_link_libraries(onr3 UAS_serial pthread ${MIP_SDK_LIBS} ${OpenCV_LIBS})
#target_link_libraries(gx3_25_test ${MIP_SDK_LIBS})
#target_link_libraries(sift_test ${OpenCV_LIBS})
#target_link_libraries(surf_test ${OpenCV_LIBS})
