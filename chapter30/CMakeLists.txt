project(THREADS-THREAD-SYNCHRONIZATION C)

find_package(Threads)

add_subdirectory(thread-incr)
add_subdirectory(mutex-types)
add_subdirectory(mutex-prod-cons)
add_subdirectory(cond-prod-cons)
add_subdirectory(thread-multijoin)