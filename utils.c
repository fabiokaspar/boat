#include <stdio.h>
#include "utils.h"


void* MallocSafe(size_t bytes)
{
    void* p = malloc(bytes);

    if(p != NULL) return p;

    else
    {
        printf("Overflow Memory!\n\n");
        exit(EXIT_FAILURE);
    }
}

/*
void stop(float delta)
{
    time_t inicio = clock();
    float dt = 0;

    while(dt < delta)
    {
        dt = ((float)clock() - inicio)/CLOCKS_PER_SEC;
    }
}
*/
