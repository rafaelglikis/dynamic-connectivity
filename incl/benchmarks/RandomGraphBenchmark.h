#ifndef DYNAMIC_CONNECTIVITY_RANDOMGRAPHBENCHMARK_H
#define DYNAMIC_CONNECTIVITY_RANDOMGRAPHBENCHMARK_H

#include "Benchmark.h"

/**
 * RandomGraphBenchmark
 * Creates a random graph and runs the benchmark.
 */
class RandomGraphBenchmark : public Benchmark
{
public:
    explicit RandomGraphBenchmark(unsigned long, unsigned long);
    void run(unsigned long, unsigned long) override;
};


#endif //DYNAMIC_CONNECTIVITY_RANDOMGRAPHBENCHMARK_H
