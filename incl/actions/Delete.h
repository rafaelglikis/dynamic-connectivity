#ifndef DYNAMIC_CONNECTIVITY_DELETEACTION_H
#define DYNAMIC_CONNECTIVITY_DELETEACTION_H

#include "Action.h"

/**
 *  Delete
 *  represents a delete operation on a set of edges
 *  the deletion can be undone
 *  with the undo() method
 */
class Delete : public Action
{
public:
    Delete(std::set<Edge>&, Edge);
    void undo();
};

#endif //DYNAMIC_CONNECTIVITY_DELETEACTION_H
