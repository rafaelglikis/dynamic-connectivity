#ifndef DYNAMIC_CONNECTIVITY_ACTIONTEST_H
#define DYNAMIC_CONNECTIVITY_ACTIONTEST_H

#include "Test.h"
#include <vector>
#include "../graph_types/Relatives.h"
#include "../graph_types/DynamicGraph.h"

class ActionTest : public Test
{
private:
    DynamicGraph G;
    std::vector<Relatives> rel;
    void insertTest();
    void deleteTest();
public:
    ActionTest();
    void run() override;
};


#endif //DYNAMIC_CONNECTIVITY_ACTIONTEST_H
