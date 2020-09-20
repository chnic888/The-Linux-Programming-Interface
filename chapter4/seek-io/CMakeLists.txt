cmake_minimum_required(VERSION 3.17)
project(seek-io C)

set(CMAKE_C_STANDARD 11)

add_executable(${PROJECT_NAME} main.c seek_io.h seek_io.c)