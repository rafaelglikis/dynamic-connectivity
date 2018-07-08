#include <iostream>
#include "../../incl/benchmarks/TreeBenchmark.h"
#include "../../incl/utilities/Utilities.h"

/**
 * Creates a tree graph of @param(vertices) vertices
 * @param vertices (graph vertices)
 */
TreeBenchmark::TreeBenchmark(unsigned long vertices)
{
    int offset = 1;
    for (Vertex i=0; i<=vertices; ++i) {
        if (offset+i > vertices) {
            break;
        }
        add_edge(i, i+offset, G);
        offset++;
        if (offset+i > vertices) {
            break;
        }
        add_edge(i, i+offset, G);
    }
}

/**
 * Runs getInitTime(), getDeletionsTime(deletions), getQueryTime(queries) for the specified parameters
 * and prints the result.
 * throws invalid_argument exepion if the deletions are too many
 * @param deletions (number of deletions)
 * @param queries (number of queries)
 */
void TreeBenchmark::run(unsigned long deletions, unsigned long queries)
{
    if (deletions > num_edges(G)) {
        throw std::invalid_argument("TreeBenchmark: Too many deletions");
    }

    std::cout << "[i] tree initialization time: " << std::fixed << this->getInitTime();
    std::cout << " seconds (" << num_vertices(G) << " vertices " << num_edges(G) << " edges)" << std::endl;
    std::cout << "[i] tree deletion time: " << std::fixed << this->getDeletionsTime(deletions);
    std::cout << " seconds (" << deletions << " deletions)" << std::endl;
    std::cout << "[i] tree query time: ";
    std::cout << std::fixed << this->getQueryTime(queries) << " seconds (" << queries << " queries)" << std::endl;
}
