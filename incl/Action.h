#ifndef DYNAMIC_CONNECTIVITY_ACTION_H
#define DYNAMIC_CONNECTIVITY_ACTION_H


#include "DynamicGraph.h"
class DynamicGraph;
class Action
{
protected:
    DynamicGraph *G;
    Vertex v;
    std::set<Edge> *rel;
    Edge edge;
public:
    Action(DynamicGraph &, Vertex, std::set<Edge>&, Edge);
    virtual void undo() = 0;
};


#endif //DYNAMIC_CONNECTIVITY_ACTION_H
