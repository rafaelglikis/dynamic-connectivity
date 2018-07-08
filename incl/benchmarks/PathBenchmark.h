#ifndef DYNAMIC_CONNECTIVITY_PATHBENCHMARK_H
#define DYNAMIC_CONNECTIVITY_PATHBENCHMARK_H

#include "Benchmark.h"

/**
 * PathBenchmark
 * Creates a path graph and runs the benchmark.
 */
class PathBenchmark : public Benchmark
{
public:
    explicit PathBenchmark(unsigned long);
    void run(unsigned long, unsigned long) override;
};


#endif //DYNAMIC_CONNECTIVITY_PATHBENCHMARK_H
