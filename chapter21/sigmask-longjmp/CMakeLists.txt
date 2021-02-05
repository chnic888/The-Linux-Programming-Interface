cmake_minimum_required(VERSION 3.17)
project(21-sigmask-longjmp C)

add_compile_definitions(USE_SIGSETJMP)
include_directories(./include)
add_library(${PROJECT_NAME}_static STATIC src/sigmask_longjmp.c src/signal_functions.c)
add_executable(${PROJECT_NAME} main.c)
target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}_static)
