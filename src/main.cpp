#include <iostream>
#include <iomanip>
#include "../incl/graph_types/DynamicGraph.h"
#include "../incl/tests/DynamicGraphTest.h"
#include "../incl/examples/example.h"
#include "../incl/tests/ActionTest.h"
#include "../incl/benchmarks/PathBenchmark.h"
#include "../incl/benchmarks/RandomGraphBenchmark.h"
#include "../incl/benchmarks/TreeBenchmark.h"
#include "../incl/benchmarks/PathCompleteGraphsBenchmark.h"
#include <cmath>
#include <boost/program_options.hpp>

unsigned long vertices;
unsigned long edges;
unsigned long deletions;
unsigned long queries;

boost::program_options::variables_map parseArguments(int argc, char *const *argv)
{
    boost::program_options::options_description options("Usage");
    options.add_options()
            ("test", "run all tests")
            ("benchmark", "run benchmark specified benchmarks")
            ("path", "run benchmark with path graph")
            ("tree", "run benchmark with tree graph")
            ("random", "run benchmark with random graph")
            ("path-complete", "run benchmark with path of complete subgraphs")
            ("vertices,v", boost::program_options::value<unsigned long>(&vertices)
                    ->default_value(100), "specify number of vertices (default 100)")
            ("edges,e", boost::program_options::value<unsigned long>(&edges)
                     ->default_value(static_cast<const unsigned long &>(100*log(100))),
             "specify number of edges (default vertices*log(vertices)) (only for random graphs)")
            ("deletions,d", boost::program_options::value<unsigned long>(&deletions)
                    ->default_value(50), "specify number of vertices (default 50)")
            ("queries,q", boost::program_options::value<unsigned long>(&queries)
                    ->default_value(50), "specify number of vertices (default 50)")
            ("example", "run an example")
            ("help", "produce help message")
            ;

    boost::program_options::variables_map vm;
    store(parse_command_line(argc, argv, options), vm);
    notify(vm);

    if (vm.count("help")) {
        std::cout << options << "\n";
        exit(1);
    }

    return vm;
}

int main(int argc, char* argv[])
{
    boost::program_options::variables_map args = parseArguments(argc, argv);

    if (args.count("test")) {
        ActionTest actionTest;
        DynamicGraphTest graphTest;
        actionTest.run();
        graphTest.run();
    }
    if (args.count("benchmark")) {
        if (args.count("path")) {
            PathBenchmark pathBenchmark(vertices);
            pathBenchmark.run(deletions, queries);
        }
        if (args.count("tree")) {
            TreeBenchmark treeBenchmark(vertices);
            treeBenchmark.run(deletions, queries);
        }
        if (args.count("random")) {
            RandomGraphBenchmark randomGraphBenchmark(vertices, edges);
            randomGraphBenchmark.run(deletions, queries);
        }
        if (args.count("path-complete")) {
            PathCompleteGraphsBenchmark pathCompleteGraphsBenchmark(
                    static_cast<int>(sqrt(vertices)),
                    static_cast<int>(sqrt(vertices)));
            pathCompleteGraphsBenchmark.run(deletions, queries);
        }
    }
    if (args.count("example")) {
        runExample();
    }

    return 0;
}

