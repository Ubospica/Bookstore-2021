cmake_minimum_required(VERSION 3.16)
project(Bookstore_2021)

set(CMAKE_CXX_STANDARD 14)

set (Bookstore_2021_VERSION_MAJOR 0)
set (Bookstore_2021_VERSION_MINOR 1)

include_directories(src)

add_executable(code
        src/main.cpp src/MainClass.cpp src/User.cpp src/UserStatus.cpp
        src/Data.hpp src/Data.cpp src/Book.cpp src/Bookstore.cpp)

add_subdirectory(src/BPlusTree)
target_include_directories(code PUBLIC src/BPlusTree)

# enable_testing()
# add_test(NAME BPTTest COMMAND BPlusTreeTest)