#include "incl/DynamicGraph.h"
#include "boost/graph/adjacency_list.hpp"
#include <iostream>



int main()
{
    DynamicGraph G;

    add_edge(0, 1, G);
    add_edge(0, 2, G);
    add_edge(2, 3, G);

    add_vertex(G);
    G.init();
    G.visualize();

    return 0;
}

