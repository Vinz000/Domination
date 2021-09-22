#include "io.h"

WINDOW *g_boardWin, *g_titleWin, *g_stackWin, *g_statsWin, *g_noticeWin, *g_controlsWin, *g_nameWin;
PANEL *g_panel_1, *g_panel_2, *g_panel_3;

void initializeCurses()
{
    initscr();            
    noecho();             
    keypad(stdscr, TRUE); 

    start_color(); 
    init_pair(DEFAULT_COLOR, COLOR_WHITE, COLOR_BLACK);
    init_pair(GREEN_COLOR, COLOR_GREEN, COLOR_GREEN);
    init_pair(WHITE_COLOR, COLOR_WHITE, COLOR_WHITE);
    init_pair(BLUE_COLOR, COLOR_BLUE, COLOR_BLUE);
    init_pair(RED_COLOR, COLOR_RED, COLOR_RED);
    init_pair(RED_TEXT, COLOR_RED, COLOR_BLACK);
    init_pair(CYAN_TEXT, COLOR_CYAN, COLOR_BLACK);
    init_pair(GREEN_TEXT, COLOR_GREEN, COLOR_BLACK);
    curs_set(0); 

    bkgd(COLOR_PAIR(DEFAULT_COLOR));
    wrefresh(stdscr);
}

void initializeWindows()
{
    int maxScreenX, maxScreenY; 
    getmaxyx(stdscr, maxScreenY, maxScreenX);

    if (maxScreenY < 39 || maxScreenX < 88)
    {
        endwin();
        printf("Change size of terminal to atleast 86*39\n");
        printf("Try fullscreen!\n");
        exit(1);
    }

    g_titleWin = newwin(7, 86, 1, (maxScreenX / 2) - 43);
    g_boardWin = newwin(25, 42, 9, (maxScreenX) / 2 - 36);
    g_stackWin = newwin(16, 12, 9, (maxScreenX) / 2 + 10);
    g_noticeWin = newwin(3, 86, 36, (maxScreenX / 2) - 43);
    g_nameWin = newwin(3, 20, 25, (maxScreenX) / 2 + 10);
    g_statsWin = newwin(7, 20, 28, (maxScreenX) / 2 + 10);
    g_controlsWin = newwin(10, 20, 25, (maxScreenX / 2) + 10);

    g_panel_1 = new_panel(g_controlsWin);
    g_panel_2 = new_panel(g_statsWin);
    g_panel_3 = new_panel(g_nameWin);

    set_panel_userptr(g_panel_1, g_panel_2);
    set_panel_userptr(g_panel_2, g_panel_1);

    box(g_controlsWin, 0, 0);
    mvwprintw(g_controlsWin, 1, 5, "~Controls~");
    mvwprintw(g_controlsWin, 3, 1, "Q: Quit");
    mvwprintw(g_controlsWin, 4, 1, "A: Cancel Move");
    mvwprintw(g_controlsWin, 5, 1, "D: Place Piece");
    mvwprintw(g_controlsWin, 6, 1, "S: Select Piece");
    mvwprintw(g_controlsWin, 7, 1, "W: Return Piece");
    mvwprintw(g_controlsWin, 8, 1, "Move: ARROW KEYS");

    wprintw(g_titleWin, "########   #######  ##     ## #### ##    ##    ###    ######## ####  #######  ##    ##"
                        "##     ## ##     ## ###   ###  ##  ###   ##   ## ##      ##     ##  ##     ## ###   ##"
                        "##     ## ##     ## #### ####  ##  ####  ##  ##   ##     ##     ##  ##     ## ####  ##"
                        "##     ## ##     ## ## ### ##  ##  ## ## ## ##     ##    ##     ##  ##     ## ## ## ##"
                        "##     ## ##     ## ##     ##  ##  ##  #### #########    ##     ##  ##     ## ##  ####"
                        "##     ## ##     ## ##     ##  ##  ##   ### ##     ##    ##     ##  ##     ## ##   ###"
                        "########   #######  ##     ## #### ##    ## ##     ##    ##    ####  #######  ##    ##");

    wbkgd(g_controlsWin, COLOR_PAIR(DEFAULT_COLOR));
    wbkgd(g_boardWin, COLOR_PAIR(DEFAULT_COLOR));
    wbkgd(g_stackWin, COLOR_PAIR(DEFAULT_COLOR));
    wbkgd(g_titleWin, COLOR_PAIR(CYAN_TEXT));
    wbkgd(g_statsWin, COLOR_PAIR(DEFAULT_COLOR));
    wbkgd(g_noticeWin, COLOR_PAIR(DEFAULT_COLOR));

    wrefresh(g_titleWin);
}
