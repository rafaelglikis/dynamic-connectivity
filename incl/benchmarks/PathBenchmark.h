#ifndef DYNAMIC_CONNECTIVITY_PATHBENCHMARK_H
#define DYNAMIC_CONNECTIVITY_PATHBENCHMARK_H

#include "Benchmark.h"

class PathBenchmark : public Benchmark
{
public:
    explicit PathBenchmark(unsigned long);
    void initTime() override;
    void deletionsTime(unsigned long) override;
    void queryTime(unsigned long) override;
};


#endif //DYNAMIC_CONNECTIVITY_PATHBENCHMARK_H
