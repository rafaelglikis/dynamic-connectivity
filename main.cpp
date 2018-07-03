#include "incl/DynamicGraph.h"
#include "boost/graph/adjacency_list.hpp"
#include <iostream>



int main()
{
    DynamicGraph G;

    add_edge(0, 1, G);
    add_edge(0, 2, G);
    add_edge(2, 3, G);
    add_edge(4, 5, G);
    add_edge(7, 8, G);
    add_edge(4, 6, G);
    add_edge(6, 5, G);

    add_vertex(G);

    G.init();
    G.visualize();

    return 0;
}

