#ifndef DYNAMIC_CONNECTIVITY_COMPLETEPARTSGRAPHBENCHMARK_H
#define DYNAMIC_CONNECTIVITY_COMPLETEPARTSGRAPHBENCHMARK_H


#include "Benchmark.h"

/**
 * PathCompleteGraphsBenchmark
 * Creates a path of complete graphs and runs the benchmark.
 */
class PathCompleteGraphsBenchmark : public Benchmark
{
private:
    std::set<Edge> deletionSet;
    double getDeletionsTime(unsigned long) override;
public:
    explicit PathCompleteGraphsBenchmark(int completeSubgraphs, int vertices);
    void run(unsigned long, unsigned long) override;
};


#endif //DYNAMIC_CONNECTIVITY_COMPLETEPARTSGRAPHBENCHMARK_H
