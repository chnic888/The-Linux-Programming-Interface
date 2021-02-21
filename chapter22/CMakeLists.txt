project(SIGNALS-ADVANCED-FEATURES C)

add_subdirectory(catch-rtsigs)
add_subdirectory(t-sigsuspend)

if (UNIX AND NOT APPLE)
    add_subdirectory(signalfd-sigval)
    add_subdirectory(t-sigqueue)
    add_subdirectory(t-sigwaitinfo)
endif ()