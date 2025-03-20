#ifndef __CONNECT4_AI_H_
#define __CONNECT4_AI_H_

#include <stdbool.h>

extern int check(char *board, int height, int width, int column);

extern int drop(char *board, int height, int width, int column, char symbol);

extern bool win(char *board, int height, int width, int row, int column);

extern int connect4_ai(char *board, int height, int width, char myself);

#endif
