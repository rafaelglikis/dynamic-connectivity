#ifndef DYNAMIC_CONNECTIVITY_INSERTACTION_H
#define DYNAMIC_CONNECTIVITY_INSERTACTION_H


#include "Action.h"

class Insert : public Action
{
public:
    Insert(std::set<Edge>&, Edge);
    void undo() override;
};


#endif //DYNAMIC_CONNECTIVITY_INSERTACTION_H
