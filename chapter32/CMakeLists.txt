project(THREADS-THREAD-CANCELLATION C)

find_package(Threads)

add_subdirectory(thread-cancel)
add_subdirectory(thread-cleanup)