#include "../incl/Action.h"

Action::Action(DynamicGraph &G, Vertex v)
{
    this->G = &G;
    this->v = v;
}
