cmake_minimum_required(VERSION 3.17)

project(The-Linux-Programming-Interface C)

foreach (NUM RANGE ${START} ${END})
    set(PREFIX)

    if (NUM LESS 10)
        set(PREFIX 0)
    endif ()

    if (NOT EXISTS "${PROJECT_SOURCE_DIR}/chapter${PREFIX}${NUM}/CMakeLists.txt")
        continue()
    endif ()

    add_subdirectory(chapter${PREFIX}${NUM})
endforeach ()
