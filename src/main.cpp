#include "../incl/graph_types/DynamicGraph.h"
#include "../incl/tests/DynamicGraphTest.h"
#include "../incl/examples/examples.h"
#include "../incl/tests/ActionTest.h"
#include "../incl/benchmarks/PathBenchmark.h"

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
    pathBenchmark.initTime();
    pathBenchmark.deletionsTime(5000);
    pathBenchmark.queryTime(5000);

    // Example
    //example();

    return 0;
}

