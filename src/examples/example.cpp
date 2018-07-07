#include <iostream>
#include "../../incl/graph_types/DynamicGraph.h"

void example()
{
    DynamicGraph G;

    add_edge(0, 1, G);
    add_edge(0, 2, G);
    add_edge(2, 3, G);

    add_edge(4, 5, G);
    add_edge(4, 6, G);
    add_edge(6, 5, G);

    add_edge(7, 8, G);

    add_edge(9, 10, G);
    add_edge(9, 11, G);
    add_edge(10, 11, G);
    add_edge(13, 11, G);
    add_edge(15, 11, G);
    add_edge(10, 12, G);
    add_edge(12, 13, G);
    add_edge(14, 13, G);
    add_edge(14, 15, G);
    add_edge(13, 15, G);

    add_edge(16, 17, G);
    add_edge(16, 19, G);
    add_edge(18, 17, G);
    add_edge(18, 19, G);

    G.printInfo();
    G.visualize();

    G.deleteEdge(7, 8);

    G.printInfo();
    G.visualize();

    if(G.areConnected(7, 8)) {
        std::cout << "Vertices 7 and 8 are connected!" << std::endl;
    }
    else {
        std::cout << "Vertices 7 and 8 not are connected!" << std::endl;
    }
}