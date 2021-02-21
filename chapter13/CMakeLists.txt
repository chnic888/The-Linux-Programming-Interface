project(FILE-IO-BUFFERING C)

if (UNIX AND NOT APPLE)
    add_subdirectory(direct-read)
endif ()