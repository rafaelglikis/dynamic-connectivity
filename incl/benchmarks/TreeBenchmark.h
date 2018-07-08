#ifndef DYNAMIC_CONNECTIVITY_TREE_H
#define DYNAMIC_CONNECTIVITY_TREE_H

#include "Benchmark.h"

class TreeBenchmark : public Benchmark
{
public:
    explicit TreeBenchmark(unsigned long);
    void run(unsigned long, unsigned long) override;
};


#endif //DYNAMIC_CONNECTIVITY_TREE_H
