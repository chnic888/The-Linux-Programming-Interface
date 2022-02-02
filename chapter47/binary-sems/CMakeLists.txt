project(47-binary-sems C)

include_directories(./include)
add_library(${PROJECT_NAME}_static STATIC src/binary_sems.c)
add_executable(${PROJECT_NAME} main.c)
target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}_static)
