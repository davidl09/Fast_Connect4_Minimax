cmake_minimum_required(VERSION 3.23)
project(connect4 C)

set(CMAKE_C_STANDARD 11)

add_executable(connect4 main.c colours.h C4_game_engine.c C4_game_engine.h)
