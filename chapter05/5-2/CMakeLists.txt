project(5-2-append-seek-set C)

include_directories(./include)
add_library(${PROJECT_NAME}_static STATIC src/append_seek_set.c)
add_executable(${PROJECT_NAME} main.c)
target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}_static)