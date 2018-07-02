#include "../incl/DynamicGraph.h"

DynamicGraph::DynamicGraph(leda::graph &g)
{
    this->G = &g;
}

void DynamicGraph::del_edge(leda::edge e)
{
    this->G->del_edge(e);
}


