#ifndef DYNAMIC_CONNECTIVITY_COMPLETEPARTSGRAPHBENCHMARK_H
#define DYNAMIC_CONNECTIVITY_COMPLETEPARTSGRAPHBENCHMARK_H


#include "Benchmark.h"

class CompletePartsGraphBenchmark : public Benchmark
{
private:
    std::set<Edge> deletionSet;
    double getDeletionsTime(unsigned long) override;
public:
    explicit CompletePartsGraphBenchmark(int completeSubgraphs, int vertices);
    void run(unsigned long, unsigned long) override;
};


#endif //DYNAMIC_CONNECTIVITY_COMPLETEPARTSGRAPHBENCHMARK_H
