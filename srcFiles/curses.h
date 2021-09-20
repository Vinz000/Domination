#ifndef SW2_CURSES_H
#define SW2_CURSES_H

#include <ncurses.h>
#include "game.h"

// colors used
#define DEFAULT_COLOR 1
#define GREEN_COLOR 2
#define WHITE_COLOR 3
#define BLUE_COLOR 4
#define RED_COLOR 5
#define RED_TEXT 6
#define CYAN_TEXT 7
#define GREEN_TEXT 8

void initializeCurses();
void initializeWindows();
void winScreen(player *p);
void drawBoard(square board[BOARD_SIZE][BOARD_SIZE], cursorLocation *cursor);

#endif 
