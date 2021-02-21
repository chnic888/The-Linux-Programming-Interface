project(MONITORING-FILE-EVENTS C)

if (UNIX AND NOT APPLE)
    add_subdirectory(demo-inotify)
endif ()
