#ifndef DYNAMIC_CONNECTIVITY_INSERTACTION_H
#define DYNAMIC_CONNECTIVITY_INSERTACTION_H


#include "Action.h"

class InsertAction : public Action
{
private:
    std::set<Edge> rel;
    Edge edge;
public:
    InsertAction(DynamicGraph&, Vertex, std::set<Edge>&, Edge);
    void undo() override;
};


#endif //DYNAMIC_CONNECTIVITY_INSERTACTION_H
