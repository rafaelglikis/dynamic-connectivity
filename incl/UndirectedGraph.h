#ifndef DYNAMIC_CONNECTIVITY_GRAPH_H
#define DYNAMIC_CONNECTIVITY_GRAPH_H

#include <boost/graph/adjacency_list.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
        boost::no_property, boost::no_property, boost::no_property,
        boost::listS> UndirectedGraph;
typedef boost::graph_traits<UndirectedGraph>::vertex_descriptor Vertex;
typedef boost::graph_traits<UndirectedGraph>::edge_descriptor Edge;
typedef boost::graph_traits<UndirectedGraph>::vertex_iterator VertexIterator;
typedef boost::graph_traits<UndirectedGraph>::edge_iterator EdgeIterator;
typedef boost::graph_traits<UndirectedGraph>::out_edge_iterator OutEdgeIterator;

#endif //DYNAMIC_CONNECTIVITY_GRAPH_H
