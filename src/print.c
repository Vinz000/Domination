#include "io.h"

WINDOW *g_boardWin, *g_titleWin, *g_stackWin, *g_statsWin, *g_noticeWin, *g_controlsWin, *g_nameWin;
int g_referenceX1, g_referenceX2;

void printColor(WINDOW *win, int locY, int locX, int colorType, int height, int width)
{
    wattron(win, COLOR_PAIR(colorType));

    for (int i = 0; i < height; i++)
        for (int j = 0; j < width - 1; j++)
            mvwprintw(win, locY + i, locX + j, "  ");

    wattroff(win, COLOR_PAIR(colorType));
}

void updateNoticeWin(char *message)
{
    werase(g_noticeWin);
    box(g_noticeWin, 0, 0);

    mvwprintw(g_noticeWin, 1, 1, "%s", message);

    wrefresh(g_noticeWin);
}

void updateNameWin(player *p)
{
    werase(g_nameWin);
    box(g_nameWin, 0, 0);

    int length = strlen(p->name);
    mvwprintw(g_nameWin, 1, (18 - length) / 2, "~%s~", p->name);

    wrefresh(g_nameWin);
}

void updateStatsWin(player *p)
{
    werase(g_statsWin);
    box(g_statsWin, 0, 0);

    mvwprintw(g_statsWin, 1, 2, "~Player Details~");
    mvwprintw(g_statsWin, 3, 1, "Color:");
    (PLAYER_COLOR == GREEN) ? printColor(g_statsWin, 3, 9, GREEN_COLOR, 1, 8)
                            : printColor(g_statsWin, 3, 9, RED_COLOR, 1, 8);
    mvwprintw(g_statsWin, 4, 1, "Reserve pieces: %d", p->own_pieces);
    mvwprintw(g_statsWin, 5, 1, "Enemies pieces: %d", p->opp_pieces);

    wrefresh(g_statsWin);
}

void updateStackWin(square *s)
{
    werase(g_stackWin);
    box(g_stackWin, 0, 0);

    int num = s->num_pieces;
    piece *temp = s->stack;

    if (temp != NULL)
    {
        for (int i = 16 - 3 * num; i <= 13; i += 3)
        {
            (temp->pce_color == GREEN) ? printColor(g_stackWin, i, 1, GREEN_COLOR, 2, 10)
                                       : printColor(g_stackWin, i, 1, RED_COLOR, 2, 10);
            temp = temp->next;
        }
    }

    wrefresh(g_stackWin);
}

void drawCursor(int cursorY, int cursorX, square *s)
{
    printColor(g_boardWin, cursorY, cursorX, WHITE_COLOR, 2, 4);
    updateStackWin(s);
    wrefresh(g_boardWin);
}

void revertColor(int cursorY, int cursorX, square *s)
{
    int colorType;
    if (s->stack != NULL)
    {
        if (s->stack->pce_color == GREEN)
            colorType = GREEN_COLOR;
        else
            colorType = RED_COLOR;
    }
    else
        colorType = DEFAULT_COLOR;

    printColor(g_boardWin, cursorY, cursorX, colorType, 2, 4);
}

void HozLines(cursorLocation *cursor)
{
    for (int i = 0; i < 4; i++)
        mvwaddch(g_boardWin, CURSOR_Y, CURSOR_X++, ACS_HLINE);
}

void vertLines(square board[BOARD_SIZE][BOARD_SIZE], cursorLocation *cursor, int max)
{
    getArrayValues(cursor);
    while (CURSOR_X <= max)
    {
        ARRAY_X = (CURSOR_X) / 5;

        mvwaddch(g_boardWin, CURSOR_Y, CURSOR_X, ACS_VLINE);
        mvwaddch(g_boardWin, CURSOR_Y+1, CURSOR_X, ACS_VLINE);

        if (CURRENT_PIECE.stack != NULL && CURSOR_X != max)
        {
            (CURRENT_PIECE_COLOR == GREEN) ? printColor(g_boardWin, CURSOR_Y, CURSOR_X + 1, GREEN_COLOR, 2, 4)
                                            : printColor(g_boardWin, CURSOR_Y, CURSOR_X + 1, RED_COLOR, 2, 4);
        }
        CURSOR_X += 5;
    }
    
    CURSOR_Y += 2;
}

void topLayer(square board[BOARD_SIZE][BOARD_SIZE], cursorLocation *cursor)
{
    int minRowX; 
    int maxRowX; 
    for (int i = 0; i < 3; i++)
    {
        CURSOR_X = minRowX = g_referenceX1 - (5 * i);
        maxRowX = g_referenceX2 + (5 * i);

        while (CURSOR_X <= maxRowX)
        {
            if (CURSOR_X == minRowX)
                mvwaddch(g_boardWin, CURSOR_Y, CURSOR_X++, ACS_ULCORNER);
            else if (CURSOR_X == maxRowX)
            {
                mvwaddch(g_boardWin, CURSOR_Y, CURSOR_X, ACS_URCORNER);
                break;
            }
            else
            {
                if (minRowX == g_referenceX1)
                    mvwaddch(g_boardWin, CURSOR_Y, CURSOR_X++, ACS_TTEE);
                else
                    mvwaddch(g_boardWin, CURSOR_Y, CURSOR_X++, ACS_PLUS);
            }
            HozLines(cursor);
        }
        CURSOR_Y++;
        CURSOR_X = minRowX; 
        vertLines(board, cursor, maxRowX); 
    }
}

