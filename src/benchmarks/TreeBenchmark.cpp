#include <iostream>
#include "../../incl/benchmarks/TreeBenchmark.h"
#include "../../incl/utilities/Utilities.h"


TreeBenchmark::TreeBenchmark(unsigned long depth)
{
    unsigned long edges=2;
    for (unsigned long i=3; i<depth; ++i) {
        edges = edges*2 + 2;
    }

    int offset = 1;
    for (Vertex i=0; i<=edges; ++i) {
        add_edge(i, i+offset, G);
        offset++;
        add_edge(i, i+offset, G);
    }
}

void TreeBenchmark::run(unsigned long deletions, unsigned long queries)
{
    if (deletions > num_edges(G)) {
        throw std::invalid_argument("TreeBenchmark: Too many deletions");
    }

    std::cout << "[i] tree initialization time: " << std::fixed << this->getInitTime();
    std::cout << " seconds (" << num_vertices(G) << " vertices)" << std::endl;
    std::cout << "[i] tree deletion time: " << std::fixed << this->getDeletionsTime(deletions);
    std::cout << " seconds (" << deletions << " deletions)" << std::endl;
    std::cout << "[i] tree query time: ";
    std::cout << std::fixed << this->getQueryTime(queries) << " seconds (" << queries << " queries)" << std::endl;
}
