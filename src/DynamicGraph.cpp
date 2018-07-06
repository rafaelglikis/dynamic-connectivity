#include "../incl/DynamicGraph.h"
#include "../incl/Delete.h"
#include "../incl/Insert.h"
#include <iostream>
#include <algorithm>
#include <sstream>
#include <omp.h>

void DynamicGraph::init()
{
    this->components.resize(num_vertices(*this));
    this->dist.resize(num_vertices(*this));
    this->relatives.resize(num_vertices(*this));

    buildBFSStructure(0);
    this->updateRelatives();
}

void DynamicGraph::buildBFSStructure(const Vertex &s)
{
    std::vector<bool> visited(boost::num_vertices(*this));
    this->nextComponent++;
    this->bfs(s, visited, 0);

    while(std::find(visited.begin(), visited.end(), 0) != visited.end() ) {
        // Add Virtual Edges
        for(Vertex i=1; i<dist.size(); ++i ) {
            if(!visited[i]){
                boost::add_edge(0, i, *this);
                this->virtualEdges.insert(edge(0,i,*this).first);

                this->nextComponent++;
                this->bfs(i, visited, 1);
            }
        }
    }
}

void DynamicGraph::bfs(const Vertex &s, std::vector<bool> &visited, const int startingDistance)
{
    std::list<Vertex> queue;
    this->dist[s] = startingDistance;
    visited[s]=true;
    this->components[s] = this->nextComponent;
    queue.push_back(s);

    // Basic BFS
    while(!queue.empty()) {
        Vertex v = queue.front();
        queue.pop_front();

        OutEdgeIterator ei, ei_end;
        for(boost::tie(ei, ei_end) = out_edges(v, *this); ei != ei_end; ++ei) {
            Vertex u = target(*ei, *this);
            if(!visited[u]){
                dist[u] = this->dist[v]+1;
                this->components[u] = this->nextComponent;
                visited[u]=true;
                queue.push_back(u);
            }
        }
    }
}

void DynamicGraph::updateRelatives()
{
    for(Vertex v = 0; v < this->dist.size(); v++) {
        OutEdgeIterator ei, ei_end;
        for(boost::tie(ei, ei_end) = out_edges(v, *this); ei != ei_end; ++ei) {
            Vertex u = target(*ei, *this);
            if(this->dist[v] == this->dist[u] + 1 ) {
                this->relatives[v].a_pred.insert(*ei);
            }
            else if(this->dist[v] == this->dist[u]) {
                this->relatives[v].b_sibl.insert(*ei);
            }
            else if(this->dist[v] == this->dist[u] - 1 ) {
                this->relatives[v].c_succ.insert(*ei);
            }
        }
    }
}

void DynamicGraph::hideVirtualEdges()
{
    for(auto ei = this->virtualEdges.begin(); ei != virtualEdges.end(); ++ei) {
        Edge e = *ei;
        boost::remove_edge(*ei, *this);
    }
}

void DynamicGraph::showVirtualEdges()
{
    for(auto ei = this->virtualEdges.begin(); ei != virtualEdges.end(); ++ei) {
        Vertex s = source(*ei, *this);
        Vertex t = target(*ei, *this);
        boost::add_edge(s, t, *this);
    }
}

bool DynamicGraph::areConnected(const Vertex &v, const Vertex &u)
{
    return this->components[v] == this->components[u];
}

void DynamicGraph::deleteEdge(const Vertex &v, const Vertex &u)
{
    bool edgeExist = edge(v,u,*this).second;
    if (edgeExist) {
        Edge e = edge(v,u,*this).first;
        boost::remove_edge(e, *this);
        this->handleDeletion(e);
    }
    else {
        throw std::invalid_argument("Edge does not exist");
    }
}

void DynamicGraph::handleDeletion(Edge e)
{
    Vertex v = source(e, *this);
    Vertex u = target(e, *this);
    this->halt = false;
    #pragma omp parallel sections
    {
        #pragma omp section
        {
            checkComponentNotBreak(v, u, e);
        }

        #pragma omp section
        {
            if(checkComponentBreak(v, u)) {
                add_edge(v, u, *this);
                this->virtualEdges.insert(edge(v, u, *this).first);
            }

        }
    }
}

bool DynamicGraph::checkComponentBreak(const Vertex &v, const Vertex &u)
{
    // Using bidirectional BFS
    std::list<Vertex> queueV;
    std::list<Vertex> queueU;
    std::vector<bool> visitedV(boost::num_vertices(*this), false);
    std::vector<bool> visitedU(boost::num_vertices(*this), false);
    std::list<Vertex> visitedListV;
    std::list<Vertex> visitedListU;

    visitedV[v]=true;
    visitedU[u]=true;
    queueV.push_back(v);
    queueU.push_back(u);
    visitedListV.push_back(v);
    visitedListU.push_back(u);

    while(!queueV.empty() && !queueU.empty()) {
        if (this->halt) {
            return false;
        }

        if(!this->bdbfsStep(queueV, visitedV, visitedU, visitedListV) || this->halt) {
            return false;
        }

        if (queueV.empty()) {
            this->halt = true;
            updateVisitedComponents(visitedListV);
            break;
        }

        if(!this->bdbfsStep(queueU, visitedU, visitedV, visitedListU) || this->halt) {
            return false;
        }

        if (queueU.empty()) {
            this->halt = true;
            updateVisitedComponents(visitedListU);
            break;
        }
    }
    return true;
}

