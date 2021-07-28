project(7-1-free-and-sbrk-verify C)

include_directories(./include)
add_library(${PROJECT_NAME}_static STATIC src/free_and_sbrk_verify.c)
add_executable(${PROJECT_NAME} main.c)
target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}_static)