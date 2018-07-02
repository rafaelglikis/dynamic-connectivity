#ifndef DYNAMIC_CONNECTIVITY_DYNAMICGRAPH_H
#define DYNAMIC_CONNECTIVITY_DYNAMICGRAPH_H

#include <LEDA/graph/graph.h>

class DynamicGraph
{
private:
    leda::graph* G;

public:
    explicit DynamicGraph(leda::graph &g);

    void del_edge(leda::edge e);
};


#endif
