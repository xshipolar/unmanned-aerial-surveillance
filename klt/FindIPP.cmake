cmake_policy(PUSH)
cmake_minimum_required(VERSION 2.6)
cmake_policy(POP)

#IF( NOT IPP_FOUND )

FILE(GLOB IPP_INCLUDE_PATHS_0 "$ENV{ProgramFiles}/Intel/IPP/*.*/include")
FILE(GLOB IPP_INCLUDE_PATHS_1 "/opt/intel/composerxe/ipp/*.*/include")
FILE(GLOB IPP_INCLUDE_PATHS_2 "/usr/local/intel/compoerxe/ipp/*.*/include")
FILE(GLOB IPP_INCLUDE_PATHS_3 "/opt/intel/ipp/*.*/include")

SET(
    IPP_INCLUDE_PATHS
    ${IPP_INCLUDE_PATHS_0}
    ${IPP_INCLUDE_PATHS_1}
    ${IPP_INCLUDE_PATHS_2}
    ${IPP_INCLUDE_PATHS_3}
)

FIND_PATH(
  IPP_INCLUDE_DIR
  ippi.h
  ${IPP_INCLUDE_PATHS}
)

SET(IPPROOT "${IPP_INCLUDE_DIR}/..")

        if(WIN32)
            FIND_PATH(IPP_PATH "ippi.dll"
                PATHS ${CMAKE_PROGRAM_PATH} ${CMAKE_SYSTEM_PROGRAM_PATH} ${IPPROOT}
                PATH_SUFFIXES bin
                DOC "The path to IPP dynamic libraries")
            if(NOT IPP_PATH)
                FIND_PATH(IPP_PATH "ippiem64t-${v}.dll"
                    PATHS ${CMAKE_PROGRAM_PATH} ${CMAKE_SYSTEM_PROGRAM_PATH}
                    PATH_SUFFIXES bin
                    DOC "The path to IPP dynamic libraries")
            endif()
        endif()
        if(UNIX)
            FIND_PATH(IPP_PATH "${CMAKE_SHARED_LIBRARY_PREFIX}ippi${CMAKE_SHARED_LIBRARY_SUFFIX}"
                PATHS ${CMAKE_LIBRARY_PATH} ${CMAKE_SYSTEM_LIBARY_PATH} ${IPPROOT} "${IPPROOT}/lib"
                PATH_SUFFIXES lib intel64 ia32
                DOC "The path to IPP dynamic libraries")
            message(STATUS "IPP_PATH = " ${IPP_PATH})
            if(NOT IPP_PATH)
                FIND_PATH(IPP_PATH "${CMAKE_SHARED_LIBRARY_PREFIX}ippiem64t${CMAKE_SHARED_LIBRARY_SUFFIX}"
                    PATHS ${CMAKE_LIBRARY_PATH} ${CMAKE_SYSTEM_LIBRARY_PATH} ${IPPROOT} "${IPPROOT}/lib"
                    PATH_SUFFIXES lib intel64 emt64
                    DOC "The path to IPP dynamic libraries")
                message(STATUS "IPP_PATH (emt64) = " ${IPP_PATH})
            endif()
        endif()
        if(IPP_PATH)
            file(GLOB IPP_HDRS "${IPP_PATH}/../include")
            if(IPP_HDRS)
                set(IPP_FOUND TRUE)
            endif()
        endif()

    set(IPP_LIBRARY_DIR ${IPP_PATH})
 
    add_definitions(-DHAVE_IPP)
    include_directories("${IPP_INCLUDE_DIR}")
    link_directories("${IPP_LIBRARY_DIR}")

    file(GLOB em64t_files "${IPP_PATH}/../lib/*em64t*")
    set(IPP_ARCH)
    if(em64t_files)
        set(IPP_ARCH "em64t")
    endif()

    set(A ${CMAKE_SHARED_LIBRARY_PREFIX})
    #set(B ${IPP_ARCH}${CMAKE_STATIC_LIBRARY_SUFFIX})
    set(B ${IPP_ARCH}${CMAKE_SHARED_LIBRARY_SUFFIX})
    if(WIN32)
        set(L l)
    else()
        set(L)
    endif()
    set(IPP_LIBS ${A}ippcore${B} ${A}ipps${B}  ${A}ippi${B})

#message(STATUS "IPPPATH = "  ${IPP_PATH})
#message(STATUS "IPP_LIBS ="  ${IPP_LIBS})

#ENDIF( NOT IPP_FOUND )
