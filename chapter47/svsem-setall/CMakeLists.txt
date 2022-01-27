project(47-svsem-setall C)

include_directories(./include)
add_library(${PROJECT_NAME}_static STATIC src/svsem_setall.c)
add_executable(${PROJECT_NAME} main.c)
target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}_static)
