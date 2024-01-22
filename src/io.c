#include "io.h"
#include <stdio.h>

void mov_vrt(int y)
{
    if (y < 0)      // Move Up
        printf("\e[%dA", -y);
    else if (y > 0) // Move Down
        printf("\e[%dB", y);
}

void mov_hoz(int x)
{
    if (x > 0)      // Move Right
        printf("\e[%dC", x);
    else if (x < 0) // Move Left
        printf("\e[%dD", -x);
}

void mov_cur(int x, int y)
{
    mov_vrt(y);
    mov_hoz(x);
}

void mov_CR()
{
    printf("\r");
}

void mov_HOME()
{
    printf("\e[H");
}

