#ifndef SW2_PRINT_H
#define SW2_PRINT_H

#include "curses.h"

void updateNameWin(player *p);
void updateStatsWin(player *p);
void updateStackWin(square *s);
void updateNoticeWin(char *message);
void drawCursor(int cursorY, int cursorX, square *s);
void revertColor(int cursorY, int cursorX, square *s);
void printColor(WINDOW *win, int y, int x, int colorType, int height, int width);

#endif 