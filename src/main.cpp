#include <iostream>
#include <iomanip>
#include "../incl/graph_types/DynamicGraph.h"
#include "../incl/tests/DynamicGraphTest.h"
#include "../incl/examples/examples.h"
#include "../incl/tests/ActionTest.h"
#include "../incl/benchmarks/PathBenchmark.h"
#include "../incl/benchmarks/RandomGraphBenchmark.h"
#include "../incl/benchmarks/TreeBenchmark.h"
#include "../incl/benchmarks/PathCompleteGraphsBenchmark.h"
#include <cmath>

int main(int argc, char* argv[])
{
    // Parse arguments
    bool test = false;
    bool example = false;
    bool benchmark = false;
    bool path = false;
    bool tree = false;
    bool random = false;
    bool partComplete = false;
    bool help = false;

    unsigned long vertices = 10000;
    unsigned long edges = static_cast<unsigned long>(vertices*log(vertices));
    unsigned long deletes = 5000;
    unsigned long queries = 5000;

    for(int i=0; i<argc; ++i) {
        if(std::string(argv[i])=="-v" || std::string(argv[i])=="--vertices")
            vertices= static_cast<unsigned long>(atol(argv[i+1]));
        else if(std::string(argv[i])=="-e" || std::string(argv[i])=="--edges")
            edges= static_cast<unsigned long>(atol(argv[i+1]));
        else if(std::string(argv[i])=="-d" || std::string(argv[i])=="--deletions")
            deletes= static_cast<unsigned long>(atol(argv[i+1]));
        else if(std::string(argv[i])=="-q" || std::string(argv[i])=="--queries")
            queries= static_cast<unsigned long>(atol(argv[i+1]));
        else if(std::string(argv[i])=="benchmark")
            benchmark=true;
        else if(std::string(argv[i])=="test")
            test=true;
        else if(std::string(argv[i])=="example")
            example=true;
        else if(std::string(argv[i])=="--path")
            path=true;
        else if(std::string(argv[i])=="--tree")
            tree=true;
        else if(std::string(argv[i])=="--random")
            random=true;
        else if(std::string(argv[i])=="--path-complete")
            partComplete=true;
        else if(std::string(argv[i])=="--help" || std::string(argv[i])=="-h")
            help=true;
    }

    if (test) {
        ActionTest actionTest;
        DynamicGraphTest graphTest;
        actionTest.run();
        graphTest.run();
    }
    if (benchmark) {
        if (path) {
            PathBenchmark pathBenchmark(vertices);
            pathBenchmark.run(deletes, queries);
        }
        if (tree) {
            TreeBenchmark treeBenchmark(vertices);
            treeBenchmark.run(deletes, queries);
        }
        if (random) {
            RandomGraphBenchmark randomGraphBenchmark(vertices, edges);
            randomGraphBenchmark.run(deletes, queries);
        }
        if (partComplete) {
            PathCompleteGraphsBenchmark pathCompleteGraphsBenchmark(
                    static_cast<int>(sqrt(vertices)),
                    static_cast<int>(sqrt(vertices)));
            pathCompleteGraphsBenchmark.run(deletes, queries);
        }
    }
    if (example) {
        runExample();
    }

    if (help || argc < 2) {
        std::cout << "Usage: " << std::endl;
        std::cout << "  " << argv[0] << " ACTION* [OPTION]*" << std::endl;
        std::cout << std::endl;
        std::cout << "Example: " << std::endl;
        std::cout << "  " << argv[0] << " benchmark --random -v 500 -e 1000 -d 50 -q 50" << std::endl;
        std::cout << std::endl;
        std::cout << "Test: " << std::endl;
        std::cout << std::left << std::setw(30) <<"  test " << "run all tests" << std::endl;
        std::cout << std::endl;
        std::cout << "Benchmark: " << std::endl;
        std::cout << std::left << std::setw(30) <<"  benchmark" << "run benchmark specified benchmarks" << std::endl;
        std::cout << std::left << std::setw(30) <<"  --path" << "run benchmark with path graph" << std::endl;
        std::cout << std::left << std::setw(30) <<"  --tree" << "run benchmark with tree graph" << std::endl;
        std::cout << std::left << std::setw(30) <<"  --random" << "run benchmark with tree graph" << std::endl;
        std::cout << std::left << std::setw(30) <<"  --path-complete" << "run benchmark with path of partly complete graphs" << std::endl;
        std::cout << std::endl;
        std::cout << "Benchmark options: " << std::endl;
        std::cout << std::left << std::setw(30) <<"  -v, --vertices <number>" << "specify number of vertices (default 10000)" << std::endl;
        std::cout << std::left << std::setw(30) <<"  -e, --edges <number>" << "specify number of edges (default 10000*log(10000))" << std::endl;
        std::cout << std::left << std::setw(30) <<"  -d, --deletions <number>" << "specify number of deletions (default 5000)" << std::endl;
        std::cout << std::left << std::setw(30) <<"  -q, --queries <number>" << "specify number of queries (default 5000)" << std::endl;
        std::cout << std::endl;
        std::cout << "Examples: " << std::endl;
        std::cout << std::left << std::setw(30) <<"  example" << "run an example" << std::endl;
        std::cout << std::endl;
    }

    return 0;
}

