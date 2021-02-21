project(USERS-AND-GROUPS C)

add_subdirectory(8-1)
add_subdirectory(8-2)
add_subdirectory(ugid-functions)

if (UNIX AND NOT APPLE)
    add_subdirectory(check-password)
    add_subdirectory(shadow-functions)
endif ()