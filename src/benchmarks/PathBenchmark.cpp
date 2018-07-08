#include <iostream>
#include "../../incl/benchmarks/PathBenchmark.h"

/**
 * Creates a path graph of @param(vertices) vertices
 * @param vertices (graph vertices)
 */
PathBenchmark::PathBenchmark(unsigned long vertices)
{
    for (Vertex i=0; i<vertices-1; ++i) {
        add_edge(i, i+1, G);
    }
}

/**
 * Runs getInitTime(), getDeletionsTime(deletions), getQueryTime(queries) for the specified parameters
 * and prints the result.
 * throws invalid_argument exepion if the deletions are too many
 * @param deletions (number of deletions)
 * @param queries (number of queries)
 */
void PathBenchmark::run(unsigned long deletions, unsigned long queries)
{
    if (deletions > num_edges(G)) {
        throw std::invalid_argument("PathBenchmark: Too many deletions");
    }

    std::cout << "[i] path initialization time: " << std::fixed << this->getInitTime();
    std::cout << " seconds (" << num_vertices(G) << " vertices " << num_edges(G) << " edges)" << std::endl;
    std::cout << "[i] path deletion time: " << std::fixed << this->getDeletionsTime(deletions);
    std::cout << " seconds (" << deletions << " deletions)" << std::endl;
    std::cout << "[i] path query time: ";
    std::cout << std::fixed << this->getQueryTime(queries) << " seconds (" << queries << " queries)" << std::endl;
}