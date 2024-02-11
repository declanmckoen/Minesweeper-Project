cmake_minimum_required(VERSION 3.25)
project(Minesweeper)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp variables.h windows.h general.h textures.h textures.cpp board.h board.cpp tiles.h tiles.cpp digits.h digits.cpp)
add_executable(Minesweeper ${SOURCE_FILES})
include_directories(/Users/local/include)

find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)
include_directories(${SFML_INCLUDE_DIRS})
target_link_libraries(Minesweeper sfml-system sfml-window sfml-graphics sfml-audio sfml-network)
