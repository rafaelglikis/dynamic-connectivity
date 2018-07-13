#include <iostream>
#include <fstream>
#include "../../incl/graph_types/DynamicGraph.h"

/**
 * Runs an example
 */
void runExample()
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
    std::ofstream file1;
    file1.open ("1.txt");
    G.visualize(file1);
    G.visualize();
    G.printInfo();
    file1.close();

    G.deleteEdge(12, 13);
    std::ofstream file2;
    file2.open("2.txt");
    G.visualize(file2);
    G.visualize();
    G.printInfo();
    file2.close();

    G.deleteEdge(7, 8);
    std::ofstream file3;
    file3.open("3.txt");
    G.visualize(file3);
    G.visualize();
    G.printInfo();
    file3.close();

    if(G.areConnected(7, 8)) {
        std::cout << "Vertices 7 and 8 are connected!" << std::endl;
    }
    else {
        std::cout << "Vertices 7 and 8 not are connected!" << std::endl;
    }
}