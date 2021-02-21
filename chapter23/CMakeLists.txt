project(TIMERS-AND-SLEEPING C)

add_subdirectory(real-time)
add_subdirectory(t-sleep)
add_subdirectory(t-nanosleep)
add_subdirectory(timed-read)

if (UNIX AND NOT APPLE)
    add_subdirectory(demo-timerfd)
endif ()