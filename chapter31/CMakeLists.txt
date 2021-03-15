project(THREADS-THREAD-SAFETY-AND-PER-THREAD-STORAGE C)

find_package(Threads)

add_subdirectory(strerror-test)
add_subdirectory(strerror-tsd)