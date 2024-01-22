#ifndef IO_H
#define IO_H

// String colors
#define STR_RED "\e[31;1m"
#define STR_NML "\e[0m"

// Clear screen macro
#define CLEAR_SCREEN() printf("\e[2J\e[H\n"); 

void mov_vrt(int y);
void mov_hoz(int x);
void mov_cur(int x, int y);
void mov_CR();
void mov_HOME();

#endif
