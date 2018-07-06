#include "../incl/DeleteAction.h"

DeleteAction::DeleteAction(DynamicGraph &G, Vertex v, std::set<Edge> &rel, Edge e)
        : Action (G, v)
{
    this->rel = rel;
    this->edge = e;
}

void DeleteAction::undo()
{

}
