#include "../../incl/graph_types/DynamicGraph.h"
#include "../../incl/actions/Delete.h"
#include "../../incl/actions/Insert.h"
#include <iostream>
#include <algorithm>
#include <sstream>
#include <omp.h>

DynamicGraph::DynamicGraph() : UndirectedGraph()
{
    this->nextComponent = 0;
    this->halt = false;
    this->isInitialized = false;
}

/**
 * Initialization
 * Creation of the first BFS structure starting always from vertex 0.
 * init components level and relatives vectors.
 * init relatives struct for each note.
 */
void DynamicGraph::init()
{
    this->components.resize(num_vertices(*this));
    this->level.resize(num_vertices(*this));
    this->relatives.resize(num_vertices(*this));

    buildBFSStructure(0);
    this->updateRelatives();
    this->isInitialized = true;
}

/**
 * Creates the first bfs structure starting from vertex s.
 * @param s (start vertex)
 */
void DynamicGraph::buildBFSStructure(const Vertex &s)
{
    std::vector<bool> visited(boost::num_vertices(*this));
    this->nextComponent++;
    this->bfs(s, visited, 0);

    // While not visited notes exist
    while(std::find(visited.begin(), visited.end(), 0) != visited.end() ) {
        // For each note not visited yet
        for(Vertex i=1; i<level.size(); ++i ) {
            // Add a virtual edge
            if(!visited[i]){
                boost::add_edge(0, i, *this);
                this->virtualEdges.insert(edge(0,i,*this).first);
                this->nextComponent++;

                // And continue scanning from here
                this->bfs(i, visited, 1);
            }
        }
    }
}

/**
 * Runs breath first search algorithm on DynamicGraph
 * starting from note s, and level startingLevel
 * @param s (start vertex)
 * @param visited (vector with vertex visited status)
 * @param startingLevel (level to start bfs)
 */
void DynamicGraph::bfs(const Vertex &s, std::vector<bool> &visited, const int startingLevel)
{
    std::list<Vertex> queue;
    this->level[s] = startingLevel;
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
                level[u] = this->level[v]+1;
                this->components[u] = this->nextComponent;
                visited[u]=true;
                queue.push_back(u);
            }
        }
    }
}

/**
 * Updates relatives struct for each vertex.
 */
void DynamicGraph::updateRelatives()
{
    for(Vertex v = 0; v < this->level.size(); v++) {
        OutEdgeIterator ei, ei_end;
        for(boost::tie(ei, ei_end) = out_edges(v, *this); ei != ei_end; ++ei) {
            Vertex u = target(*ei, *this);
            if(this->level[v] == this->level[u] + 1 ) {
                this->relatives[v].a_pred.insert(*ei);
            }
            else if(this->level[v] == this->level[u]) {
                this->relatives[v].b_sibl.insert(*ei);
            }
            else if(this->level[v] == this->level[u] - 1 ) {
                this->relatives[v].c_succ.insert(*ei);
            }
        }
    }
}

/**
 * Hides all virtual edges in order to use Dynamic graph for other purposes.
 */
void DynamicGraph::hideVirtualEdges()
{
    for(auto ei = this->virtualEdges.begin(); ei != virtualEdges.end(); ++ei) {
        Edge e = *ei;
        boost::remove_edge(*ei, *this);
    }
}

/**
 * Brings back virtual edges in order to make a deletion.
 */
void DynamicGraph::showVirtualEdges()
{
    for(auto ei = this->virtualEdges.begin(); ei != virtualEdges.end(); ++ei) {
        Vertex s = source(*ei, *this);
        Vertex t = target(*ei, *this);
        boost::add_edge(s, t, *this);
    }
}

/**
 * Answers the question:
 * Are vertices v and u in the same connected component?
 * in constant time.
 * Initialization required.
 * @param v (vertex)
 * @param u (vertex)
 * @return true if v and u in the same connected component.
 */
bool DynamicGraph::areConnected(const Vertex &v, const Vertex &u)
{
    if(!this->isInitialized) {
        throw std::runtime_error("Dynamic Graph not initialized!");
    }
    return this->components[v] == this->components[u];
}

/**
 * Deletes the edge connecting vertices v and u.
 * Initialization required.
 * @param v (vertex)
 * @param u (vertex)
 */
void DynamicGraph::deleteEdge(const Vertex &v, const Vertex &u)
{
    if(!this->isInitialized) {
        throw std::runtime_error("Dynamic Graph not initialized!");
    }

    bool edgeExist = edge(v,u,*this).second;
    if (!edgeExist) {
        throw std::invalid_argument("Edge does not exist");
    }
    Edge e = edge(v,u,*this).first;
    boost::remove_edge(e, *this);
    this->handleDeletion(e);
}

/**
 * Handles the deletion.
 * Running to threads.
 *  - Thread A: detects if the deletion breaks a component. O(|E|log(|E|))
 *  - Thread B: detects if the deletion does not break a component. O(|E|.|V|)
 * When one of the previews threads finish positively both threads stop.
 * If the component break create a new component with the smaller subgraph.
 * Replace the deleted edge with a virtual one.
 *
 * @param e (edge)
 */
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

