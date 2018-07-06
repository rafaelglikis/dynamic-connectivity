#include "../incl/Insert.h"
Insert::Insert(DynamicGraph &G, Vertex v, std::set<Edge> &rel, Edge e)
        : Action (G, v, rel, e) { }

void Insert::undo()
{
    this->rel->erase(this->edge);
}
