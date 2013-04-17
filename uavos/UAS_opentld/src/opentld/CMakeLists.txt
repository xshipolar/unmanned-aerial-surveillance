link_directories(${OpenCV_LIB_DIR})

include_directories(main
    ../libopentld/imacq
    ../libopentld/mftracker
    ../libopentld/tld
    ../3rdparty/cvblobs
    ${OpenCV_INCLUDE_DIRS})

if(NOT USE_SYSTEM_LIBS)
    include_directories(../3rdparty/libconfig)
endif(NOT USE_SYSTEM_LIBS)

#-------------------------------------------------------------------------------
# main
add_library(main
    main/Config.cpp
    main/Gui.cpp
    main/Main.cpp
    main/Settings.cpp
    main/Config.h
    main/Gui.h
    main/Main.h
    main/Settings.h)

target_link_libraries(main libopentld cvblobs config++ ${OpenCV_LIBS})

#-------------------------------------------------------------------------------
# opentld
add_executable(opentld
    OpenTLD.cpp)

target_link_libraries(opentld main libopentld cvblobs config++ ${OpenCV_LIBS})

install(TARGETS opentld DESTINATION bin)

