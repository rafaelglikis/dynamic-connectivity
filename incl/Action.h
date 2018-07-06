#ifndef DYNAMIC_CONNECTIVITY_ACTION_H
#define DYNAMIC_CONNECTIVITY_ACTION_H


#include "DynamicGraph.h"
class DynamicGraph;
class Action
{
protected:
    DynamicGraph *G;
    Vertex v;
public:
    Action(DynamicGraph &, Vertex);
    virtual void undo() = 0;
};


#endif //DYNAMIC_CONNECTIVITY_ACTION_H
