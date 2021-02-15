cmake_minimum_required(VERSION 3.17)
project(22-t-sigsuspend C)

include_directories(./include)
add_library(${PROJECT_NAME}_static STATIC src/t_sigsuspend.c)
add_executable(${PROJECT_NAME} main.c)
target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}_static)
