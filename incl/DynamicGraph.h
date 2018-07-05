#ifndef DYNAMIC_CONNECTIVITY_DYNAMICGRAPH_H
#define DYNAMIC_CONNECTIVITY_DYNAMICGRAPH_H

#include <boost/graph/adjacency_list.hpp>
#include <unordered_set>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
        boost::no_property, boost::no_property, boost::no_property,
        boost::listS> UndirectedGraph;
typedef boost::graph_traits<UndirectedGraph>::vertex_descriptor Vertex;
typedef boost::graph_traits<UndirectedGraph>::edge_descriptor Edge;
typedef boost::graph_traits<UndirectedGraph>::vertex_iterator VertexIterator;
typedef boost::graph_traits<UndirectedGraph>::edge_iterator EdgeIterator;
typedef boost::graph_traits<UndirectedGraph>::out_edge_iterator OutEdgeIterator;

struct Relatives {
    std::set<Edge> a_pred;
    std::set<Edge> b_sibl;
    std::set<Edge> c_succ;
};

class DynamicGraph : public UndirectedGraph
{
private:
    std::vector<unsigned long> components;
    std::vector<int> dist;
    std::vector<Relatives> relatives;
    std::set<Edge> virtualEdges;
    unsigned long nextComponent = 0;
    bool halt = false;

    // Initialization Operations
    void bfs(const Vertex&);
    void updateRelatives();

    // Deletion operations
    void hideVirtualEdges();
    void showVirtualEdges();

    void handleDeletion(const Vertex&, const Vertex&);
    bool checkComponentBreak(const Vertex &, const Vertex &);
    void updateVisitedComponents(std::list<Vertex>&);
    bool checkComponentNotBreak(Vertex, Vertex);

public:
    // Operations
    void init();
    bool areConnected(const Vertex&, const Vertex&);
    void deleteEdge(const Vertex&, const Vertex&);

    // Printers
    void visualize();
    void printInfo();
};

#endif
