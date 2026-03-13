# FindFFmpeg.cmake - Find FFmpeg libraries
# This module finds the following FFmpeg components:
#   avcodec, avformat, avutil, swscale, avdevice
#
# Variables defined by this module:
#   FFMPEG_FOUND         - True if all requested components were found
#   FFMPEG_INCLUDE_DIRS  - Include directories for FFmpeg
#   FFMPEG_LIBRARIES     - Libraries for all requested components
#   FFMPEG_VERSION       - Version of FFmpeg found
#
# For each component, the following variables are defined:
#   FFMPEG_<component>_FOUND    - True if the component was found
#   FFMPEG_<component>_LIBRARY  - Library for the component
#   FFMPEG_<component>_INCLUDE_DIR - Include directory for the component

set(FFMPEG_COMPONENTS avcodec avformat avutil swscale)

if(FFmpeg_FIND_COMPONENTS)
    set(FFMPEG_COMPONENTS ${FFmpeg_FIND_COMPONENTS})
endif()

# Try to find FFmpeg via pkg-config first
find_package(PkgConfig QUIET)
if(PKG_CONFIG_FOUND)
    pkg_check_modules(PC_FFMPEG libavcodec libavformat libavutil libswscale)
    if(PC_FFMPEG_FOUND)
        set(FFMPEG_VERSION ${PC_FFMPEG_VERSION})
    endif()
endif()

# Find include directory
find_path(FFMPEG_INCLUDE_DIR
    NAMES libavcodec/avcodec.h
    HINTS
        ${PC_FFMPEG_INCLUDE_DIRS}
        ${FFMPEG_ROOT}
        ENV FFMPEG_DIR
    PATH_SUFFIXES include
)

if(FFMPEG_INCLUDE_DIR)
    # Try to extract version from version.h
    find_file(FFMPEG_VERSION_HEADER
        NAMES libavutil/version.h
        HINTS ${FFMPEG_INCLUDE_DIR}
    )
    if(FFMPEG_VERSION_HEADER)
        file(STRINGS ${FFMPEG_VERSION_HEADER} FFMPEG_VERSION_MAJOR
            REGEX "^#define LIBAVUTIL_VERSION_MAJOR[ \t]+([0-9]+)")
        file(STRINGS ${FFMPEG_VERSION_HEADER} FFMPEG_VERSION_MINOR
            REGEX "^#define LIBAVUTIL_VERSION_MINOR[ \t]+([0-9]+)")
        file(STRINGS ${FFMPEG_VERSION_HEADER} FFMPEG_VERSION_MICRO
            REGEX "^#define LIBAVUTIL_VERSION_MICRO[ \t]+([0-9]+)")
        
        if(FFMPEG_VERSION_MAJOR AND FFMPEG_VERSION_MINOR AND FFMPEG_VERSION_MICRO)
            string(REGEX REPLACE "^#define LIBAVUTIL_VERSION_MAJOR[ \t]+([0-9]+)" "\\1"
                FFMPEG_VERSION_MAJOR "${FFMPEG_VERSION_MAJOR}")
            string(REGEX REPLACE "^#define LIBAVUTIL_VERSION_MINOR[ \t]+([0-9]+)" "\\1"
                FFMPEG_VERSION_MINOR "${FFMPEG_VERSION_MINOR}")
            string(REGEX REPLACE "^#define LIBAVUTIL_VERSION_MICRO[ \t]+([0-9]+)" "\\1"
                FFMPEG_VERSION_MICRO "${FFMPEG_VERSION_MICRO}")
            set(FFMPEG_VERSION "${FFMPEG_VERSION_MAJOR}.${FFMPEG_VERSION_MINOR}.${FFMPEG_VERSION_MICRO}")
        endif()
    endif()
endif()

# Find libraries for each component
foreach(COMPONENT ${FFMPEG_COMPONENTS})
    string(TOUPPER ${COMPONENT} UPPER_COMPONENT)
    
    find_library(FFMPEG_${UPPER_COMPONENT}_LIBRARY
        NAMES ${COMPONENT}
        HINTS
            ${PC_FFMPEG_LIBRARY_DIRS}
            ${FFMPEG_ROOT}
            ENV FFMPEG_DIR
        PATH_SUFFIXES lib lib64 bin
    )
    
    if(FFMPEG_${UPPER_COMPONENT}_LIBRARY)
        set(FFMPEG_${COMPONENT}_FOUND TRUE)
        list(APPEND FFMPEG_LIBRARIES ${FFMPEG_${UPPER_COMPONENT}_LIBRARY})
    else()
        set(FFMPEG_${COMPONENT}_FOUND FALSE)
    endif()
endforeach()

# Set include directories
set(FFMPEG_INCLUDE_DIRS ${FFMPEG_INCLUDE_DIR})

# Handle the QUIET and REQUIRED arguments
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(FFmpeg
    REQUIRED_VARS FFMPEG_INCLUDE_DIRS FFMPEG_LIBRARIES
    VERSION_VAR FFMPEG_VERSION
    HANDLE_COMPONENTS
)

mark_as_advanced(
    FFMPEG_INCLUDE_DIR
    FFMPEG_VERSION
    FFMPEG_LIBRARIES
)

foreach(COMPONENT ${FFMPEG_COMPONENTS})
    string(TOUPPER ${COMPONENT} UPPER_COMPONENT)
    mark_as_advanced(FFMPEG_${UPPER_COMPONENT}_LIBRARY)
endforeach()