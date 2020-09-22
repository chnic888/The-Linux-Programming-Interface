project(3-1-reboot C)

include_directories(./include)

add_library(${PROJECT_NAME}_static STATIC src/reboot.c)

add_executable(${PROJECT_NAME} main.c)

target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}_static)