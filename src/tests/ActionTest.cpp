#include <iostream>
#include "../../incl/tests/ActionTest.h"
#include "../../incl/actions/Insert.h"
#include "../../incl/actions/Delete.h"

ActionTest::ActionTest()
{
    add_edge(0, 1, G);
    add_edge(0, 2, G);
    add_edge(0, 3, G);
    add_edge(0, 4, G);

    Relatives relatives;
    this->rel.push_back(relatives);

    this->rel[0].a_pred.insert(edge(0, 1, G).first);
    this->rel[0].a_pred.insert(edge(0, 2, G).first);
    this->rel[0].a_pred.insert(edge(0, 3, G).first);
}

void ActionTest::insertTest()
{
    Insert action(rel[0].a_pred, edge(0, 1, G).first);
    action.undo();

    assert(rel[0].a_pred.size()==2);
    assert(rel[0].a_pred.find(edge(0, 2, G).first) != rel[0].a_pred.end());
    assert(rel[0].a_pred.find(edge(0, 3, G).first) != rel[0].a_pred.end());

    std::cout << "[+] insert action test OK!" << std::endl;
}

void ActionTest::deleteTest()
{
    Delete action(rel[0].a_pred, edge(0, 1, G).first);
    action.undo();

    assert(rel[0].a_pred.size()==3);
    assert(rel[0].a_pred.find(edge(0, 1, G).first) != rel[0].a_pred.end());
    assert(rel[0].a_pred.find(edge(0, 2, G).first) != rel[0].a_pred.end());
    assert(rel[0].a_pred.find(edge(0, 3, G).first) != rel[0].a_pred.end());

    std::cout << "[+] delete action test OK!" << std::endl;
}

void ActionTest::run()
{
    insertTest();
    deleteTest();
}