cmake_minimum_required(VERSION 3.17)
project(30-mutex-prod-cons C)

add_executable(${PROJECT_NAME} main.c)
target_link_libraries(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})