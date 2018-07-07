#include "../../incl/benchmarks/RandomGraphBenchmark.h"
#include "boost/graph/random.hpp"
#include "boost/random/mersenne_twister.hpp"
#include "../../incl/utilities/Utilities.h"


RandomGraphBenchmark::RandomGraphBenchmark(unsigned long vertices, unsigned int edges)
{
    boost::mt19937 gen;
    boost::generate_random_graph(G, vertices, edges, gen, false, false);
}

void RandomGraphBenchmark::initTime()
{
    clock_t begin = clock();
    G.init();
    clock_t end = clock();
    double elapsed_secs = double(end-begin) / CLOCKS_PER_SEC;
    std::cout << "[i] random graph initialization time: ";
    std::cout << std::fixed << elapsed_secs << " seconds (" << num_vertices(G) << " vertices)" << std::endl;
}

void RandomGraphBenchmark::deletionsTime(unsigned long deletions)
{
    int deletionsCount = 0;
    std::list<Edge> edges;

    if (deletions > num_edges(G)) {
        throw std::invalid_argument("RandomGraphBenchmark: Too many deletions");
    }

    EdgeIterator ei, ei_end;
    for(boost::tie(ei, ei_end) = boost::edges(G); ei != ei_end; ++ei) {
        edges.push_back(*ei);
        deletionsCount++;
        if (deletionsCount== deletions) {
            break;
        }
    }

    clock_t begin = clock();
    for(auto e = edges.begin(); e!=edges.end(); ++e) {
        G.deleteEdge(boost::source(*e, G), boost::target(*e, G));
    }
    clock_t end = clock();
    double elapsed_secs = double(end-begin) / CLOCKS_PER_SEC;

    std::cout << "[i] random graph deletion time: ";
    std::cout << std::fixed << elapsed_secs << " seconds (" << deletions << " deletions)" << std::endl;
}

void RandomGraphBenchmark::queryTime(unsigned long queries)
{
    Vertex limit = num_vertices(G) - 1;
    clock_t begin = clock();
    for (Vertex i=0; i<queries-1; ++i) {
        G.areConnected(Utilities::randomRange(0, limit), Utilities::randomRange(0, limit));
    }
    clock_t end = clock();
    double elapsed_secs = double(end-begin) / CLOCKS_PER_SEC;
    std::cout << "[i] random graph query time: ";
    std::cout << std::fixed << elapsed_secs << " seconds (" << queries << " queries)" << std::endl;
}
