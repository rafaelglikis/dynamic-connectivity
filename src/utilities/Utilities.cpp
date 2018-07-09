#include <cstdlib>
#include <ctime>
#include "../../incl/utilities/Utilities.h"
#include "../../incl/graph_types/UndirectedGraph.h"

/**
 * Generates a random Vertex in range.
 * Seed initialized once.
 * @param min (minimum Vertex to return)
 * @param max (maximum Vertex to return)
 * @return number
 */
Vertex Utilities::randomRange(Vertex min, Vertex max)
{
    static bool first = true;
    if (first) {
        srand(static_cast<unsigned int>(time(nullptr))); //seeding for the first time only!
        first = false;
    }
    return min + rand() % (( max + 1 ) - min);
}
