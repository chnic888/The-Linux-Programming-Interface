project(3-1-reboot C)

include_directories(./include)

add_library(3_1_static STATIC src/reboot.c)

add_executable(${PROJECT_NAME} main.c)

target_link_libraries(${PROJECT_NAME} 3_1_static)