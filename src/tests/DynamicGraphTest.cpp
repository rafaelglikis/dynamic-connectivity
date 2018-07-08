#include <iostream>
#include "../../incl/tests/DynamicGraphTest.h"


DynamicGraphTest::DynamicGraphTest()
{
    add_edge(0, 1, this->G);
    add_edge(0, 2, this->G);
    add_edge(2, 3, this->G);

    add_edge(4, 5, this->G);
    add_edge(4, 6, this->G);
    add_edge(6, 5, this->G);

    add_edge(7, 8, this->G);

    add_edge(9, 10, this->G);
    add_edge(9, 11, this->G);
    add_edge(10, 11, this->G);
    add_edge(13, 11, this->G);
    add_edge(15, 11, this->G);
    add_edge(10, 12, this->G);
    add_edge(12, 13, this->G);
    add_edge(14, 13, this->G);
    add_edge(14, 15, this->G);
    add_edge(13, 15, this->G);

    add_edge(16, 17, this->G);
    add_edge(16, 19, this->G);
    add_edge(18, 17, this->G);
    add_edge(18, 19, this->G);

}

void DynamicGraphTest::testInit()
{
    this->G.init();

    // test levels
    assert(G.getLevel(0)==0);
    assert(G.getLevel(1)==1);
    assert(G.getLevel(2)==1);
    assert(G.getLevel(3)==2);
    assert(G.getLevel(4)==1);
    assert(G.getLevel(5)==2);
    assert(G.getLevel(6)==2);
    assert(G.getLevel(7)==1);
    assert(G.getLevel(8)==2);
    assert(G.getLevel(9)==1);
    assert(G.getLevel(10)==2);
    assert(G.getLevel(11)==2);
    assert(G.getLevel(12)==3);
    assert(G.getLevel(13)==3);
    assert(G.getLevel(14)==4);
    assert(G.getLevel(15)==3);
    assert(G.getLevel(16)==1);
    assert(G.getLevel(17)==2);
    assert(G.getLevel(18)==3);
    assert(G.getLevel(19)==2);

    // test components
    assert(G.getComponent(0)==1);
    assert(G.getComponent(1)==1);
    assert(G.getComponent(2)==1);
    assert(G.getComponent(3)==1);
    assert(G.getComponent(4)==2);
    assert(G.getComponent(5)==2);
    assert(G.getComponent(6)==2);
    assert(G.getComponent(7)==3);
    assert(G.getComponent(8)==3);
    assert(G.getComponent(9)==4);
    assert(G.getComponent(10)==4);
    assert(G.getComponent(11)==4);
    assert(G.getComponent(12)==4);
    assert(G.getComponent(13)==4);
    assert(G.getComponent(14)==4);
    assert(G.getComponent(15)==4);
    assert(G.getComponent(16)==5);
    assert(G.getComponent(17)==5);
    assert(G.getComponent(18)==5);
    assert(G.getComponent(19)==5);

    // test virtual edges
    std::list<Edge> expectedVirtualEdges = {
            edge(0, 4, G).first,
            edge(0, 7, G).first,
            edge(0, 9, G).first,
            edge(0, 16, G).first
    };
    testVirtualEdges(expectedVirtualEdges);


    std::cout << "[+] test init OK!" << std::endl;
}

