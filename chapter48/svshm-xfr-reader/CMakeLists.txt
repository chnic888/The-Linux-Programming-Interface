project(48-svshm-xfr-reader C)

include_directories(./include)
add_library(${PROJECT_NAME}_static STATIC src/svshm_xfr_reader.c src/binary_sems.c)
add_executable(${PROJECT_NAME} main.c)
target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}_static)
