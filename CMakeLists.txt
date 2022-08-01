cmake_minimum_required(VERSION 3.21)
project(cpp_playground)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -O0")

add_executable(playGround main.cpp MyVector.cpp)
