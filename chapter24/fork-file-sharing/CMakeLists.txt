project(24-fork-file-sharing C)

include_directories(./include)
add_library(${PROJECT_NAME}_static STATIC src/fork_file_sharing.c)
add_executable(${PROJECT_NAME} main.c)
target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}_static)
