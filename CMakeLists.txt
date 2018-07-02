cmake_minimum_required(VERSION 3.10)
project(dynamic_connectivity)

include_directories("/usr/include/LEDA")

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "-lleda -lX11")

find_package(Boost)
IF (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIR})
endif()


add_executable(dynamic_connectivity main.cpp src/DynamicGraph.cpp incl/DynamicGraph.h)