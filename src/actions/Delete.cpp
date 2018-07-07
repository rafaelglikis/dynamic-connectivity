#include "../../incl/actions/Delete.h"

Delete::Delete(DynamicGraph &G, Vertex v, std::set<Edge> &rel, Edge e)
        : Action (G, v, rel, e) {}

void Delete::undo()
{
    this->rel->insert(this->edge);
}
