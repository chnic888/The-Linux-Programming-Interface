project(18-nftw-dir-tree C)

include_directories(./include)
add_library(${PROJECT_NAME}_static STATIC src/nftw_dir_tree.c)
add_executable(${PROJECT_NAME} main.c)
target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}_static)
