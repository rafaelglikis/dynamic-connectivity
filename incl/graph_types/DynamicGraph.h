#ifndef DYNAMIC_CONNECTIVITY_DYNAMICGRAPH_H
#define DYNAMIC_CONNECTIVITY_DYNAMICGRAPH_H

#include "UndirectedGraph.h"
#include "Relatives.h"
#include <set>
class Action;

/**
 * DynamicGraph
 * for Even & Shiloach [ES81] Decremental Dynamic Algorithm
 * Maintains the a dynamic "BFS" structure in order to answer
 * if two vertices are in the same connected component in constant time.
 *
 */
class DynamicGraph : public UndirectedGraph
{
private:
    std::vector<unsigned long> components;
    std::vector<unsigned long > level;
    std::set<Edge> virtualEdges;
    unsigned long nextComponent = 0;
    std::vector<Relatives> relatives;
    bool halt = false;
    bool isInitialized = false;

    // Initialization Operations
    void buildBFSStructure(const Vertex&);
    void bfs(const Vertex& , std::vector<bool>&, int);

    void updateRelatives();
    // Deletion operations
    void handleDeletion(Edge);
    bool checkComponentBreak(const Vertex &, const Vertex &);
    bool bdbfsStep(std::list<Vertex>&, std::vector<bool>&,
            std::vector<bool>&, std::list<Vertex>&);
    void updateVisitedComponents(const std::list<Vertex> &);
    bool checkComponentNotBreak(Vertex, Vertex, Edge);
    void dropLevel(Vertex w, std::list<Vertex> &,
            std::list<Action*> &, std::list<Vertex> &);
    void rollBack(std::list<Action*>&, std::list<Vertex>&);

public:
    // Operations
    void init();
    bool areConnected(const Vertex&, const Vertex&);
    void deleteEdge(const Vertex&, const Vertex&);
    void hideVirtualEdges();
    void showVirtualEdges();

    // getters
    int getLevel(Vertex v) const;
    unsigned long getComponent(Vertex v) const;
    std::list<Edge> getVirtualEdges() const;

    // Printers
    void visualize() const;
    void printInfo() const;

};

#endif
