project(PROCESS-TERMINATION C)

add_subdirectory(fork-stdio-buf)

if (UNIX AND NOT APPLE)
    add_subdirectory(exit-handlers)
endif ()