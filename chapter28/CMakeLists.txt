project(PROCESS-CREATION-AND-PROGRAM-EXECUTION-IN-MORE-DETAIL C)

add_subdirectory(acct-on)

if (UNIX AND NOT APPLE)
    add_subdirectory(t-clone)
endif ()
