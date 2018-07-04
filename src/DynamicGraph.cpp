#include "../incl/DynamicGraph.h"
#include <iostream>
#include <algorithm>
#include <sstream>

void DynamicGraph::init() {
    this->components.resize(num_vertices(*this));
    this->dist.resize(num_vertices(*this));
    this->relEdges.resize(num_vertices(*this));

    bfs(0);
    this->hideVirtualEdges();
    this->updateRelEdges();
}

void DynamicGraph::bfs(const Vertex& s) {
    std::list<Vertex> queue;
    std::vector<Vertex> visited(boost::num_vertices(*this));
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
                this->virtualEdges.push_back(edge(0,i,*this).first);

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

void DynamicGraph::updateRelEdges() {
    for(Vertex v = 0; v < this->dist.size(); v++) {
        OutEdgeIterator ei, ei_end;
        for(boost::tie(ei, ei_end) = out_edges(v, *this); ei != ei_end; ++ei) {
            Vertex u = target(*ei, *this);
            if(this->dist[v] == this->dist[u] + 1 ) {
                this->relEdges[v].a_pred_edges.push_back(*ei);
            }
            else if(this->dist[v] == this->dist[u]) {
                this->relEdges[v].b_sibl_edges.push_back(*ei);
            }
            else if(this->dist[v] == this->dist[u] - 1 ) {
                this->relEdges[v].c_succ_edges.push_back(*ei);
            }
        }
    }
}

void DynamicGraph::hideVirtualEdges() {
    for(auto ve = this->virtualEdges.begin(); ve != virtualEdges.end(); ++ve) {
        boost::remove_edge(*ve, *this);
    }
}

bool DynamicGraph::areConnected(const Vertex &v, const Vertex &u) {
    return this->components[v] == this->components[u];
}

void DynamicGraph::deleteEdge(const Vertex &v, const Vertex &u) {
    bool edgeExist = edge(v,u,*this).second;
    if (edgeExist) {
        Edge e = edge(v,u,*this).first;
        boost::remove_edge(e, *this);

        // Component Break
        if(breaksComponent(v,u)) {

        }
        // Component Not Break

    }
    else {
        throw std::invalid_argument("Edge does not exist");
    }
}

bool DynamicGraph::breaksComponent(const Vertex &v, const Vertex &u)
{
    // Using parallel DFS
    std::list<Vertex> queueV;
    std::list<Vertex> queueU;
    std::vector<bool> visitedV(boost::num_vertices(*this), false);
    std::vector<bool> visitedU(boost::num_vertices(*this), false);

    visitedV[v]=true;
    queueV.push_front(v);

    visitedU[u]=true;
    queueU.push_front(u);

    while(!queueV.empty() && !queueU.empty()) {
        OutEdgeIterator ei, ei_end;

        Vertex vv = queueV.front();
        visitedV[vv]=true;
        queueV.pop_front();
        for(boost::tie(ei, ei_end) = out_edges(vv, *this); ei != ei_end; ++ei) {
            Vertex vvTarget = target(*ei, *this);
            if (u == vvTarget) return false;
            if(!visitedV[vvTarget]){
                queueV.push_front(vvTarget);
            }
        }

        if (queueV.empty()) {
            this->nextComponent ++;
            for (auto visited = visitedV.begin(); visited != visitedV.end(); ++visited){
                if (*visited) this->components[std::distance(visitedV.begin(), visited)] = this->nextComponent;
            }
        }

        Vertex uu = queueU.front();
        visitedU[uu]=true;
        queueU.pop_front();
        for(boost::tie(ei, ei_end) = out_edges(uu, *this); ei != ei_end; ++ei) {
            Vertex uuTarget = target(*ei, *this);
            if (v == uuTarget) return false;
            if(!visitedU[uuTarget]) {
                queueU.push_front(uuTarget);
            }
        }

        if (queueU.empty()) {
            this->nextComponent ++;
            for (auto visited = visitedU.begin(); visited != visitedU.end(); ++visited){
                if (*visited) this->components[std::distance(visitedU.begin(), visited)] = this->nextComponent;
            }
        }
    }
}

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
    std::cout << "graph D {" << std::endl;
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
        std::cout << "    " << u << " -- " << v << ";" << std::endl;
    }

    std::cout << "}" << std::endl;
}

void DynamicGraph::printInfo() {
    for (Vertex v = 0; v < this->dist.size(); v++) {
        std::cout << "Vertex " << v << " (component " << this->components[v] << ")" << std::endl;
        std::cout << " - pred: ";
        for (auto ei = this->relEdges[v].a_pred_edges.begin(); ei != this->relEdges[v].a_pred_edges.end(); ei++) {
            std::cout << *ei << " ";
        }
        std::cout << std::endl;
        std::cout << " - sibl: ";
        for (auto ei = this->relEdges[v].b_sibl_edges.begin(); ei != this->relEdges[v].b_sibl_edges.end(); ei++) {
            std::cout << *ei << " ";
        }
        std::cout << std::endl;
        std::cout << " - succ: ";
        for (auto ei = this->relEdges[v].c_succ_edges.begin(); ei != this->relEdges[v].c_succ_edges.end(); ei++) {
            std::cout << *ei << " ";
        }
        std::cout << std::endl;
    }
}








