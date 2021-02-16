cmake_minimum_required(VERSION 3.17)
project(22-signalfd-sigval C)

include_directories(./include)
add_library(${PROJECT_NAME}_static STATIC src/signalfd_sigval.c)
add_executable(${PROJECT_NAME} main.c)
target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}_static)
