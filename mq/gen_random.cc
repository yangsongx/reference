#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/**
 * This is simulate Python's random api like:
 *
 * random.randint(1, 10)
 *
 */
int random_int(int from, int to)
{
    int ret = 0;

    /*Below calling MUST called only once
      probably only in starting, not in function
     */

    //srand(time(NULL));

    ret = (rand()%(to -from))+from;

    return ret;
}

