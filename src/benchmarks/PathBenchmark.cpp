#include <iostream>
#include "../../incl/benchmarks/PathBenchmark.h"

PathBenchmark::PathBenchmark(unsigned long vertices)
{
    for (Vertex i=0; i<vertices-1; ++i) {
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

void PathBenchmark::deletionsTime(unsigned long deletions)
{
    if (deletions > num_vertices(G)/2) {
        throw std::invalid_argument("PathBenchmark: Too many deletions");
    }
    Vertex offset = 2500;
    clock_t begin = clock();
    for (Vertex i=offset; i<deletions; ++i) {
        G.deleteEdge(i, i+1);
    }
    clock_t end = clock();
    double elapsed_secs = double(end-begin) / CLOCKS_PER_SEC;
    std::cout << "[i] path deletion time: ";
    std::cout << std::fixed << elapsed_secs << " seconds (" << deletions << " deletions)" << std::endl;
}

void PathBenchmark::queryTime(unsigned long queries)
{
    clock_t begin = clock();
    for (Vertex i=0; i<queries-1; ++i) {
        G.areConnected(i, i+1);
    }
    clock_t end = clock();
    double elapsed_secs = double(end-begin) / CLOCKS_PER_SEC;
    std::cout << "[i] path query time: ";
    std::cout << std::fixed << elapsed_secs << " seconds (" << queries << " queries)" << std::endl;
}
