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

add_executable(dynamic_connectivity src/main.cpp src/graph_types/DynamicGraph.cpp incl/graph_types/DynamicGraph.h src/graph_types/DynamicGraph.cpp incl/graph_types/Relatives.h incl/graph_types/UndirectedGraph.h src/actions/Action.cpp incl/actions/Action.h src/actions/Delete.cpp incl/actions/Delete.h src/actions/Insert.cpp incl/actions/Insert.h src/tests/DynamicGraphTest.cpp incl/tests/DynamicGraphTest.h incl/tests/Test.h src/examples/example.cpp incl/examples/examples.h src/tests/ActionTest.cpp incl/tests/ActionTest.h src/benchmarks/Benchmark.cpp incl/benchmarks/Benchmark.h src/benchmarks/PathBenchmark.cpp incl/benchmarks/PathBenchmark.h src/benchmarks/RandomGraphBenchmark.cpp incl/benchmarks/RandomGraphBenchmark.h src/utilities/Utilities.cpp incl/utilities/Utilities.h src/benchmarks/TreeBenchmark.cpp incl/benchmarks/TreeBenchmark.h src/benchmarks/PathCompleteGraphsBenchmark.cpp incl/benchmarks/PathCompleteGraphsBenchmark.h)
