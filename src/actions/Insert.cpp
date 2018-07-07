#include "../../incl/actions/Insert.h"
Insert::Insert(std::set<Edge> &rel, Edge e)
        : Action (rel, e) { }

void Insert::undo()
{
    this->rel->erase(this->edge);
}
