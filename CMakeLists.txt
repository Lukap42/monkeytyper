set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")
cmake_minimum_required(VERSION 3.27)
project(MonkeyTyper)

set(CMAKE_CXX_STANDARD 20)

set(BUILD_SHARED_LIBS FALSE)

include(FetchContent)

FetchContent_Declare(
        fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt
        GIT_TAG 10.2.1
)

FetchContent_Declare(
        sfml
        GIT_REPOSITORY https://github.com/SFML/SFML
        GIT_TAG 2.6.1
)

FetchContent_MakeAvailable(fmt)
FetchContent_MakeAvailable(sfml)

add_executable(MonkeyTyper main.cpp
        Word.h
        Word.cpp
        UserText.h
        UserText.cpp
        GameState.h
        DisplayObject.h
        DisplayObject.cpp
)
target_link_libraries(MonkeyTyper fmt)
target_link_libraries(MonkeyTyper sfml-graphics)
