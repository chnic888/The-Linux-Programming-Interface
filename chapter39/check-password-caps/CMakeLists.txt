cmake_minimum_required(VERSION 3.17)
project(39-check-password-caps C)

include_directories(./include)
add_library(${PROJECT_NAME}_static STATIC src/check_password_caps.c)
add_executable(${PROJECT_NAME} main.c)
target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}_static)
