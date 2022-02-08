project(49-anon-mmap C)

include_directories(./include)
add_library(${PROJECT_NAME}_static STATIC src/anon_mmap.c)
add_executable(${PROJECT_NAME} main.c)
target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}_static)
