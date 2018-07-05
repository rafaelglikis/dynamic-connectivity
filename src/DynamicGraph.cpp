#include "../incl/DynamicGraph.h"
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
    std::list<Vertex> queue;
    std::vector<bool> visited(boost::num_vertices(*this));
    this->nextComponent++;

    visited[s]=true;
    this->dist[s] = 0;
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



    while(std::find(visited.begin(), visited.end(), 0) != visited.end() ) {
        // Add Virtual Edges
        for(Vertex i=1; i<dist.size(); ++i ) {
            if(!visited[i]){
                boost::add_edge(0, i, *this);
                this->virtualEdges.insert(edge(0,i,*this).first);

                this->nextComponent++;
                visited[i]=true;
                this->dist[i] = 1;
                this->components[i] = this->nextComponent;
                queue.push_back(i);

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
            // Component Not Break
            checkComponentNotBreak(v, u, e);
        }

        #pragma omp section
        {
            // Component Breaks
            if(checkComponentBreak(v, u)) {
                add_edge(v, u, *this);
                this->virtualEdges.insert(edge(v, u, *this).first);
            }

        }
    }
}

bool DynamicGraph::checkComponentBreak(const Vertex &v, const Vertex &u)
{
    // Using parallel BFS
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
        OutEdgeIterator ei, ei_end;

        Vertex vv = queueV.front();
        visitedV[vv]=true;
        queueV.pop_front();

        Vertex uu = queueU.front();
        visitedU[uu]=true;
        queueU.pop_front();

        for(boost::tie(ei, ei_end) = out_edges(vv, *this); ei != ei_end; ++ei) {
            Vertex vvTarget = target(*ei, *this);
            if(!visitedV[vvTarget] && !virtualEdges.count(*ei)){
                queueV.push_back(vvTarget);
                visitedListV.push_back(vvTarget);
            }
            else if (u == vvTarget) {
                return false;
            }
        }

        for(boost::tie(ei, ei_end) = out_edges(uu, *this); ei != ei_end; ++ei) {
            Vertex uuTarget = target(*ei, *this);
            if(!visitedU[uuTarget] && !virtualEdges.count(*ei)) {
                queueU.push_back(uuTarget);
                visitedListU.push_back(uuTarget);
            }
            else if (v == uuTarget) {
                return false;
            }
        }

        if (this->halt) {
            return false;
        }
    }

    #pragma omp critical
    {
        this->halt = true;
    }

    if (queueV.empty()) {
        updateVisitedComponents(visitedListV);
    }

    if (queueU.empty()) {
        updateVisitedComponents(visitedListU);
    }

    return true;
}

void DynamicGraph::updateVisitedComponents(const std::list<Vertex>& visited)
{
    this->nextComponent ++;
    for (auto v = visited.begin(); v != visited.end(); ++v){
        this->components[*v] = this->nextComponent;
    }
    Vertex v = *visited.begin();

//    // *************************************************
//    add_edge(0, v, *this);
//    this->virtualEdges.insert(edge(0, v, *this).first);
//    // *************************************************
}

