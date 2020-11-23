cmake_minimum_required(VERSION 3.17)

project(The-Linux-Programming-Interface C)

add_subdirectory(chapter03/3-1)
add_subdirectory(chapter04/4-1)
add_subdirectory(chapter04/4-2)
add_subdirectory(chapter04/seek-io)

add_subdirectory(chapter05/5-1)
add_subdirectory(chapter05/5-2)
add_subdirectory(chapter05/5-3)
add_subdirectory(chapter05/5-4)
add_subdirectory(chapter05/5-5)
add_subdirectory(chapter05/5-6)
add_subdirectory(chapter05/fcntl-get)
add_subdirectory(chapter05/readv)
add_subdirectory(chapter05/writev)

add_subdirectory(chapter06/6-1)
add_subdirectory(chapter06/6-3)
add_subdirectory(chapter06/display-env)

add_subdirectory(chapter07/7-1)
add_subdirectory(chapter07/free-and-sbrk)

add_subdirectory(chapter08/8-1)
add_subdirectory(chapter08/8-2)
if (UNIX AND NOT APPLE)
    add_subdirectory(chapter08/check-password)
    add_subdirectory(chapter08/shadow-functions)
endif ()
add_subdirectory(chapter08/ugid-functions)

if (UNIX AND NOT APPLE)
    add_subdirectory(chapter09/9-1)
    add_subdirectory(chapter09/9-2)
    add_subdirectory(chapter09/idshow)
endif ()

add_subdirectory(chapter10/calendar-time)
add_subdirectory(chapter10/curr-time)
add_subdirectory(chapter10/process-time)
add_subdirectory(chapter10/show-time)
add_subdirectory(chapter10/strtime)

add_subdirectory(chapter11/t-fpathconf)
add_subdirectory(chapter11/t-sysconf)

if (UNIX AND NOT APPLE)
    add_subdirectory(chapter12/procfs-pidmax)
endif ()
add_subdirectory(chapter12/t-uname)

add_subdirectory(chapter13/direct-read)