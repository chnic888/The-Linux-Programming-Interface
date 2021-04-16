project(PROCESS-PRIORITIES-AND-SCHEDULING C)

add_subdirectory(t-setpriority)

if (UNIX AND NOT APPLE)
    add_subdirectory(sched-set)
    add_subdirectory(sched-view)
endif ()