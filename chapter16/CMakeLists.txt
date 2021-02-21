project(EXTENDED-ATTRIBUTES C)

if (UNIX AND NOT APPLE)
    add_subdirectory(xattr-view)
endif()