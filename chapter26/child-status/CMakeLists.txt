cmake_minimum_required(VERSION 3.17)
project(26-child-status C)

include_directories(./include)
add_library(${PROJECT_NAME}_static STATIC src/print_wait_status.c src/child_status.c)
add_executable(${PROJECT_NAME} main.c)
target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}_static)
