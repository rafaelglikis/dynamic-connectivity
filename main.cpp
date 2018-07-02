#include <LEDA/graph/graph.h>
#include "incl/DynamicGraph.h"

int main()
{
    leda::graph G;
    G.make_undirected();

    leda::node n0=G.new_node();
    leda::node n1=G.new_node();
    leda::node n2=G.new_node();
    leda::node n3=G.new_node();
    leda::node n4=G.new_node();
    leda::node n5=G.new_node();
    leda::node n6=G.new_node();
    leda::node n7=G.new_node();
    leda::node n8=G.new_node();
    leda::node n9=G.new_node();
    leda::node n10=G.new_node();

    leda::edge  e1 = G.new_edge(n1,n2);
    leda::edge  e2 = G.new_edge(n1,n0);
    leda::edge  e3 = G.new_edge(n1,n10);
    leda::edge  e4 = G.new_edge(n2,n3);
    leda::edge  e5 = G.new_edge(n3,n4);
    leda::edge  e6 = G.new_edge(n4,n5);
    leda::edge  e7 = G.new_edge(n5,n6);
    leda::edge  e8 = G.new_edge(n5,n7);
    leda::edge  e9 = G.new_edge(n6,n7);
    leda::edge  e10 = G.new_edge(n7,n8);
    leda::edge  e11 = G.new_edge(n7,n9);
    leda::edge  e12 = G.new_edge(n8,n9);
    leda::edge  e13 = G.new_edge(n10,n5);

    auto *dg = new DynamicGraph(G);
    dg->del_edge(e1);

    leda::edge e;
    forall_edges(e,G) {           //iterate over all edges e of G
        leda::node source=G.source(e);  //compute source of e
        leda::node target=G.target(e);  //compute target of e

        std::cout << "edge ";
        G.print_edge(e);          //print edge
        std::cout << " has source ";
        G.print_node(source);     //print source
        std::cout << " and target ";
        G.print_node(target);     //print target
        std::cout << std::endl;
    }

    return 0;
}