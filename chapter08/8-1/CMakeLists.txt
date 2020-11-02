cmake_minimum_required(VERSION 3.17)
project(8-1-getpwnam C)

include_directories(./include)
add_library(${PROJECT_NAME}_static STATIC src/getpwnam.c)
add_executable(${PROJECT_NAME} main.c)
target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}_static)