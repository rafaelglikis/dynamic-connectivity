#include "../../incl/actions/Action.h"

Action::Action(std::set<Edge>& rel, Edge e)
{
    this->rel = &rel;
    this->edge = e;
}
