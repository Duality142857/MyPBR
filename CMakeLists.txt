cmake_minimum_required(VERSION 3.20)
project(MyPBR VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(ext)

find_package ( OpenMP )
if ( OpenMP_FOUND )
	message ( STATUS "OpenMP_VERSION = ${OpenMP_VERSION}" )
endif ()
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")


add_executable(MyPBR main.cpp)
add_subdirectory(geometry)
# target_compile_options(MyPBR PUBLIC -fopenmp)


