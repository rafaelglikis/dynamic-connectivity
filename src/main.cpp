#include "../incl/graph_types/DynamicGraph.h"
#include "../incl/tests/DynamicGraphTest.h"
#include "../incl/examples/examples.h"
#include "../incl/tests/ActionTest.h"
#include "../incl/benchmarks/PathBenchmark.h"
#include "../incl/benchmarks/RandomGraphBenchmark.h"

#include <iostream>

/**
 * Returns a random number in between min-max.
*/
int randomRange(int min, int max)
{
    static bool first = true;
    if (first)
    {
        srand( time(NULL) ); //seeding for the first time only!
        first = false;
    }
    return min + rand() % (( max + 1 ) - min);
}

int main()
{
    // Tests
    ActionTest actionTest;
    DynamicGraphTest graphTest;
    actionTest.run();
    graphTest.run();

    // Benchmark
//    PathBenchmark pathBenchmark(10000);
//    pathBenchmark.initTime();
//    pathBenchmark.deletionsTime(5000);
//    pathBenchmark.queryTime(5000);
    RandomGraphBenchmark randomGraphBenchmark(10000, 10000);
    randomGraphBenchmark.initTime();
    randomGraphBenchmark.deletionsTime(5000);
    randomGraphBenchmark.queryTime(5000);

    // Example
    //example();

    return 0;
}

