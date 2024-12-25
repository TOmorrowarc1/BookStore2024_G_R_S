cmake_minimum_required(VERSION 3.16)
project(code)

set(CMAKE_CXX_STANDARD 17)

include_directories(
    "libs"
    "src"
)

add_executable(code
        main/ioput.cpp
        )