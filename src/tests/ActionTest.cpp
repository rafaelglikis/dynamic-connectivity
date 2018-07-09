#include <iostream>
#include "../../incl/tests/ActionTest.h"
#include "../../incl/actions/Insert.h"
#include "../../incl/actions/Delete.h"

/**
 * Creates a minimal dynamic graph structure to operate on.
 */
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

/**
 * Tests if insertions undid correctly.
 */
void ActionTest::insertTest()
{
    Insert action(rel[0].a_pred, edge(0, 1, G).first);
    action.undo();

    assert(rel[0].a_pred.size()==2);
    assert(rel[0].a_pred.find(edge(0, 2, G).first) != rel[0].a_pred.end());
    assert(rel[0].a_pred.find(edge(0, 3, G).first) != rel[0].a_pred.end());

    std::cout << "[+] insert action test OK!" << std::endl;
}

/**
 * Tests if the deletions undid correctly.
 */
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

/**
 * Run the tests in the correct order.
 */
void ActionTest::run()
{
    insertTest();
    deleteTest();
}