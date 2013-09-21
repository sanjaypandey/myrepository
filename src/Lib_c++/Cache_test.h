//For debug purpuse only..
#define DEBUG

#include "Cache.h"

void cache_test()
{
    Cache<int,char>  cache(10);
    int i,j;
    for(int i=0;i<15;i++)
    {
        j=i;
        if(i>10)
        j=i-10;
        cache.put(j,'a'+i);
        cache.dump(cout);
    }
}
