#include "../incl/graph_types/DynamicGraph.h"
#include "../incl/tests/DynamicGraphTest.h"
#include "../incl/examples/examples.h"
#include "../incl/tests/ActionTest.h"
#include "../incl/benchmarks/PathBenchmark.h"
#include "../incl/benchmarks/RandomGraphBenchmark.h"
#include "../incl/benchmarks/TreeBenchmark.h"

#include <iostream>

int main()
{
    // Tests
    ActionTest actionTest;
    DynamicGraphTest graphTest;
    actionTest.run();
    graphTest.run();

    // Benchmark
    PathBenchmark pathBenchmark(10000);
    pathBenchmark.run(5000, 5000);
    RandomGraphBenchmark randomGraphBenchmark(50, 50);
    randomGraphBenchmark.run(25, 25);
    TreeBenchmark treeBenchmark(10);
    treeBenchmark.run(500, 500);


    // Example
    //example();

    return 0;
}

