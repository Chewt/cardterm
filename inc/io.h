#ifndef IO_H
#define IO_H

// String colors
#define STR_RED "\e[31;1m"
#define STR_NML "\e[0m"

// Clear screen macro
#define CLEAR_SCREEN() printf("\e[2J\e[H\n"); 

void cur_Y(int y);
void cur_X(int x);
void cur_MOV(int x, int y);
void cur_CR();
void cur_HOME();
void cur_SAV();
void cur_RES();

#endif
