cmake_minimum_required(VERSION 2.6)
project(onr)

find_package(Threads)
find_package(OpenCV REQUIRED)

# put executable in bin folder
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../bin)

# project include dir
include_directories( ${PROJECT_SOURCE_DIR}/../include )

# create exectubale for file
add_executable(onr onr.cpp)
add_executable(sift_test sift_test.cpp)
add_executable(surf_test surf_test.cpp)

# link boost_system and pthread to prevent error
target_link_libraries(onr pthread ${OpenCV_LIBS})
target_link_libraries(sift_test ${OpenCV_LIBS})
target_link_libraries(surf_test ${OpenCV_LIBS})
