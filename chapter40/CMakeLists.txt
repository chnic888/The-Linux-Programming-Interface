project(LOGIN-ACCOUNTING C)

add_subdirectory(dump-utmpx)

if (UNIX AND NOT APPLE)
    add_subdirectory(utmpx-login)
    add_subdirectory(view-lastlog)
endif()
