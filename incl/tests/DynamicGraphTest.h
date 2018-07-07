#ifndef DYNAMIC_CONNECTIVITY_DYNAMICGRAPHTEST_H
#define DYNAMIC_CONNECTIVITY_DYNAMICGRAPHTEST_H

#include "../graph_types/DynamicGraph.h"
#include "Test.h"

class DynamicGraphTest : public Test
{
private:
    DynamicGraph G;
    void testInit();
    void testNonBreakDelete();
    void testBreakDelete();
    void testBig();
    void testVirtualEdges(std::list<Edge> &);
public:
    DynamicGraphTest();
    void run() override;
};

#endif //DYNAMIC_CONNECTIVITY_DYNAMICGRAPHTEST_H
