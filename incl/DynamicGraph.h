#ifndef DYNAMIC_CONNECTIVITY_DYNAMICGRAPH_H
#define DYNAMIC_CONNECTIVITY_DYNAMICGRAPH_H

#include <boost/graph/adjacency_list.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
        boost::no_property, boost::no_property, boost::no_property, boost::listS> UndirectedGraph;
typedef boost::graph_traits<UndirectedGraph>::vertex_descriptor Vertex;
typedef boost::graph_traits<UndirectedGraph>::edge_descriptor Edge;
typedef boost::graph_traits<UndirectedGraph>::vertex_iterator VertexIterator;
typedef boost::graph_traits<UndirectedGraph>::edge_iterator EdgeIterator;
typedef boost::graph_traits<UndirectedGraph>::out_edge_iterator OutEdgeIterator;

struct relatives {
    std::list<Edge> a_pred_edges;
    std::list<Edge> b_sibl_edges;
    std::list<Edge> c_succ_edges;
};

class DynamicGraph : public UndirectedGraph
{
private:
    std::vector<unsigned long> components;
    std::vector<int> dist;
    std::vector<relatives> relEdges;
    std::list<Edge> virtualEdges;
    unsigned long nextComponent = 0;

    bool breaksComponent(const Vertex&, const Vertex&);
    void bfs(const Vertex&);
    void updateRelEdges();
    void hideVirtualEdges();

public:
    void init();
    bool areConnected(const Vertex&, const Vertex&);
    void deleteEdge(const Vertex&, const Vertex&);
    void visualize();
    void printInfo();
};


#endif
