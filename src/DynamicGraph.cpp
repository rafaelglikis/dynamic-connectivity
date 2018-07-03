#include "../incl/DynamicGraph.h"
#include <iostream>
#include <algorithm>
#include <sstream>

void DynamicGraph::visualize()
{
    // Creating components streams
    std::vector<std::ostringstream> subgraphs(this->nextComponent);
    for(unsigned long i = 0; i<this->nextComponent; ++i) {
        subgraphs[i] << "  subgraph cluster" << i << " { ";
    }
    for(unsigned long i = 0; i<this->components.size(); ++i) {
        subgraphs[components[i]-1] << i << "; ";
    }
    for(unsigned long i = 0; i<this->nextComponent; ++i) {
        subgraphs[i] << " }\n";
    }

    // Printing graph style
    std::cout << "graph D {" << std::endl;
    std::cout << "  size=\"5,3\"" << std::endl;
    std::cout << "  ratio=\"fill\"" << std::endl;
    std::cout << "  edge[style=\"bold\"]" << std::endl;
    std::cout << "  node[shape=\"circle\"]" << std::endl;
    // Print subgraphs by vertices
    for(unsigned long i = 0; i<this->nextComponent; ++i) {
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

void DynamicGraph::init() {
    this->components.resize(num_vertices(*this));
    this->dist.resize(num_vertices(*this));
    this->relEdges.resize(num_vertices(*this));
    bfs(0);

//    only for dfs
//    auto searchNode = std::find(this->components.begin(), this->components.end(), 0);
//    while (std::find(this->components.begin(), this->components.end(), 0) != this->components.end()) {
//        this->nextComponent++;
//        dfs(static_cast<const unsigned long &>(std::distance(components.begin(), searchNode)));
//        searchNode = std::find(this->components.begin(), this->components.end(), 0);
//    }
}

void DynamicGraph::bfs(const Vertex& s) {
    std::list<Vertex> queue;
    std::vector<Vertex> visited(num_vertices(*this));
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

    std::list<Edge> virtualEdges;
    while(std::find(visited.begin(), visited.end(), 0) != visited.end() ) {
        // Add Virtual Edges
        for(Vertex i=1; i<dist.size(); ++i ) {
            if(!visited[i]){
                boost::add_edge(0, i, *this);
                virtualEdges.push_back(edge(0,i,*this).first);

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
        // Do BFS again for non Found Vertices
        //break;
    }

    // Delete virtual edges
    for(auto ve = virtualEdges.begin(); ve != virtualEdges.end(); ++ve) {
        boost::remove_edge(*ve, *this);
    }

    // Fill relEdges struct for each vertex
}

void DynamicGraph::dfs(const unsigned long &s)
{
    components[s] = this->nextComponent;

    OutEdgeIterator ei, ei_end;
    for(boost::tie(ei, ei_end) = out_edges(s, *this); ei != ei_end; ++ei) {
        Vertex v = target(*ei, *this);
        if(!components[v]) dfs(v);
    }
}