/**
 * Thread A
 * Check if the component breaks after the deletion of the
 * edge connecting vertices v and u.
 * If the component break create a new component with the smaller subgraph
 * and stop thread B.
 * @param v (vertex)
 * @param u (vertex)
 * @return true if the components break
 */
bool DynamicGraph::checkComponentBreak(const Vertex &v, const Vertex &u)
{
    std::list<Vertex> queueV;
    std::list<Vertex> queueU;
    std::vector<bool> visitedV(boost::num_vertices(*this), false);
    std::vector<bool> visitedU(boost::num_vertices(*this), false);
    std::list<Vertex> visitedListV;
    std::list<Vertex> visitedListU;

    // Using bidirectional BFS
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

        if(!this->bdbfsStep(queueV, visitedV, visitedU, visitedListV)
           || this->halt) {
            return false;
        }

        if (queueV.empty()) {
            this->halt = true;
            updateVisitedComponents(visitedListV);
            break;
        }

        if(!this->bdbfsStep(queueU, visitedU, visitedV, visitedListU)
           || this->halt) {
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

/**
 * A step of bidirectional breath first search algorithm used in thread A.
 * @param queue bdbfs Q
 * @param visitedV vertices vector - true if  visited from vertex v
 * @param visitedU vertices vector - true if visited from vertex u
 * @param visitedList list of vertices visited from vertex v
 * @return false if reaches a vertex known by the oposite vertex.
 */
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

/**
 * Updates component label for each vertex in visited list
 * @param visited (vertices to change component)
 */
void DynamicGraph::updateVisitedComponents(const std::list<Vertex>& visited)
{
    this->nextComponent++;
    for (auto v = visited.begin(); v != visited.end(); ++v){
        this->components[*v] = this->nextComponent;
    }
}

/**
 * Thread B
 * Checks if the deletion of the edge e does not break the component.
 * If does not, then stop thread A.
 * Changes the levels relatives structure for some vertices
 * in order to keep the BFS structure.
 * Cases:
 * Case 1: v and u are on the same level
 *  - The edge is simply deleted from b_sibl(u) and b_sibl(v),
 *  - halt return true
 * Case 2: v and u are on different levels
 *  -  We remove e from, c_succ(u) and a_pred(v).
 * Case 2.1: a_pred(v) is not empty
 *  - halt and return true
 * Case 2.2 a_pred(v) is empty
 * - drop level of v.  its drop may cause a whole avalanche (see below).
 * - if the avalanche reaches the end halt and return true.
 * - if thread A finishes first then the DynamicGraph BFS structure
 *   returns to the initial state (undo all changes one by one)
 * @param v (vertex)
 * @param u (vertex)
 * @param e (edge)
 * @return true if the deletion does not break the component.
 */
bool DynamicGraph::checkComponentNotBreak(Vertex v, Vertex u, Edge e)
{
    // Case 1: v and u are on the same level
    if (this->level[v]==this->level[u]) {
        this->halt = true;
        this->relatives[v].b_sibl.erase(e);
        this->relatives[u].b_sibl.erase(e);
        return true;
    }

    // Case 2: v and u are on different levels
    if (this->level[v]!=this->level[u]) {
        std::list<Action*> actions; // changes stack
        std::list<Vertex> incVertices; // vertices dropped stack (each vertex multible times)
        // For Generality u in Li-1 and v Li
        if (this->level[v] < this->level[u]) {
            Vertex tmp = v;
            v = u;
            u = tmp;
        }

        actions.push_front(new Delete(this->relatives[u].c_succ, e));
        this->relatives[u].c_succ.erase(e);
        actions.push_front(new Delete(this->relatives[v].a_pred, e));
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
                dropLevel(w, queue, actions, incVertices);

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

/**
 * Thread B
 * Case 2: v and u are on different levels
 * Case 2.2 a_pred(v) is empty
 * Step of the avalanche.
 * Steps described below.
 * @param w (vertex)
 * @param queue (vertices has to drop level)
 * @param actions (the action stack for undo)
 * @param incVertices (vector of vertices that droped level for undo)
 */
void DynamicGraph::dropLevel(Vertex w, std::list<Vertex> &queue,
                             std::list<Action *> &actions, std::list<Vertex> &incVertices)
{
    // Remove w from its level (say, Lj), and put it in the next level (Lj+1)
    incVertices.push_front(w);
    level[w]++;

    // For each edge e (w---w') in b_sibl(w)
    for(auto ei = relatives[w].b_sibl.begin(); ei != relatives[w].b_sibl.end(); ++ei) {
        Vertex ww = (w==target(*ei, *this)) ? source(*ei, *this) : target(*ei, *this);
        // remove e from b_sibl(w')
        actions.push_front(new Delete(relatives[ww].b_sibl, *ei));
        relatives[ww].b_sibl.erase(*ei);
        // and put it in c_succ(w')
        actions.push_front(new Insert(relatives[ww].c_succ, *ei));
        relatives[ww].c_succ.insert(*ei);
    }

    // a_pred(w) <- b_pred(w)
    for(auto ei = relatives[w].a_pred.begin(); ei != relatives[w].a_pred.end(); ++ei) {
        actions.push_front(new Delete(relatives[w].a_pred, *ei));
    }
    relatives[w].a_pred.clear();
    for(auto ei = relatives[w].b_sibl.begin(); ei != relatives[w].b_sibl.end(); ++ei) {
        actions.push_front(new Insert(relatives[w].a_pred, *ei));
        relatives[w].a_pred.insert(*ei);
    }

    // For each edge e (w---w') in c_succ(w)
    for(auto ei = relatives[w].c_succ.begin(); ei != relatives[w].c_succ.end(); ++ei) {
        Vertex ww = (w==target(*ei, *this)) ? source(*ei, *this) : target(*ei, *this);
        // remove e from a_pred(w')
        actions.push_front(new Delete(relatives[ww].a_pred, *ei));
        relatives[ww].a_pred.erase(*ei);
        // and put it in b_sibl(w');
        actions.push_front(new Insert(relatives[ww].b_sibl, *ei));
        relatives[ww].b_sibl.insert(*ei);
        // if the new a_pred(w') is empty, put w' on Q.
        if(relatives[ww].a_pred.empty()) {
            queue.push_back(ww);
        }
    }

    // b_sibl(w) <- c_succ(w)
    for(auto ei = relatives[w].b_sibl.begin(); ei != relatives[w].b_sibl.end(); ++ei) {
        actions.push_front(new Delete(relatives[w].b_sibl, *ei));
    }
    relatives[w].b_sibl.clear();
    for(auto ei = relatives[w].c_succ.begin(); ei != relatives[w].c_succ.end(); ++ei) {
        actions.push_front(new Insert(relatives[w].b_sibl, *ei));
        relatives[w].b_sibl.insert(*ei);
    }

    // c_succ(w) <- {empty}
    for(auto ei = relatives[w].c_succ.begin(); ei != relatives[w].c_succ.end(); ++ei) {
        actions.push_front(new Delete(relatives[w].c_succ, *ei));
    }
    relatives[w].c_succ.clear();

    // If a_pred(w) is empty, put w on Q.
    if(relatives[w].a_pred.empty()) {
        queue.push_back(w);
    }
}

/**
 * Undo all actions given as parameters
 * @param actions (insert and delete actions)
 * @param incVertices (vertices dropped)
 */
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
        this->level[v]--;
    }
}

/**
 * Prints graph in GraphViz format
 * https://dreampuf.github.io/GraphvizOnline/
 */
void DynamicGraph::visualize() const
{
    // Creating components streams
    std::vector<std::string> subgraphs(this->nextComponent);
    for(unsigned long i = 0; i<this->nextComponent; ++i) {
        subgraphs[i].append("  subgraph cluster");
        subgraphs[i].append(std::to_string((long long int)i));
        subgraphs[i].append(" { label = \"Component ");
        subgraphs[i].append(std::to_string((long long int)i+1)); 
        subgraphs[i].append("\" ");
    }
    for(unsigned long i = 0; i<this->components.size(); ++i) {
        subgraphs[components[i]-1].append(std::to_string((long long int)i));
        subgraphs[components[i]-1].append("; ");
    }
    for(unsigned long i = 0; i<this->nextComponent; ++i) {
        subgraphs[i].append(" }\n");
    }

    // Printing graph style
    std::cout << "graph G {" << std::endl;
    std::cout << "  size=\"5,3\"" << std::endl;
    std::cout << "  ratio=\"fill\"" << std::endl;
    std::cout << "  edge[style=\"bold\"]" << std::endl;
    std::cout << "  node[shape=\"circle\"]" << std::endl;
    // Print subgraphs by vertices
    for(unsigned long i = 0; i<this->nextComponent; ++i) {
        std::cout  << subgraphs[i];
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
    for (Vertex v = 0; v < this->level.size(); v++) {
        std::cout << "Vertex " << v  << std::endl;
        std::cout << " - component " << this->components[v] << std::endl;
        std::cout << " - level " << this->level[v] << std::endl;
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

/**
 * Returns the level of the given vertex.
 * @param v vertex
 * @return level
 */
int DynamicGraph::getLevel(Vertex v) const
{
    return this->level[v];
}

/**
 * Returns the component of the given vertex.
 * @param v vertex
 * @return level
 */
unsigned long DynamicGraph::getComponent(const Vertex v) const
{
    return this->components[v];
}

/**
 * Returns all virtual edges
 * @return virtualEdgesList
 */
std::list<Edge> DynamicGraph::getVirtualEdges() const
{
    std::list<Edge> virtualEdgesList;
    for(auto ei = this->virtualEdges.begin(); ei != this->virtualEdges.end(); ++ei) {
        virtualEdgesList.push_back(*ei);
    }
    return virtualEdgesList;
}














