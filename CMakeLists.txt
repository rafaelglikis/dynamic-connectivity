cmake_minimum_required(VERSION 3.10)
project(dynamic_connectivity)

include_directories("/usr/include/LEDA")

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "-lleda -lX11")

find_package(Boost)
if (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIR})
endif()

find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

add_executable(dynamic_connectivity main.cpp src/DynamicGraph.cpp incl/DynamicGraph.h src/DynamicGraph.cpp incl/Relatives.h incl/UndirectedGraph.h src/Action.cpp incl/Action.h src/Delete.cpp incl/Delete.h src/Insert.cpp incl/Insert.h)
