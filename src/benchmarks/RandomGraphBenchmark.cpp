#include "../../incl/benchmarks/RandomGraphBenchmark.h"
#include "boost/graph/random.hpp"
#include "boost/random/mersenne_twister.hpp"


RandomGraphBenchmark::RandomGraphBenchmark(unsigned long vertices, unsigned long edges)
{
    boost::mt19937 gen;
    boost::generate_random_graph(G, vertices, edges, gen, false, false);
}

void RandomGraphBenchmark::run(unsigned long deletions, unsigned long queries)
{
    if (deletions > num_edges(G)) {
        throw std::invalid_argument("RandomGraphBenchmark: Too many deletions");
    }

    std::cout << "[i] random graph initialization time: " << std::fixed << this->getInitTime();
    std::cout << " seconds (" << num_vertices(G) << " vertices " << num_edges(G) << " edges)" << std::endl;
    std::cout << "[i] random graph deletion time: " << std::fixed << this->getDeletionsTime(deletions);
    std::cout << " seconds (" << deletions << " deletions)" << std::endl;
    std::cout << "[i] random graph query time: " << std::fixed << this->getQueryTime(queries);
    std::cout << " seconds (" << queries << " queries)" << std::endl;
}

