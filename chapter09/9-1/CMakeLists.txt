cmake_minimum_required(VERSION 3.17)
project(9-1-setuids C)

include_directories(./include)
add_library(${PROJECT_NAME}_static STATIC src/setuids.c)
add_executable(${PROJECT_NAME} main.c)
target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}_static)