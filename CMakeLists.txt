cmake_minimum_required(VERSION 3.22)
project(connect4 C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

include_directories(${CMAKE_SOURCE_DIR}/include)

link_directories(${CMAKE_SOURCE_DIR}/lib)

add_library(colours STATIC IMPORTED)

set_target_properties(colours PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/lib/libcolours.a)

add_executable(connect4 main.c C4_game_engine.c C4_game_engine.h)

target_link_libraries(connect4 -lcolours -lm)


