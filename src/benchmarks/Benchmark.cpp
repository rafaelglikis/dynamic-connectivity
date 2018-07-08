#include "../../incl/benchmarks/Benchmark.h"
#include "../../incl/utilities/Utilities.h"

double Benchmark::getInitTime()
{
    clock_t begin = clock();
    G.init();
    clock_t end = clock();
    double elapsed_secs = double(end-begin) / CLOCKS_PER_SEC;
    return elapsed_secs;
}

double Benchmark::getDeletionsTime(unsigned long deletions)
{
    int deletionsCount = 0;
    std::vector<Edge> edges;

    EdgeIterator ei, ei_end;
    for(boost::tie(ei, ei_end) = boost::edges(G); ei != ei_end; ++ei) {
        edges.push_back(*ei);
        deletionsCount++;
        if (deletionsCount==deletions) {
            break;
        }
    }

    std::random_shuffle(edges.begin(), edges.end());

    clock_t begin = clock();
    for(auto e = edges.begin(); e!=edges.end(); ++e) {
        G.deleteEdge(boost::source(*e, G), boost::target(*e, G));
    }
    clock_t end = clock();
    double elapsed_secs = double(end-begin) / CLOCKS_PER_SEC;
}

double Benchmark::getQueryTime(unsigned long queries)
{
    Vertex limit = num_vertices(G) - 1;
    clock_t begin = clock();
    for (Vertex i=0; i<queries-1; ++i) {
        G.areConnected(Utilities::randomRange(0, limit), Utilities::randomRange(0, limit));
    }
    clock_t end = clock();
    double elapsed_secs = double(end-begin) / CLOCKS_PER_SEC;
}
