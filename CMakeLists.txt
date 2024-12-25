cmake_minimum_required(VERSION 3.16)
project(code)

set(CMAKE_CXX_STANDARD 17)

include_directories(
    "libs"
    "src"
)

add_executable(code
        libs/storage.hpp
        libs/String.cpp
        libs/String.cpp
        libs/tokenscanner.cpp
        libs/tokenscanner.hpp
        src/account.cpp
        src/account.hpp
        src/diary.cpp
        src/diary.hpp
        src/book.cpp
        src/book.hpp
        main/ioput.cpp
        )