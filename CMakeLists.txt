cmake_minimum_required(VERSION 3.20)
project(MyPBR VERSION 0.1.0)

include_directories(ext)

add_executable(MyPBR main.cpp)
add_subdirectory(geometry)

