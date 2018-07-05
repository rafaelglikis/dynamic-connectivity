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

    G.init();

    G.visualize();

    G.deleteEdge(13, 15);
    G.deleteEdge(9, 10);
    G.deleteEdge(9, 11);
    G.deleteEdge(10, 11);
    G.deleteEdge(13, 11);
    G.deleteEdge(15, 11);
    G.deleteEdge(12, 10);
    G.deleteEdge(13, 12);

//    G.printInfo();

    //G.deleteEdge(14, 13);
    //G.deleteEdge(14, 15);

    G.visualize(true);
//    std::cout << G.areConnected(0,9) << std::endl;
//    std::cout << G.areConnected(0,1) << std::endl;

//    add_edge(0, 1, G);
//    add_edge(0, 2, G);
//    add_edge(1, 2, G);
//    add_edge(1, 3, G);
//    add_edge(2, 4, G);
//    G.init();
//    G.printInfo();
//    G.deleteEdge(0, 2);
//    G.printInfo();
//    G.visualize();

    return 0;
}

