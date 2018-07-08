#include "../../incl/actions/Insert.h"

/**
 * Constructor
 * @param rel set of edges to operate on
 * @param e action edge
 */
Insert::Insert(std::set<Edge> &rel, Edge e)
        : Action (rel, e) { }

/**
 * Undo the operation by deleting
 * an edge from the set of edges
 */
void Insert::undo()
{
    this->rel->erase(this->edge);
}
