#ifndef DYNAMIC_CONNECTIVITY_DELETEACTION_H
#define DYNAMIC_CONNECTIVITY_DELETEACTION_H


#include "Action.h"

class DeleteAction : public Action
{
private:
    std::set<Edge> rel;
    Edge edge;
public:
    DeleteAction(DynamicGraph&, Vertex, std::set<Edge>&, Edge);
    void undo() override;
};


#endif //DYNAMIC_CONNECTIVITY_DELETEACTION_H
