project(LOGIN-ACCOUNTING C)

add_subdirectory(dump-utmpx)
add_subdirectory(utmpx-login)

if (UNIX AND NOT APPLE)
    add_subdirectory(view-lastlog)
endif()
