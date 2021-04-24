project(CAPABILITIES C)

if (UNIX AND NOT APPLE)
    add_subdirectory(check-password-caps)
endif()
