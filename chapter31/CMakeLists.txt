project(THREADS-THREAD-SAFETY-AND-PER-THREAD-STORAGE C)

find_package(Threads)

if (UNIX AND NOT APPLE)
    add_subdirectory(strerror-test)
    add_subdirectory(strerror-tsd)
    add_subdirectory(strerror-tls)
endif ()
