project(6-3-set-unset-env C)

include_directories(./include)
add_library(${PROJECT_NAME}_static STATIC src/set_unset_env.c)
add_executable(${PROJECT_NAME} main.c)
target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}_static)