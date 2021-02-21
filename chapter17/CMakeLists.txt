project(ACCESS-CONTROL-LISTS C)

if (UNIX AND NOT APPLE)
    add_subdirectory(acl-view)
endif ()