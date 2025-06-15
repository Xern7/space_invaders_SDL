cmake_minimum_required(VERSION 3.10)
project(sdl-game)

set(CMAKE_CXX_STANDARD 17)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(sdl-game main.cpp)
target_link_libraries(sdl-game ${SDL2_LIBRARIES})

