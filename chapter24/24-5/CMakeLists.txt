project(24-5-fork-child-wait-parent C)

include_directories(./include)
add_library(${PROJECT_NAME}_static STATIC src/fork_child_wait_parent.c)
add_executable(${PROJECT_NAME} main.c)
target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}_static)
