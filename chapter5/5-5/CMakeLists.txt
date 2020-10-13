cmake_minimum_required(VERSION 3.17)
project(5-5-dup-verify C)

include_directories(./include)
add_library(${PROJECT_NAME}_static STATIC src/dup_verify.c)
add_executable(${PROJECT_NAME} main.c)
target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}_static)