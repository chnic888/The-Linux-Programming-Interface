project(46-svmsg-file-client C)

include_directories(./include)
add_library(${PROJECT_NAME}_static STATIC src/svmsg_file_client.c)
add_executable(${PROJECT_NAME} main.c)
target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}_static)
