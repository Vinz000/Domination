#include "print.h"

#ifndef SW2_IO_H
#define SW2_IO_H

void endScreen(player *p, play *g);
void getArrayValues(cursorLocation *cursor);
void printColor(WINDOW *win, int y, int x, int colorType, int height, int width);
bool checkWin(square board[BOARD_SIZE][BOARD_SIZE], play *g, player players[PLAYERS_NUM]);
void runGame(square board[BOARD_SIZE][BOARD_SIZE], player players[PLAYERS_NUM], play *info, cursorLocation *cursor);

#endif 
