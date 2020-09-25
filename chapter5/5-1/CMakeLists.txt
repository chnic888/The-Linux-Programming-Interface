cmake_minimum_required(VERSION 3.17)
project(5-1-large-file C)

include_directories(./include)
add_library(${PROJECT_NAME}_static STATIC src/large_file.c)
add_executable(${PROJECT_NAME} main.c)
target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}_static)