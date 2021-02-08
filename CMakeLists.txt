cmake_minimum_required(VERSION 3.16)
project(Bookstore_2021)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(src)

add_executable(Bookstore_2021
        src/main.cpp
        )
