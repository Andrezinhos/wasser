cmake_minimum_required(VERSION 3.10)
project(Wasser)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

include_directories(include/SDL2)
include_directories(include/headers)
include_directories(include/external)

link_directories(lib)

set(SOURCES
    src/player.cpp
    src/Media.cpp
    src/Playlist.cpp
    src/History.cpp
    src/Track.cpp
    src/Booster.cpp
    src/Boost.cpp
    src/Mixed.cpp
    src/EQ.cpp
)

add_executable(mediakit ${SOURCES})

target_link_libraries(mediakit SDL2 SDL2_mixer)
