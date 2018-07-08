#ifndef DYNAMIC_CONNECTIVITY_INSERTACTION_H
#define DYNAMIC_CONNECTIVITY_INSERTACTION_H

#include "Action.h"

/**
 *  Insert
 *  represents a insert operation on a set of edges
 *  the deletion can be undone
 *  with the undo() method
 */
class Insert : public Action
{
public:
    Insert(std::set<Edge>&, Edge);
    void undo() override;
};


#endif //DYNAMIC_CONNECTIVITY_INSERTACTION_H
