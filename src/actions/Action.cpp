#include "../../incl/actions/Action.h"

Action::Action(DynamicGraph &G, Vertex v, std::set<Edge>& rel, Edge e)
{
    this->G = &G;
    this->v = v;
    this->rel = &rel;
    this->edge = e;
}
