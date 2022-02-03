project(48-svshm-xfr-writer C)

include_directories(./include)
add_library(${PROJECT_NAME}_static STATIC src/binary_sems.c src/svshm_xfr_writer.c)
add_executable(${PROJECT_NAME} main.c)
target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}_static)
