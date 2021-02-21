project(SYSTEM-AND-PROCESS-INFORMATION C)

add_subdirectory(t-uname)

if (UNIX AND NOT APPLE)
    add_subdirectory(procfs-pidmax)
endif ()