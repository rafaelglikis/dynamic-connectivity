#ifndef DYNAMIC_CONNECTIVITY_ACTION_H
#define DYNAMIC_CONNECTIVITY_ACTION_H


#include "../graph_types/DynamicGraph.h"
class DynamicGraph;
class Action
{
protected:
    std::set<Edge> *rel;
    Edge edge;
public:
    Action(std::set<Edge>&, Edge);
    virtual void undo() = 0;
};


#endif //DYNAMIC_CONNECTIVITY_ACTION_H