bool DynamicGraph::checkComponentNotBreak(Vertex v, Vertex u, Edge e)
{
    // Case 1: v and u are on the same level
    if (this->dist[v]==this->dist[u]) {
        std::cout << "Case 1: " << v << " and " << u << " are on the same level" << std::endl;
        this->halt = true;
        this->relatives[v].b_sibl.erase(e);
        this->relatives[u].b_sibl.erase(e);
        return true;
    }

    // Keep the old structure deep copy
    std::vector<int> oldDist(this->dist);
    std::vector<Relatives> oldRelatives(this->relatives);

    // Case 2: v and u are on different levels
    if (this->dist[v]!=this->dist[u]) {
        std::cout << "Case 2:  "<< v << " and " << u << " are on different levels" << std::endl;
        // For Generality u in Li-1 and v Li
        if (this->dist[v] < this->dist[u]) {
            Vertex tmp = v;
            v = u;
            u = tmp;
        }
        this->relatives[u].c_succ.erase(e);
        this->relatives[v].a_pred.erase(e);

        // Case 2.1: a_pred(v) is not empty
        if(!this->relatives[v].a_pred.empty()) {
            std::cout << "Case 2.1: a_pred(" << v << ") is not empty" << std::endl;
            this->halt = true;
            return true;
        }
        // Case 2.2 a_pred(v) is empty
        else {
            std::cout << "Case 2.1: a_pred(" << v << ") is empty" << std::endl;
            std::list<Vertex> queue;
            queue.push_back(v);

            // If Q is empty, the procedure and both processes halt
            while(!queue.empty()) {
                if(this->halt) {
                    this->dist = oldDist;
                    this->relatives = oldRelatives;
                    return false;
                }

                // Let w be the first element of Q. Remove w from Q
                Vertex w = queue.front();
                queue.pop_front();

                // Remove w from its level (say, Lj), and put it in the next level (Lj+1)
                this->dist[w]++;

                // For each edge e (w---w') in b_sibl(w)
                for(auto e = this->relatives[w].b_sibl.begin(); e!=this->relatives[w].b_sibl.end(); ++e) {
                    Vertex ww = (w==target(*e, *this)) ? source(*e, *this) : target(*e, *this);
                    this->relatives[ww].b_sibl.erase(*e); // remove e from b_sibl(w')
                    this->relatives[ww].c_succ.insert(*e); // and put it in c_succ(w')
                }

                // a_pred(w) <- b_pred(w)
                this->relatives[w].a_pred.clear();
                for(auto e = this->relatives[w].b_sibl.begin(); e!=this->relatives[w].b_sibl.end(); ++e) {
                    this->relatives[w].a_pred.insert(*e);
                }

                // For each edge e (w---w') in c_succ(w)
                for(auto e = this->relatives[w].c_succ.begin(); e!=this->relatives[w].c_succ.end(); ++e) {
                    Vertex ww = (w==target(*e, *this)) ? source(*e, *this) : target(*e, *this);
                    this->relatives[ww].a_pred.erase(*e); // remove e from a_pred(w')
                    this->relatives[ww].b_sibl.insert(*e); // and put it in b_sibl(w');
                    // if the new a_pred(w') is empty, put w' on Q.
                    if(this->relatives[ww].a_pred.empty()) {
                        queue.push_back(ww);
                    }
                }

                // b_sibl(w) <- c_succ(w)
                this->relatives[w].b_sibl.clear();
                for(auto e = this->relatives[w].c_succ.begin(); e!=this->relatives[w].c_succ.end(); ++e) {
                    this->relatives[w].b_sibl.insert(*e);
                }

                // c_succ(w) <- {empty}
                this->relatives[w].c_succ.clear();

                // If a_pred(w) is empty, put w on Q.
                if(this->relatives[w].a_pred.empty()) {
                    queue.push_back(w);
                }

                if (this->halt)
                {
                    std::cout << "process halt" << std::endl;
                    this->dist = oldDist;
                    this->relatives = oldRelatives;
                }
                // Repeat
            }
        }
        this->halt = true;
        return true;
    }
}

void DynamicGraph::visualize() const
{
    this->visualize(false);
}

/**
 * Prints graph in GraphViz
 * https://dreampuf.github.io/GraphvizOnline/
 */
void DynamicGraph::visualize(bool printVirtualEdges) const
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
        if (virtualEdges.count(*ei) && !printVirtualEdges) continue;
        Vertex u,v;
        Edge e = *ei;
        u = source(e, *this);
        v = target(e, *this);
        std::cout << "  " << u << " -- " << v << ";" << std::endl;
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
            if(this->virtualEdges.count(*ei)) {
                std::cout << "~";
            }
            std::cout << *ei << " ";
        }
        std::cout << std::endl;
        std::cout << " - sibl: ";
        for (auto ei = this->relatives[v].b_sibl.begin(); ei != this->relatives[v].b_sibl.end(); ei++) {
            if(this->virtualEdges.count(*ei)) {
                std::cout << "~";
            }
            std::cout << *ei << " ";
        }
        std::cout << std::endl;
        std::cout << " - succ: ";
        for (auto ei = this->relatives[v].c_succ.begin(); ei != this->relatives[v].c_succ.end(); ei++) {
            if(this->virtualEdges.count(*ei)) {
                std::cout << "~";
            }
            std::cout << *ei << " ";
        }
        std::cout << std::endl;
    }
}








