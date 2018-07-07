#ifndef DYNAMIC_CONNECTIVITY_RANDOMGRAPHBENCHMARK_H
#define DYNAMIC_CONNECTIVITY_RANDOMGRAPHBENCHMARK_H

#include "Benchmark.h"

class RandomGraphBenchmark : public Benchmark
{
public:
    explicit RandomGraphBenchmark(unsigned long, unsigned int);
    void initTime() override;
    void deletionsTime(unsigned long) override;
    void queryTime(unsigned long) override;
};


#endif //DYNAMIC_CONNECTIVITY_RANDOMGRAPHBENCHMARK_H
