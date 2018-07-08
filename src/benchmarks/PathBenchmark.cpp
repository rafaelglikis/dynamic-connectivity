#include <iostream>
#include "../../incl/benchmarks/PathBenchmark.h"

PathBenchmark::PathBenchmark(unsigned long vertices)
{
    for (Vertex i=0; i<vertices-1; ++i) {
        add_edge(i, i+1, G);
    }
}

void PathBenchmark::run(unsigned long deletions, unsigned long queries)
{
    if (deletions > num_edges(G)) {
        throw std::invalid_argument("PathBenchmark: Too many deletions");
    }

    std::cout << "[i] path initialization time: " << std::fixed << this->getInitTime();
    std::cout << " seconds (" << num_vertices(G) << " vertices)" << std::endl;
    std::cout << "[i] path deletion time: " << std::fixed << this->getDeletionsTime(deletions);
    std::cout << " seconds (" << deletions << " deletions)" << std::endl;
    std::cout << "[i] path query time: ";
    std::cout << std::fixed << this->getQueryTime(queries) << " seconds (" << queries << " queries)" << std::endl;
}