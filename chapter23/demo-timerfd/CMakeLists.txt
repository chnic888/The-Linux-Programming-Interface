project(23-demo-timerfd C)

include_directories(./include)
add_library(${PROJECT_NAME}_static STATIC src/demo_timerfd.c src/itimerspec_from_str.c)
add_executable(${PROJECT_NAME} main.c)
target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}_static)
