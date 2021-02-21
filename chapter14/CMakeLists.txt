project(FILE-SYSTEMS C)

add_subdirectory(t-statvfs)

if (UNIX AND NOT APPLE)
    add_subdirectory(t-mount)
    add_subdirectory(t-umount)
endif ()