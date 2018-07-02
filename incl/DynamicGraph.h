#ifndef DYNAMIC_CONNECTIVITY_DYNAMICGRAPH_H
#define DYNAMIC_CONNECTIVITY_DYNAMICGRAPH_H

#include <boost/graph/adjacency_list.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, boost::no_property> UndirectedGraph;
typedef boost::graph_traits<UndirectedGraph>::vertex_descriptor Vertex;
typedef boost::graph_traits<UndirectedGraph>::edge_descriptor Edge;
typedef boost::graph_traits<UndirectedGraph>::vertex_iterator VertexIterator;
typedef boost::graph_traits<UndirectedGraph>::edge_iterator EdgeIterator;
typedef boost::graph_traits<UndirectedGraph>::out_edge_iterator OutEdgeIterator;


class DynamicGraph : public UndirectedGraph
{
private:
    std::vector<int> components;
    int nextComponent = 0;
    void dfsUpdateComponent(const long& s);

public:
    void visualize();
    void init();

};


#endif
