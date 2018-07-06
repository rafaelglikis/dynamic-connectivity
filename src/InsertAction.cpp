#include "../incl/InsertAction.h"
InsertAction::InsertAction(DynamicGraph &G, Vertex v, std::set<Edge> &rel, Edge e)
        : Action (G, v)
{
    this->rel = rel;
    this->edge = e;
}

void InsertAction::undo()
{

}
