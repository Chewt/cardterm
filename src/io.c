#include "io.h"
#include <ctype.h>
#include <stdio.h>

void get_player_input(char* s)
{
    if (s[0] == 0)
        fgets(s, 255, stdin);
    s[255] = '\n';
    int i;
    while (s[i] != '\n')
    {
        s[i] = tolower(s[i]);
        i++;
    }
    s[i] = 0;
}

void cur_Y(int y)
{
    if (y < 0)      // Move Up
        printf("\e[%dA", -y);
    else if (y > 0) // Move Down
        printf("\e[%dB", y);
}

void cur_X(int x)
{
    if (x > 0)      // Move Right
        printf("\e[%dC", x);
    else if (x < 0) // Move Left
        printf("\e[%dD", -x);
}

void cur_MOV(int x, int y)
{
    cur_Y(y);
    cur_X(x);
}

void cur_CR()
{
    printf("\r");
}

void cur_HOME()
{
    printf("\e[H");
}

void cur_SAV()
{
    printf("\e[s");
}

void cur_RES()
{
    printf("\e[u");
}
