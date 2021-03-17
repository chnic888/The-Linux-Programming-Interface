cmake_minimum_required(VERSION 3.17)
project(31-thread-cleanup C)

include_directories(./include)
add_library(${PROJECT_NAME}_static STATIC src/thread_cleanup.c)
add_executable(${PROJECT_NAME} main.c)
target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}_static ${CMAKE_THREAD_LIBS_INIT})