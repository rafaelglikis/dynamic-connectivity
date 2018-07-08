#ifndef DYNAMIC_CONNECTIVITY_RELATIVES_H
#define DYNAMIC_CONNECTIVITY_RELATIVES_H

#include <set>
#include "UndirectedGraph.h"

/**
 * A Set of relative edges of a vertex in level i
 */
struct Relatives
{
    std::set<Edge> a_pred; // connects vertex with vertices in Li-1
    std::set<Edge> b_sibl; // connects vertex with vertices in Li
    std::set<Edge> c_succ; // connects vertex with vertices in Li+1
};

#endif //DYNAMIC_CONNECTIVITY_RELATIVES_H
