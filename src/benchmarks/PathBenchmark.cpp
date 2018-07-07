#include <iostream>
#include "../../incl/benchmarks/PathBenchmark.h"

PathBenchmark::PathBenchmark(unsigned long numberOfVertices)
{
    for (Vertex i=0; i<numberOfVertices-1; ++i) {
        add_edge(i, i+1, G);
    }
}

void PathBenchmark::initTime()
{
    clock_t begin = clock();
    G.init();
    clock_t end = clock();
    double elapsed_secs = double(end-begin) / CLOCKS_PER_SEC;
    std::cout << "[i] path initialization time: ";
    std::cout << std::fixed << elapsed_secs << " seconds (" << num_vertices(G) << " vertices)" << std::endl;
}

void PathBenchmark::deletionsTime(unsigned long numberOfDeletions)
{
    if (numberOfDeletions > num_vertices(G)/2) {
        throw std::invalid_argument("Too many deletions");
    }
    Vertex offset = 2500;
    clock_t begin = clock();
    for (Vertex i=offset; i<numberOfDeletions; ++i) {
        G.deleteEdge(i, i+1);
    }
    clock_t end = clock();
    double elapsed_secs = double(end-begin) / CLOCKS_PER_SEC;
    std::cout << "[i] path deletion time: ";
    std::cout << std::fixed << elapsed_secs << " seconds (" << numberOfDeletions << " queries)" << std::endl;
}

void PathBenchmark::queryTime(unsigned long numberOfQueries)
{
    clock_t begin = clock();
    for (Vertex i=0; i<numberOfQueries-1; ++i) {
        G.areConnected(i, i+1);
    }
    clock_t end = clock();
    double elapsed_secs = double(end-begin) / CLOCKS_PER_SEC;
    std::cout << "[i] path query time: ";
    std::cout << std::fixed << elapsed_secs << " seconds (" << numberOfQueries << " numberOfQueries)" << std::endl;
}
