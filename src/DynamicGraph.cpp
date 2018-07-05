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

    bfs(0);
    this->updateRelatives();
}

void DynamicGraph::bfs(const Vertex& s)
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
    for(auto ve = this->virtualEdges.begin(); ve != virtualEdges.end(); ++ve) {
        boost::remove_edge(*ve, *this);
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
        this->handleDeletion(v, u);
    }
    else {
        throw std::invalid_argument("Edge does not exist");
    }
}

void DynamicGraph::handleDeletion(const Vertex &v, const Vertex &u)
{
    this->halt = false;
    #pragma omp parallel sections
    {
        #pragma omp section
        {
            // Component Not Break
            //checkComponentNotBreak(v, u);
        }

        #pragma omp section
        {
            // Component Breaks
            checkComponentBreak(v, u);
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

void DynamicGraph::updateVisitedComponents(std::list<Vertex>& visited)
{
    this->nextComponent ++;
    for (auto v = visited.begin(); v != visited.end(); ++v){
        this->components[*v] = this->nextComponent;
    }
}

bool DynamicGraph::checkComponentNotBreak(Vertex v, Vertex u)
{
    // Case 1: v and u are on the same level
    if (this->dist[v]==this->dist[u]) {
        std::cout << "Case 1: " << v << " and " << u << " are on the same level" << std::endl;
        this->halt = true;
        this->relatives[v].b_sibl.erase(edge(v, u, *this).first);
        this->relatives[u].b_sibl.erase(edge(v, u, *this).first);
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
        this->relatives[u].c_succ.erase(edge(v, u, *this).first); // *********
        this->relatives[v].a_pred.erase(edge(v, u, *this).first); // *********

        // Case 2.1: a_pred(v) is not empty
        if(! this->relatives[v].a_pred.empty()) {
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

                // For each edge e (w---w') in b_sibl(w), remove e from b_sibl(w') and put it in c_succ(w')

                // a_pred(w) <- b_pred(w)

                // For each edge e (w---w') in c_succ(w),
                // remove e from a(w') and put it in b_sibl(w');
                // if the new a_pred(w') is empty, put w' on Q.

                // b_sibl(w) <- c_succ(w)
                // c_succ(w) <- {empty}

                // If a_pred(w) is empty, put w on Q.

                // Repeat
            }
        }
        this->halt = true;
        return true;
    }
}

/**
 * Prints graph in GraphViz
 * https://dreampuf.github.io/GraphvizOnline/
 */
void DynamicGraph::visualize()
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
        if (virtualEdges.count(*ei)) continue;
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
void DynamicGraph::printInfo()
{
    for (Vertex v = 0; v < this->dist.size(); v++) {
        std::cout << "Vertex " << v  << std::endl;
        std::cout << " - component " << this->components[v] << std::endl;
        std::cout << " - dist " << this->dist[v] << std::endl;
        std::cout << " - pred: ";
        for (auto ei = this->relatives[v].a_pred.begin(); ei != this->relatives[v].a_pred.end(); ei++) {
            std::cout << *ei << " ";
        }
        std::cout << std::endl;
        std::cout << " - sibl: ";
        for (auto ei = this->relatives[v].b_sibl.begin(); ei != this->relatives[v].b_sibl.end(); ei++) {
            std::cout << *ei << " ";
        }
        std::cout << std::endl;
        std::cout << " - succ: ";
        for (auto ei = this->relatives[v].c_succ.begin(); ei != this->relatives[v].c_succ.end(); ei++) {
            std::cout << *ei << " ";
        }
        std::cout << std::endl;
    }
}





