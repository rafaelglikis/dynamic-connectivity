#ifndef DYNAMIC_CONNECTIVITY_DYNAMICGRAPH_H
#define DYNAMIC_CONNECTIVITY_DYNAMICGRAPH_H
#include "Graph.h"
#include "relatives.h"
#include <set>

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
    void buildBFSStructure(const Vertex &);
    void updateRelatives();

    // Deletion operations
    void handleDeletion(const Vertex&, const Vertex&);
    bool checkComponentBreak(const Vertex &, const Vertex &);
    void updateVisitedComponents(std::list<Vertex>&);
    bool checkComponentNotBreak(Vertex, Vertex);

public:
    // Operations
    void init();
    bool areConnected(const Vertex&, const Vertex&);
    void deleteEdge(const Vertex&, const Vertex&);
    void hideVirtualEdges();
    void showVirtualEdges();

    // Printers
    void visualize();
    void printInfo();
};

#endif
