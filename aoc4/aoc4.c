#include <stdio.h>

int main()
{
    int lf, lt, rf, rt;
    int p1 = 0, p2 = 0;
    while( scanf("%d-%d,%d-%d\n", &lf, &lt, &rf, &rt) == 4)
    {
        if(lf <= rf && lt >= rt || rf <= lf && rt >= lt)
            ++p1;
        if( !(lt < rf || rt < lf))
            ++p2;
    }
    printf("pt1 = %d\npt2 = %d\n", p1, p2);
}