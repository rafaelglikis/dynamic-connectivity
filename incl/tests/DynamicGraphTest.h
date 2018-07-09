#ifndef DYNAMIC_CONNECTIVITY_DYNAMICGRAPHTEST_H
#define DYNAMIC_CONNECTIVITY_DYNAMICGRAPHTEST_H

#include "../graph_types/DynamicGraph.h"
#include "Test.h"

/**
 * Tests DynamicGraph structure with the following tests:
 *  - init
 *  - non break delete
 *  - break delete
 *  - big test
 */
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
    void run();
};

#endif //DYNAMIC_CONNECTIVITY_DYNAMICGRAPHTEST_H
