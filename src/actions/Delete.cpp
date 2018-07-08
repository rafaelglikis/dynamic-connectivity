#include "../../incl/actions/Delete.h"

/**
 * Constructor
 * @param rel set of edges to operate on
 * @param e action edge
 */
Delete::Delete(std::set<Edge> &rel, Edge e)
        : Action (rel, e) {}

/**
 * Undo the operation by inserting
 * an edge to the set of edges
 */
void Delete::undo()
{
    this->rel->insert(this->edge);
}
