project(45-svmsg-demo-server C)

include_directories(./include)
add_library(${PROJECT_NAME}_static STATIC src/svmsg_demo_server.c)
add_executable(${PROJECT_NAME} main.c)
target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}_static)
