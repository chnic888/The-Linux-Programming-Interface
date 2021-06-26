cmake_minimum_required(VERSION 3.17)
project(44-pipe-sync C)

include_directories(./include)
add_library(${PROJECT_NAME}_static STATIC src/pipe_sync.c)
add_executable(${PROJECT_NAME} main.c)
target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}_static)
