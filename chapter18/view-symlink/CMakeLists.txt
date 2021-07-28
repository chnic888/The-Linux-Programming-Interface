project(18-view-symlink C)

include_directories(./include)
add_library(${PROJECT_NAME}_static STATIC src/view_symlink.c)
add_executable(${PROJECT_NAME} main.c)
target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}_static)
