#ifndef DYNAMIC_CONNECTIVITY_INCREAMENTDISTANCE_H
#define DYNAMIC_CONNECTIVITY_INCREAMENTDISTANCE_H

#include "Action.h"

class IncreamentDistance : public Action
{
public:
    IncreamentDistance(DynamicGraph&, Vertex);

    void undo() override;
};


#endif //DYNAMIC_CONNECTIVITY_INCREAMENTDISTANCE_H