bool DynamicGraph::bdbfsStep(std::list<Vertex>& queue, std::vector<bool>& visitedV,
               std::vector<bool>& visitedU, std::list<Vertex>& visitedList)
{
    Vertex vv = queue.front();
    visitedV[vv]=true;
    queue.pop_front();

    OutEdgeIterator ei, ei_end;
    for(boost::tie(ei, ei_end) = out_edges(vv, *this); ei != ei_end; ++ei) {
        Vertex vvTarget = target(*ei, *this);
        if (visitedU[vvTarget]) {
            return false;
        }
        if(!visitedV[vvTarget] && !virtualEdges.count(*ei)){
            queue.push_back(vvTarget);
            visitedList.push_back(vvTarget);
        }
    }
    return true;
}

void DynamicGraph::updateVisitedComponents(const std::list<Vertex>& visited)
{
    this->nextComponent++;
    for (auto v = visited.begin(); v != visited.end(); ++v){
        this->components[*v] = this->nextComponent;
    }
}

bool DynamicGraph::checkComponentNotBreak(Vertex v, Vertex u, Edge e)
{
    // Case 1: v and u are on the same level
    if (this->dist[v]==this->dist[u]) {
        this->halt = true;
        this->relatives[v].b_sibl.erase(e);
        this->relatives[u].b_sibl.erase(e);
        return true;
    }

    // Case 2: v and u are on different levels
    if (this->dist[v]!=this->dist[u]) {
        std::list<Action*> actions;
        std::list<Vertex> incVertices;
        // For Generality u in Li-1 and v Li
        if (this->dist[v] < this->dist[u]) {
            Vertex tmp = v;
            v = u;
            u = tmp;
        }

        actions.push_front(new Delete(*this, u, this->relatives[u].c_succ, e));
        this->relatives[u].c_succ.erase(e);
        actions.push_front(new Delete(*this, v, this->relatives[v].a_pred, e));
        this->relatives[v].a_pred.erase(e);

        // Case 2.1: a_pred(v) is not empty
        if(!this->relatives[v].a_pred.empty()) {
            this->halt = true;

            while (!actions.empty()) {
                delete actions.front();
                actions.pop_front();
            }

            return true;
        }
        // Case 2.2 a_pred(v) is empty
        else {
            std::list<Vertex> queue;
            queue.push_back(v);

            // If Q is empty, the procedure and both processes halt
            while(!queue.empty()) {
                // Let w be the first element of Q. Remove w from Q
                Vertex w = queue.front();
                queue.pop_front();

                // Remove w from its level (say, Lj), and put it in the next level (Lj+1)
                incVertices.push_front(w);
                this->dist[w]++;

                // For each edge e (w---w') in b_sibl(w)
                for(auto ei = this->relatives[w].b_sibl.begin(); ei!=this->relatives[w].b_sibl.end(); ++ei) {
                    Vertex ww = (w==target(*ei, *this)) ? source(*ei, *this) : target(*ei, *this);
                    // remove e from b_sibl(w')
                    actions.push_front(new Delete(*this, ww, this->relatives[ww].b_sibl, *ei));
                    this->relatives[ww].b_sibl.erase(*ei);
                    // and put it in c_succ(w')
                    actions.push_front(new Insert(*this, ww, this->relatives[ww].c_succ, *ei));
                    this->relatives[ww].c_succ.insert(*ei);
                }

                // a_pred(w) <- b_pred(w)
                for(auto ei = this->relatives[w].a_pred.begin(); ei!=this->relatives[w].a_pred.end(); ++ei) {
                    actions.push_front(new Delete(*this, w, this->relatives[w].a_pred, *ei));
                }
                this->relatives[w].a_pred.clear();
                for(auto ei = this->relatives[w].b_sibl.begin(); ei!=this->relatives[w].b_sibl.end(); ++ei) {
                    actions.push_front(new Insert(*this, w, this->relatives[w].a_pred, *ei));
                    this->relatives[w].a_pred.insert(*ei);
                }

                // For each edge e (w---w') in c_succ(w)
                for(auto ei = this->relatives[w].c_succ.begin(); ei!=this->relatives[w].c_succ.end(); ++ei) {
                    Vertex ww = (w==target(*ei, *this)) ? source(*ei, *this) : target(*ei, *this);
                    // remove e from a_pred(w')
                    actions.push_front(new Delete(*this, ww, this->relatives[ww].a_pred, *ei));
                    this->relatives[ww].a_pred.erase(*ei);
                    // and put it in b_sibl(w');
                    actions.push_front(new Insert(*this, ww, this->relatives[ww].b_sibl, *ei));
                    this->relatives[ww].b_sibl.insert(*ei);
                    // if the new a_pred(w') is empty, put w' on Q.
                    if(this->relatives[ww].a_pred.empty()) {
                        queue.push_back(ww);
                    }
                }

                // b_sibl(w) <- c_succ(w)
                for(auto ei = this->relatives[w].b_sibl.begin(); ei!=this->relatives[w].b_sibl.end(); ++ei) {
                    actions.push_front(new Delete(*this, w, this->relatives[w].b_sibl, *ei));
                }
                this->relatives[w].b_sibl.clear();
                for(auto ei = this->relatives[w].c_succ.begin(); ei!=this->relatives[w].c_succ.end(); ++ei) {
                    actions.push_front(new Insert(*this, w, this->relatives[w].b_sibl, *ei));
                    this->relatives[w].b_sibl.insert(*ei);
                }

                // c_succ(w) <- {empty}
                for(auto ei = this->relatives[w].c_succ.begin(); ei!=this->relatives[w].c_succ.end(); ++ei) {
                    actions.push_front(new Delete(*this, w, this->relatives[w].c_succ, *ei));
                }
                this->relatives[w].c_succ.clear();

                // If a_pred(w) is empty, put w on Q.
                if(this->relatives[w].a_pred.empty()) {
                    queue.push_back(w);
                }

                // Repeat
                if(this->halt) {
                    this->rollBack(actions, incVertices);
                    return false;
                }
            }
        }

        while (!actions.empty()) {
            delete actions.front();
            actions.pop_front();
        }

        this->halt = true;
        return true;
    }
}

