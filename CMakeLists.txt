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

if (UNIX AND NOT APPLE)
    add_subdirectory(chapter13/direct-read)
endif ()

add_subdirectory(chapter14/t-statvfs)
if (UNIX AND NOT APPLE)
    add_subdirectory(chapter14/t-mount)
    add_subdirectory(chapter14/t-umount)
endif ()

add_subdirectory(chapter15/t-chown)
add_subdirectory(chapter15/t-stat)
add_subdirectory(chapter15/t-umask)
add_subdirectory(chapter15/t-utime)
add_subdirectory(chapter15/t-utimensat)

if (UNIX AND NOT APPLE)
    add_subdirectory(chapter16/xattr-view)
endif()

if (UNIX AND NOT APPLE)
    add_subdirectory(chapter17/acl-view)
endif ()

add_subdirectory(chapter18/list-files)
add_subdirectory(chapter18/nftw-dir-tree)
add_subdirectory(chapter18/t-dirbasename)
add_subdirectory(chapter18/t-mkdir)
add_subdirectory(chapter18/t-unlink)
add_subdirectory(chapter18/view-symlink)

if (UNIX AND NOT APPLE)
    add_subdirectory(chapter19/demo-inotify)
endif ()

add_subdirectory(chapter20/intquit)
add_subdirectory(chapter20/ouch)
add_subdirectory(chapter20/t-kill)

add_subdirectory(chapter21/nonreentrant)
add_subdirectory(chapter21/sigmask-longjmp)
add_subdirectory(chapter21/t-sigaltstack)

add_subdirectory(chapter22/t-sigqueue)