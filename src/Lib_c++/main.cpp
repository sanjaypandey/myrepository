#include <iostream>
#include "list_test.h"
#include "hash_test.h"
#include "Cache_test.h"
#include "malloc_test.h"

using namespace std;

int main()
{
    cout << "Hello world!" << endl;
    /**
    Testing function for the generic list test.
    */
    //test_list_demo();

    /**
    Testing function for the generic hash test.
    */

    /**
    Testing function for the Cache implemenation test.
    */
    //cache_test();

    /**
    Testing function for the malloc implemenation test.
    */
    test_malloc();

    return 0;
}
