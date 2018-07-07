#include "../../incl/actions/Delete.h"

Delete::Delete(std::set<Edge> &rel, Edge e)
        : Action (rel, e) {}

void Delete::undo()
{
    this->rel->insert(this->edge);
}
