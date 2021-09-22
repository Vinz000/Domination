#include <panel.h>
#include "io.h"

int main()
{
    play info;
    cursorLocation cursor;
    square board[BOARD_SIZE][BOARD_SIZE];
    player players[PLAYERS_NUM];

    initializeInfo(&info, &cursor);
    initializePlayers(players);
    initializeBoard(board);
    initializeCurses();
    initializeWindows();

    while (!checkWin(board, &info, players))
    {
        runGame(board, players, &info, &cursor);
    }
}
