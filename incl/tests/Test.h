#ifndef TEST_H
#define TEST_H


/**
 * Abstract Test
 * Subclasses must implement how run function run
 */
class Test
{
public:
    virtual void run() = 0;
};


#endif //TEST_H
