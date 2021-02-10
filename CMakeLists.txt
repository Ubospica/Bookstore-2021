cmake_minimum_required(VERSION 3.16)
project(Bookstore_2021)

set(CMAKE_CXX_STANDARD 14)

include_directories(src)

aux_source_directory(src DIR_SRCS)
add_executable(Bookstore_2021
        src/main.cpp src/MainClass.cpp src/User.cpp src/UserStatus.cpp
        src/Exception.hpp)

