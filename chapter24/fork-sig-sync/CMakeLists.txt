project(24-fork-sig-sync C)

include_directories(./include)
add_library(${PROJECT_NAME}_static STATIC src/fork_sig_sync.c src/curr_time.c)
add_executable(${PROJECT_NAME} main.c)
target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}_static)
