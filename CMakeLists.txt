cmake_minimum_required(VERSION 3.17)

project(The-Linux-Programming-Interface C)

set(START 3)
set(END 24)

foreach (NUM RANGE ${START} ${END})
    set(PREFIX )

    IF(NUM LESS 10)
        set(PREFIX 0)
    ENDIF()

    add_subdirectory(chapter${PREFIX}${NUM})
endforeach ()
