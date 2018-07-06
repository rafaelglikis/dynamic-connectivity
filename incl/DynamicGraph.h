#ifndef DYNAMIC_CONNECTIVITY_DYNAMICGRAPH_H
#define DYNAMIC_CONNECTIVITY_DYNAMICGRAPH_H

#include "UndirectedGraph.h"
#include "Relatives.h"
#include <set>

class Action;
class DynamicGraph : public UndirectedGraph
{
private:
    std::vector<unsigned long> components;
    std::vector<int> dist;
    std::set<Edge> virtualEdges;
    unsigned long nextComponent = 0;
    std::vector<Relatives> relatives;
    bool halt = false;

    // Initialization Operations
    void buildBFSStructure(const Vertex&);
    void bfs(const Vertex& , std::vector<bool>&, const int);

    void updateRelatives();
    // Deletion operations
    void handleDeletion(Edge);
    bool checkComponentBreak(const Vertex &, const Vertex &);
    void updateVisitedComponents(const std::list<Vertex>&);
    bool bdbfsStep(std::list<Vertex>&, std::vector<bool>&,
            std::vector<bool>&, std::list<Vertex>&);
    bool checkComponentNotBreak(Vertex, Vertex, Edge);

    void rollBack(std::list<Action*>&, std::list<Vertex>&);
public:

    // Operations
    void init();
    bool areConnected(const Vertex&, const Vertex&);
    void deleteEdge(const Vertex&, const Vertex&);
    void hideVirtualEdges();
    void showVirtualEdges();

    // Printers
    void visualize() const;
    void printInfo() const;
};

#endif
