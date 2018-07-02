#include "../incl/DynamicGraph.h"
#include <iostream>
#include <algorithm>

void DynamicGraph::visualize()
{

    std::cout << "graph D {" << std::endl;
    std::cout << "  size=\"5,3\"" << std::endl;
    std::cout << "  ratio=\"fill\"" << std::endl;
    std::cout << "  edge[style=\"bold\"]" << std::endl;
    std::cout << "  node[shape=\"circle\"]" << std::endl;
    std::cout << "  subgraph cluster1 {" << std::endl;

    EdgeIterator ei, ei_end;
    for (tie(ei, ei_end) = edges(*this); ei != ei_end; ++ei) {
        Vertex u,v;
        Edge e = *ei;
        u = source(e, *this);
        v = target(e, *this);
        std::cout << "    " << u << " -- " << v << ";" << std::endl;
    }

    VertexIterator vi, vi_end;
    for (vi = vertices(*this).first; vi != vertices(*this).second; ++vi) {
        std::cout << "    " << *vi << ";" << std::endl;
    }

    std::cout << "  }" << std::endl;
    std::cout << "}" << std::endl;
}

void DynamicGraph::init() {
    this->components.resize(num_vertices(*this));

    auto searchNode = std::find(this->components.begin(), this->components.end(), 0);
    while (std::find(this->components.begin(), this->components.end(), 0) != this->components.end())
    {
        this->nextComponent++;
        dfsUpdateComponent(std::distance(components.begin(), searchNode));
        searchNode = std::find(this->components.begin(), this->components.end(), 0);
    }

    for(auto i=0; i < this->components.size(); i++) {
        std::cout << "vertex " << i << " component " << components[i] << std::endl;
    }
}

void DynamicGraph::dfsUpdateComponent(const long& s)
{
    components[s] = this->nextComponent;
    Vertex v;
    OutEdgeIterator ei, ei_end;
    for(boost::tie(ei, ei_end) = out_edges(s, *this); ei != ei_end; ++ei) {
        v = target(*ei, *this);
        if(!components[v]) dfsUpdateComponent(v);
    }
}