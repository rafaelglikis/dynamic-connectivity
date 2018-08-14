## An On-Line Edge-Deletion Problem POC
## Abstract
There is given an undirected graph G = (V, E) from which edges are deleted one at a time. Questions like "Are the vertices u and v in the same connected component?" have to be answered "on-line".

This is an implementation of a decremental dynamic algorithm [ES81] which maintains a data structure in which each question is answered in constant time and for which the total time involved in answering q questions and maintaining the data structure is O(q+|V|*|E|), and O(q+|E|*log(|E|)) for acyclic graphs.

## Requirements
* boost graph library
* openmp
* cmake

## Compile
    
    mkdir cmake-build 
    cd cmake-build 
    cmake ..
    cd ..
    cmake --build cmake-build

## Usage

Usage:

    dynamic_connectivity ACTION* [OPTION]*

Example:

    dynamic_connectivity benchmark --random -v 500 -e 1000 -d 50 -q 50

Test:

    --test                       run all tests

Benchmarks:

    --benchmark                  run benchmark specified benchmarks
    --path                       run benchmark with path graph
    --tree                       run benchmark with tree graph
    --random                     run benchmark with random graph
    --path-complete              run benchmark with path of complete subgraphs

Benchmark options:

    -v [ --vertices ] arg (=100) specify number of vertices
    -e [ --edges ] arg (=460)    specify number of edges (only for random graphs)
    -d [ --deletions ] arg (=50) specify number of vertices
    -q [ --queries ] arg (=50)   specify number of vertices

Examples:

    --example                    run an example at src/example.cpp
    
Help:

    --help                       produce help message

Visualization:

    dynamic_connectivity --example
    ./visualize-example.sh
    
## How to use it

* Create graph and initialize it
* Make deletions
* Make queries
* Repeat

### Example

    #include <iostream>
    #include "../incl/graph_types/DynamicGraph.h"
    
    int main()
    {
        // Create graph the bgl way
        DynamicGraph G;
        add_edge(0, 1, G);
        add_edge(0, 2, G);
        add_edge(2, 3, G);
        add_edge(4, 5, G);
        add_edge(4, 6, G);
        add_edge(6, 5, G);
        add_edge(7, 8, G);
    
        // Initialize dynamic Graph
        G.init();
    
        // Delete edge
        G.deleteEdge(7, 8);
        
        if(G.areConnected(7, 8)) {
            std::cout << "Vertices 7 and 8 are connected!" << std::endl;
        }
        else {
            std::cout << "Vertices 7 and 8 not are connected!" << std::endl;
        }
    }
    
## References
* [ES81] S. Even and Y. Shiloach, "An On-Line Edge-Deletion Problem", Journal of the ACM, 28(1):1-4, 1981

    
  
  