void DynamicGraph::rollBack(std::list<Action*> &actions, std::list<Vertex>& incVertices)
{
    while (!actions.empty()) {
        Action* action = actions.front();
        actions.pop_front();
        action->undo();
        delete action;
    }

    while(!incVertices.empty()) {
        Vertex v = incVertices.front();
        incVertices.pop_front();
        this->dist[v]--;
    }
}

/**
 * Prints graph in GraphViz
 * https://dreampuf.github.io/GraphvizOnline/
 */
void DynamicGraph::visualize() const
{
    // Creating components streams
    std::vector<std::ostringstream> subgraphs(this->nextComponent);
    for(int i = 0; i<this->nextComponent; ++i) {
        subgraphs[i] << "  subgraph cluster" << i << " { label = \"Component " << i+1 << "\" ";
    }
    for(int i = 0; i<this->components.size(); ++i) {
        subgraphs[components[i]-1] << i << "; ";
    }
    for(int i = 0; i<this->nextComponent; ++i) {
        subgraphs[i] << " }\n";
    }

    // Printing graph style
    std::cout << "graph G {" << std::endl;
    std::cout << "  size=\"5,3\"" << std::endl;
    std::cout << "  ratio=\"fill\"" << std::endl;
    std::cout << "  edge[style=\"bold\"]" << std::endl;
    std::cout << "  node[shape=\"circle\"]" << std::endl;
    // Print subgraphs by vertices
    for(int i = 0; i<this->nextComponent; ++i) {
        std::cout  << subgraphs[i].str();
    }
    // Print edges
    EdgeIterator ei, ei_end;
    for (tie(ei, ei_end) = edges(*this); ei != ei_end; ++ei) {
        Vertex u,v;
        Edge e = *ei;
        u = source(e, *this);
        v = target(e, *this);
        std::cout << "  " << u << " -- " << v;
        if (virtualEdges.count(*ei)) std::cout << " [style=dotted]";
        std::cout << ";" << std::endl;
    }

    std::cout << "}" << std::endl;
}


/**
 * Prints Information about the dynamic graph
 */
void DynamicGraph::printInfo() const
{
    for (Vertex v = 0; v < this->dist.size(); v++) {
        std::cout << "Vertex " << v  << std::endl;
        std::cout << " - component " << this->components[v] << std::endl;
        std::cout << " - dist " << this->dist[v] << std::endl;
        std::cout << " - pred: ";
        for (auto ei = this->relatives[v].a_pred.begin(); ei != this->relatives[v].a_pred.end(); ei++) {
            Vertex u = source(*ei, *this);
            Vertex v = target(*ei, *this);
            Edge e = edge(v, u, *this).first;
            if(this->virtualEdges.count(e)) {
                std::cout << "~";
            }
            std::cout << e << " ";
        }
        std::cout << std::endl;
        std::cout << " - sibl: ";
        for (auto ei = this->relatives[v].b_sibl.begin(); ei != this->relatives[v].b_sibl.end(); ei++) {
            Vertex u = source(*ei, *this);
            Vertex v = target(*ei, *this);
            Edge e = edge(v, u, *this).first;
            if(this->virtualEdges.count(e)) {
                std::cout << "~";
            }
            std::cout << e << " ";
        }
        std::cout << std::endl;
        std::cout << " - succ: ";
        for (auto ei = this->relatives[v].c_succ.begin(); ei != this->relatives[v].c_succ.end(); ei++) {
            Vertex u = source(*ei, *this);
            Vertex v = target(*ei, *this);
            Edge e = edge(v, u, *this).first;
            if(this->virtualEdges.count(e)) {
                std::cout << "~";
            }
            std::cout << e << " ";
        }
        std::cout << std::endl;
    }
}














