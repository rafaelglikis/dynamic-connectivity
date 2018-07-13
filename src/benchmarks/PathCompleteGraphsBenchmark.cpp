#include <iostream>
#include "../../incl/benchmarks/PathCompleteGraphsBenchmark.h"

/**
 * Creates a path of @param(completeSubgraphs) complete graphs
 * with @param(vertices) vertices each.
 * @param completeSubgraphs
 * @param vertices
 */
PathCompleteGraphsBenchmark::PathCompleteGraphsBenchmark(int completeSubgraphs, int vertices)
{

    for (Vertex i=0; i<vertices; ++i) {
        for (Vertex j=i; j<vertices; ++j) {
            if (j==i) continue;
            add_edge(i, j, G);
        }
    }

    for (Vertex k=1; k<completeSubgraphs; ++k) {
        Vertex offset = k*vertices;
        for (Vertex i=offset; i<offset+vertices; ++i) {
            for (Vertex j=i; j<offset+vertices; ++j) {
                if (j==i) continue;
                add_edge(i, j, G);
            }
        }
        add_edge(offset-1, offset, G);
        this->deletionSet.insert(edge(offset-1, offset, G).first);
    }
}

/**
 * Runs getInitTime(), getDeletionsTime(deletions), getQueryTime(queries) for the specified parameters
 * and prints the result.
 * throws invalid_argument exepion if the deletions are too many
 * @param deletions (number of deletions)
 * @param queries (number of queries)
 */
void PathCompleteGraphsBenchmark::run(unsigned long deletions, unsigned long queries)
{
    if (deletions > num_edges(G)) {
        throw std::invalid_argument("PathBenchmark: Too many deletions");
    }

    std::cout << "[i] complete parts graph initialization time: " << std::fixed << this->getInitTime();
    std::cout << " seconds (" << num_vertices(G) << " vertices " << num_edges(G) << " edges)" << std::endl;
    std::cout << "[i] complete parts graph deletion time: " << std::fixed << this->getDeletionsTime(deletions);
    std::cout << " seconds (" << deletions << " deletions)" << std::endl;
    std::cout << "[i] complete parts graph query time: ";
    std::cout << std::fixed << this->getQueryTime(queries) << " seconds (" << queries << " queries)" << std::endl;
}

/**
 * Override of getDeletionsTime()
 * This method deletes the edges connecting the complete subgraphs (deletionSet) first
 * in order to get worse times.
 * @param deletions (number of deletions)
 * @return time
 */
double PathCompleteGraphsBenchmark::getDeletionsTime(unsigned long deletions)
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

    // put the edges connecting the complete subgraphs at the beginning
    int counter = 0;
    for (auto i=this->deletionSet.begin(); i!=this->deletionSet.end(); ++i) {
        if (counter>=deletions) break;
        edges[counter] = *i;
    }

    clock_t begin = clock();
    for(auto e = edges.begin(); e!=edges.end(); ++e) {
        G.deleteEdge(boost::source(*e, G), boost::target(*e, G));
    }
    clock_t end = clock();
    double elapsed_secs = double(end-begin) / CLOCKS_PER_SEC;
    return elapsed_secs;
}
