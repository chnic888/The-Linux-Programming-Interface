project(36-rlimit-nproc C)

include_directories(./include)
add_library(${PROJECT_NAME}_static STATIC src/print_rlimit.c src/rlimit_nproc.c)
add_executable(${PROJECT_NAME} main.c)
target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}_static)
