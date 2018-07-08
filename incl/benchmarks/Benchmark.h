#ifndef DYNAMIC_CONNECTIVITY_BENCHMARK_H
#define DYNAMIC_CONNECTIVITY_BENCHMARK_H


#include "../graph_types/DynamicGraph.h"

/**
 * Abstract Benchmark
 * Mesures the time of Dynamic Graph tasks (init delete query)
 * and prints the result (run method).
 */
class Benchmark
{
protected:
    DynamicGraph G;
    double getInitTime();
    virtual double getDeletionsTime(unsigned long);
    double getQueryTime(unsigned long);
public:
    virtual void run(unsigned long, unsigned long) = 0;

};


#endif //DYNAMIC_CONNECTIVITY_BENCHMARK_H
