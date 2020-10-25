cmake_minimum_required(VERSION 3.17)

project(The-Linux-Programming-Interface C)

add_subdirectory(chapter3/3-1)
add_subdirectory(chapter4/4-1)
add_subdirectory(chapter4/4-2)
add_subdirectory(chapter4/seek-io)

add_subdirectory(chapter5/5-1)
add_subdirectory(chapter5/5-2)
add_subdirectory(chapter5/5-3)
add_subdirectory(chapter5/5-4)
add_subdirectory(chapter5/5-5)
add_subdirectory(chapter5/5-6)
add_subdirectory(chapter5/fcntl-get)
add_subdirectory(chapter5/readv)
add_subdirectory(chapter5/writev)

add_subdirectory(chapter6/6-1)
add_subdirectory(chapter6/6-3)
add_subdirectory(chapter6/display-env)

add_subdirectory(chapter7/7-1)
add_subdirectory(chapter7/free-and-sbrk)

add_subdirectory(chapter8/8-1)
add_subdirectory(chapter8/8-2)
add_subdirectory(chapter8/check-password)
add_subdirectory(chapter8/shadow-functions)
add_subdirectory(chapter8/ugid-functions)