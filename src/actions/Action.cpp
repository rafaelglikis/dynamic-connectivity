#include "../../incl/actions/Action.h"

/**
 * Constructor
 * @param rel set of edges to operate on
 * @param e action edge
 */
Action::Action(std::set<Edge>& rel, Edge e)
{
    this->rel = &rel;
    this->edge = e;
}
