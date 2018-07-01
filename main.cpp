#include <LEDA/graph/graph.h>

int main()
{
    leda::graph G;    //define directed graph G
    G.make_undirected();
    
    std::cout <<  	G.is_undirected() << std::endl;
    return 0;
}