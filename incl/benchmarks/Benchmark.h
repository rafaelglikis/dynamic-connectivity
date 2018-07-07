#ifndef DYNAMIC_CONNECTIVITY_BENCHMARK_H
#define DYNAMIC_CONNECTIVITY_BENCHMARK_H


#include "../graph_types/DynamicGraph.h"

class Benchmark
{
protected:
    DynamicGraph G;
public:
    virtual void initTime() = 0;
    virtual void deletionsTime(unsigned long) = 0;
    virtual void queryTime(unsigned long) = 0;
};


#endif //DYNAMIC_CONNECTIVITY_BENCHMARK_H
