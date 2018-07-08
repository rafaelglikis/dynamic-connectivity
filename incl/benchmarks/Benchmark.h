#ifndef DYNAMIC_CONNECTIVITY_BENCHMARK_H
#define DYNAMIC_CONNECTIVITY_BENCHMARK_H


#include "../graph_types/DynamicGraph.h"

class Benchmark
{
protected:
    DynamicGraph G;
    double getInitTime();
    double getDeletionsTime(unsigned long);
    double getQueryTime(unsigned long);
public:
    virtual void run(unsigned long, unsigned long) = 0;

};


#endif //DYNAMIC_CONNECTIVITY_BENCHMARK_H
