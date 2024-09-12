cmake_minimum_required(VERSION 3.27)
project(minesweeper C)

set(CMAKE_C_STANDARD 11)

add_executable(minesweeper main.c
        functions.c
        functions.h)
