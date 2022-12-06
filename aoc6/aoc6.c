#include <stdio.h>
#include <stdbool.h>

bool all_unique(char *b, char *e)
{
    for(;b != e; ++b)
        for(char *bb = b + 1; bb != e; ++bb)
            if ( *bb == *b)
                return false;
    return true;  
}

int ptN(char *in, int N)
{
    char *it0 = in, *itN = in + N;
    for(; *itN; ++it0, ++itN)
        if(all_unique(it0, itN))
            break;
    return itN - in;
}

int main()
{
    char buf[8192];
    fgets(buf, sizeof buf, stdin);
 
    printf("pt1 = %d\npt2 = %d\n", ptN(buf, 4), ptN(buf,14) );
}