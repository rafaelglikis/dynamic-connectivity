#include <cstdlib>
#include <ctime>
#include "../../incl/utilities/Utilities.h"

big_int_t Utilities::randomRange(big_int_t min, big_int_t max)
{
    static bool first = true;
    if (first) {
        srand(static_cast<unsigned int>(time(nullptr))); //seeding for the first time only!
        first = false;
    }
    return min + rand() % (( max + 1 ) - min);
}
