project(5-3-atomic-append C)

include_directories(./include)
add_library(${PROJECT_NAME}_static STATIC src/atomic_append.c)
add_executable(${PROJECT_NAME} main.c)
target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}_static)