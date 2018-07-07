#include "../incl/graph_types/DynamicGraph.h"
#include "../incl/tests/DynamicGraphTest.h"
#include "../incl/examples/examples.h"
#include "../incl/tests/ActionTest.h"

#include <iostream>

int main()
{
    ActionTest actionTest;
    DynamicGraphTest graphTest;
    actionTest.run();
    graphTest.run();

    //example();
    return 0;
}