void DynamicGraphTest::testNonBreakDelete()
{
    assert(G.areConnected(16, 17));

    G.deleteEdge(16, 17);
    assert(G.areConnected(16, 17));

    // test levels
    assert(G.getLevel(0)==0);
    assert(G.getLevel(1)==1);
    assert(G.getLevel(2)==1);
    assert(G.getLevel(3)==2);
    assert(G.getLevel(4)==1);
    assert(G.getLevel(5)==2);
    assert(G.getLevel(6)==2);
    assert(G.getLevel(7)==1);
    assert(G.getLevel(8)==2);
    assert(G.getLevel(9)==1);
    assert(G.getLevel(10)==2);
    assert(G.getLevel(11)==2);
    assert(G.getLevel(12)==3);
    assert(G.getLevel(13)==3);
    assert(G.getLevel(14)==4);
    assert(G.getLevel(15)==3);
    assert(G.getLevel(16)==1);
    assert(G.getLevel(17)==4);
    assert(G.getLevel(18)==3);
    assert(G.getLevel(19)==2);

    // test components nothing changes
    assert(G.getComponent(0)==1);
    assert(G.getComponent(1)==1);
    assert(G.getComponent(2)==1);
    assert(G.getComponent(3)==1);
    assert(G.getComponent(4)==2);
    assert(G.getComponent(5)==2);
    assert(G.getComponent(6)==2);
    assert(G.getComponent(9)==4);
    assert(G.getComponent(10)==4);
    assert(G.getComponent(11)==4);
    assert(G.getComponent(12)==4);
    assert(G.getComponent(13)==4);
    assert(G.getComponent(14)==4);
    assert(G.getComponent(15)==4);
    assert(G.getComponent(16)==5);
    assert(G.getComponent(17)==5);
    assert(G.getComponent(18)==5);
    assert(G.getComponent(19)==5);

    // test virtual edges
    std::list<Edge> expectedVirtualEdges = {
            edge(0, 4, G).first,
            edge(0, 7, G).first,
            edge(0, 9, G).first,
            edge(0, 16, G).first,
    };
    testVirtualEdges(expectedVirtualEdges);


    std::cout << "[+] test non break delete OK!" << std::endl;
}

void DynamicGraphTest::testBreakDelete()
{
    // are they connected
    assert(G.areConnected(7,8));

    // now?
    G.deleteEdge(7, 8);
    assert(!G.areConnected(7,8));

    // test levels
    assert(G.getLevel(0)==0);
    assert(G.getLevel(1)==1);
    assert(G.getLevel(2)==1);
    assert(G.getLevel(3)==2);
    assert(G.getLevel(4)==1);
    assert(G.getLevel(5)==2);
    assert(G.getLevel(6)==2);
    assert(G.getLevel(7)==1);
    assert(G.getLevel(8)==2);
    assert(G.getLevel(9)==1);
    assert(G.getLevel(10)==2);
    assert(G.getLevel(11)==2);
    assert(G.getLevel(12)==3);
    assert(G.getLevel(13)==3);
    assert(G.getLevel(14)==4);
    assert(G.getLevel(15)==3);
    assert(G.getLevel(16)==1);
    assert(G.getLevel(17)==4);
    assert(G.getLevel(18)==3);
    assert(G.getLevel(19)==2);

    // test components

    assert(G.getComponent(0)==1);
    assert(G.getComponent(1)==1);
    assert(G.getComponent(2)==1);
    assert(G.getComponent(3)==1);
    assert(G.getComponent(4)==2);
    assert(G.getComponent(5)==2);
    assert(G.getComponent(6)==2);
    assert(G.getComponent(7)==6);
    assert(G.getComponent(8)==3);
    assert(G.getComponent(9)==4);
    assert(G.getComponent(10)==4);
    assert(G.getComponent(11)==4);
    assert(G.getComponent(12)==4);
    assert(G.getComponent(13)==4);
    assert(G.getComponent(14)==4);
    assert(G.getComponent(15)==4);
    assert(G.getComponent(16)==5);
    assert(G.getComponent(17)==5);
    assert(G.getComponent(18)==5);
    assert(G.getComponent(19)==5);

    // test virtual edges
    std::list<Edge> expectedVirtualEdges = {
            edge(0, 4, G).first,
            edge(0, 7, G).first,
            edge(0, 9, G).first,
            edge(0, 16, G).first,
            edge(7, 8, G).first
    };
    testVirtualEdges(expectedVirtualEdges);

    std::cout << "[+] test break delete OK!" << std::endl;
}

