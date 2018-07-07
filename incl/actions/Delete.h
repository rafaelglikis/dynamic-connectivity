#ifndef DYNAMIC_CONNECTIVITY_DELETEACTION_H
#define DYNAMIC_CONNECTIVITY_DELETEACTION_H


#include "Action.h"

class Delete : public Action
{
public:
    Delete(DynamicGraph&, Vertex, std::set<Edge>&, Edge);
    void undo() override;
};

#endif //DYNAMIC_CONNECTIVITY_DELETEACTION_H
