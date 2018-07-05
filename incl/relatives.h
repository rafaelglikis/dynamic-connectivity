#ifndef DYNAMIC_CONNECTIVITY_RELATIVES_H
#define DYNAMIC_CONNECTIVITY_RELATIVES_H

struct Relatives {
    std::set<Edge> a_pred;
    std::set<Edge> b_sibl;
    std::set<Edge> c_succ;
};

#endif //DYNAMIC_CONNECTIVITY_RELATIVES_H