void midLines(cursorLocation *cursor)
{
    CURSOR_X = g_referenceX1 - 10;

    for (int i = 0; i < 9; i++)
    {
        if (i == 0)
            mvwaddch(g_boardWin, CURSOR_Y, CURSOR_X++, ACS_LTEE);
        else if (i == 8)
        {
            mvwaddch(g_boardWin, CURSOR_Y, CURSOR_X, ACS_RTEE);
            break;
        }
        else
            mvwaddch(g_boardWin, CURSOR_Y, CURSOR_X++, ACS_PLUS);
        HozLines(cursor);
    }
    CURSOR_Y++;
}

void middleLayer(square board[BOARD_SIZE][BOARD_SIZE], cursorLocation *cursor)
{
    for (int i = 0; i < 2; i++)
    {
        midLines(cursor);
        CURSOR_X = g_referenceX1 - 10;
        vertLines(board, cursor, g_referenceX2 + 10);
    }
    
    midLines(cursor);
}

void bottomLayer(square board[BOARD_SIZE][BOARD_SIZE], cursorLocation *cursor)
{
    int minRowX, maxRowX;
    for (int i = 2; i >= 0; i--)
    {
        CURSOR_X = minRowX = g_referenceX1 - (5 * i);
        maxRowX = g_referenceX2 + (5 * i);

        vertLines(board, cursor, maxRowX);
        CURSOR_X = minRowX;

        while (CURSOR_X <= maxRowX)
        {
            if (CURSOR_X == minRowX)
                mvwaddch(g_boardWin, CURSOR_Y, CURSOR_X++, ACS_LLCORNER);
            else if (CURSOR_X == maxRowX)
            {
                mvwaddch(g_boardWin, CURSOR_Y, CURSOR_X, ACS_LRCORNER);
                break;
            }
            else
            {
                if (minRowX == g_referenceX1)
                    mvwaddch(g_boardWin, CURSOR_Y, CURSOR_X++, ACS_BTEE);
                else
                    mvwaddch(g_boardWin, CURSOR_Y, CURSOR_X++, ACS_PLUS);
            }
            HozLines(cursor);
        }
        CURSOR_Y++;
    }
}

void drawBoard(square board[BOARD_SIZE][BOARD_SIZE], cursorLocation *cursor)
{
    werase(g_boardWin);

    int x = CURSOR_X;
    int y = CURSOR_Y;

    g_referenceX1 = 10; 
    g_referenceX2 = 30; 

    CURSOR_X = CURSOR_Y = 0;

    topLayer(board, cursor);
    middleLayer(board, cursor);
    bottomLayer(board, cursor);

    CURSOR_X = x;
    CURSOR_Y = y;

    getArrayValues(cursor);
}

void removeWin(WINDOW *win)
{
    werase(win);
    wrefresh(win);
    delwin(win);
}

void winScreen(player *p)
{
    int maxScreenX = getmaxx(stdscr);

    removeWin(g_boardWin);
    removeWin(g_controlsWin);
    removeWin(g_stackWin);
    removeWin(g_statsWin);
    removeWin(g_noticeWin);
    removeWin(g_titleWin);

    g_titleWin = newwin(9, 62, 7, (maxScreenX / 2) - 31);

    (PLAYER_COLOR == GREEN) ? wbkgd(g_titleWin, COLOR_PAIR(GREEN_TEXT))
                            : wbkgd(g_titleWin, COLOR_PAIR(RED_TEXT));

    mvwprintw(g_titleWin, 0, 0, " __       __  ______  __    __  __    __  ________  _______  \n"
                                "|  \\  _  |  \\|      \\|  \\  |  \\|  \\  |  \\|        \\|       \\ \n"
                                "| $$ / \\ | $$ \\$$$$$$| $$\\ | $$| $$\\ | $$| $$$$$$$$| $$$$$$$\\\n"
                                "| $$/  $\\| $$  | $$  | $$$\\| $$| $$$\\| $$| $$__    | $$__| $$\n"
                                "| $$  $$$\\ $$  | $$  | $$$$\\ $$| $$$$\\ $$| $$  \\   | $$    $$\n"
                                "| $$ $$\\$$\\$$  | $$  | $$\\$$ $$| $$\\$$ $$| $$$$$   | $$$$$$$\\\n"
                                "| $$$$  \\$$$$ _| $$_ | $$ \\$$$$| $$ \\$$$$| $$_____ | $$  | $$\n"
                                "| $$$    \\$$$|   $$ \\| $$  \\$$$| $$  \\$$$| $$     \\| $$  | $$\n"
                                " \\$$      \\$$ \\$$$$$$ \\$$   \\$$ \\$$   \\$$ \\$$$$$$$$ \\$$   \\$$\n");

    g_noticeWin = newwin(7, 52, 20, (maxScreenX / 2) - 26);
    box(g_noticeWin, 0, 0);
    mvwprintw(g_noticeWin, 1, 10, "Congrats %s for winning the game!", p->name);
    mvwprintw(g_noticeWin, 3, 21, "Goodbye!!!");
    mvwprintw(g_noticeWin, 5, 1, "Press ANY KEY to exit...");

    wrefresh(stdscr);
    wrefresh(g_titleWin);
    wrefresh(g_noticeWin);

    int inputChar = getch();

    endwin();
    exit(1);
}