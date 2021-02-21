cmake_minimum_required(VERSION 3.17)

project(The-Linux-Programming-Interface C)

foreach (NUM RANGE ${START} ${END})
    if(NUM LESS 3)
        continue()
    endif()

    set(PREFIX )

    if(NUM LESS 10)
        set(PREFIX 0)
    endif()

    add_subdirectory(chapter${PREFIX}${NUM})
endforeach ()
