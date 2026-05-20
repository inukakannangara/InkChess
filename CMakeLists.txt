cmake_minimum_required(VERSION 3.15)

project(InkChess)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(external/raylib)

file(GLOB_RECURSE SOURCES CMAKE_CONFIGURE_DEPENDS
    src/*.cpp
)

add_executable(InkChess
    ${SOURCES}
)

target_include_directories(InkChess PRIVATE include)

target_link_libraries(InkChess raylib)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT InkChess)