void DynamicGraphTest::testBig()
{
    G.deleteEdge(4, 5);
    G.deleteEdge(4, 6);

    G.deleteEdge(13, 15);
    G.deleteEdge(9, 10);

    G.deleteEdge(16, 19);

    G.deleteEdge(12, 10);
    G.deleteEdge(13, 12);

    G.deleteEdge(0, 1);
    G.deleteEdge(0, 2);
    G.deleteEdge(2, 3);

    G.deleteEdge(18, 17);
    G.deleteEdge(18, 19);

    G.deleteEdge(15, 11);
    G.deleteEdge(9, 11);
    G.deleteEdge(10, 11);
    G.deleteEdge(13, 11);

    G.deleteEdge(6, 5);

    // test levels
    assert(G.getLevel(0)==0);
    assert(G.getLevel(2)==1);
    assert(G.getLevel(3)==2);
    assert(G.getLevel(4)==1);
    assert(G.getLevel(5)==3);
    assert(G.getLevel(6)==2);
    assert(G.getLevel(7)==1);
    assert(G.getLevel(8)==2);
    assert(G.getLevel(9)==1);
    assert(G.getLevel(10)==3);
    assert(G.getLevel(11)==2);
    assert(G.getLevel(12)==4);
    assert(G.getLevel(13)==3);
    assert(G.getLevel(14)==4);
    assert(G.getLevel(15)==5);
    assert(G.getLevel(16)==1);
    assert(G.getLevel(17)==4);
    assert(G.getLevel(18)==3);
    assert(G.getLevel(19)==2);

    // test components
    assert(G.getComponent(0)==11);
    assert(G.getComponent(1)==10);
    assert(G.getComponent(2)==12);
    assert(G.getComponent(3)==1);
    assert(G.getComponent(4)==7);
    assert(G.getComponent(5)==2);
    assert(G.getComponent(6)==18);
    assert(G.getComponent(7)==6);
    assert(G.getComponent(8)==3);
    assert(G.getComponent(9)==15);
    assert(G.getComponent(10)==16);
    assert(G.getComponent(11)==17);
    assert(G.getComponent(12)==9);
    assert(G.getComponent(13)==4);
    assert(G.getComponent(14)==4);
    assert(G.getComponent(15)==4);
    assert(G.getComponent(16)==8);
    assert(G.getComponent(17)==13);
    assert(G.getComponent(18)==14);
    assert(G.getComponent(19)==5);

    // test virtual edges
    std::list<Edge> expectedVirtualEdges = {
            edge(0, 4, G).first,
            edge(0, 7, G).first,
            edge(0, 9, G).first,
            edge(0, 16, G).first,
            edge(4, 6, G).first,
            edge(16, 19, G).first,
            edge(13, 12, G).first,
            edge(7, 8, G).first,
            edge(0, 1, G).first,
            edge(0, 2, G).first,
            edge(2, 3, G).first,
            edge(18, 17, G).first,
            edge(18, 19, G).first,
            edge(9, 11, G).first,
            edge(10, 11, G).first,
            edge(13, 11, G).first,
            edge(6, 5, G).first
    };
    testVirtualEdges(expectedVirtualEdges);

    std::cout << "[+] big test OK!" << std::endl;
}

void DynamicGraphTest::run()
{
    this->testInit();
    this->testNonBreakDelete();
    this->testBreakDelete();
    this->testBig();
}

void DynamicGraphTest::testVirtualEdges(std::list<Edge> &expectedVirtualEdges)
{
    // test virtual edges
    std::list<Edge> actualVirtualEdges = G.getVirtualEdges();

    assert(expectedVirtualEdges.size()==actualVirtualEdges.size());
    while(!expectedVirtualEdges.empty() && !actualVirtualEdges.empty()) {
        assert(std::count(actualVirtualEdges.begin(), actualVirtualEdges.end(), expectedVirtualEdges.front()));
        expectedVirtualEdges.pop_front();
    }
}