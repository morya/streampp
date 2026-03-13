cmake_minimum_required(VERSION 3.25)
project(ScreenStreamer VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(WIN32)
    add_definitions(-DWIN32_LEAN_AND_MEAN)
    add_definitions(-DNOMINMAX)
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

find_package(Qt6 6.10 REQUIRED COMPONENTS Core Widgets Multimedia Gui)

find_package(FFmpeg REQUIRED COMPONENTS avcodec avformat avutil swscale)

include_directories(
    ${CMAKE_SOURCE_DIR}/src
    ${FFMPEG_INCLUDE_DIRS}
)

file(GLOB_RECURSE SOURCES 
    "src/*.cpp"
    "src/core/*.cpp"
    "src/core/*/*.cpp"
    "src/ui/*.cpp"
    "src/utils/*.cpp"
)

file(GLOB_RECURSE HEADERS 
    "src/*.h"
    "src/core/*.h"
    "src/core/*/*.h"
    "src/ui/*.h"
    "src/utils/*.h"
)

add_executable(ScreenStreamer ${SOURCES} ${HEADERS})

target_link_libraries(ScreenStreamer
    Qt6::Core
    Qt6::Widgets
    Qt6::Multimedia
    Qt6::Gui
    ${FFMPEG_LIBRARIES}
)

if(WIN32)
    target_link_libraries(ScreenStreamer ws2_32)
endif()

set_target_properties(ScreenStreamer PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE FALSE
)

install(TARGETS ScreenStreamer RUNTIME DESTINATION bin)

if(WIN32)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++")
endif()