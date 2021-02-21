project(PROCESS-CREDENTIALS C)

if (UNIX AND NOT APPLE)
    add_subdirectory(9-1)
    add_subdirectory(9-2)
    add_subdirectory(idshow)
endif ()