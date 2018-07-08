#include "../incl/graph_types/DynamicGraph.h"
#include "../incl/tests/DynamicGraphTest.h"
#include "../incl/examples/examples.h"
#include "../incl/tests/ActionTest.h"
#include "../incl/benchmarks/PathBenchmark.h"
#include "../incl/benchmarks/RandomGraphBenchmark.h"
#include "../incl/benchmarks/TreeBenchmark.h"
#include "../incl/benchmarks/CompletePartsGraphBenchmark.h"

#include <iostream>

int main()
{
    // Tests
    ActionTest actionTest;
    DynamicGraphTest graphTest;
    actionTest.run();
    graphTest.run();

    // Benchmarks
    PathBenchmark pathBenchmark(10000);
    pathBenchmark.run(5000, 5000);
    RandomGraphBenchmark randomGraphBenchmark(10000, 300000);
    randomGraphBenchmark.run(5000, 5000);
    TreeBenchmark treeBenchmark(10000);
    treeBenchmark.run(5000, 5000);
    CompletePartsGraphBenchmark completePartsGraphBenchmark(100, 100);
    completePartsGraphBenchmark.run(5000, 5000);

    // Example
    //example();

    return 0;
}

