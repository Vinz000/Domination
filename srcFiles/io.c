#include "io.h"
#include <ctype.h>

WINDOW *g_boardWin, *g_titleWin, *g_stackWin, *g_statsWin, *g_noticeWin, *g_controlsWin, *g_nameWin;
PANEL *g_panel_1, *g_panel_2, *g_panel_3;

void getArrayValues(cursorLocation *cursor)
{
    ARRAY_X = (CURSOR_X - 1) / 5;
    ARRAY_Y = (CURSOR_Y - 1) / 3;
}

bool validMove(square *s, int boardY, int boardX)
{
    return (!(boardY < 0 || boardX < 0 || boardY > 7 || boardX > 7 || s->type == INVALID));
}

void isChosenTile(square *s, play *info, cursorLocation *cursor)
{
    if (!(CHOSEN_X == ARRAY_X && CHOSEN_Y == ARRAY_Y))
        revertColor(CURSOR_Y, CURSOR_X, s);
    else
        printColor(g_boardWin, CURSOR_Y, CURSOR_X, BLUE_COLOR, 2, 4);
}

bool ownPiece(square *s, player *p)
{
    return (s->stack != NULL && s->stack->pce_color == PLAYER_COLOR);
}

bool canChoosePiece(square *s, player *p, play *info)
{

    if (ownPiece(s, p))
    {
        updateNoticeWin("Choose piece to land on!");
        HAS_CHOSEN = true;       
        PLACING_METHOD = PIECES; 
        return true;
    }
    updateNoticeWin("You can only select your own piece!");
    return false;
}

bool checkDistance(cursorLocation *cursor, play *info, square *s)
{
    int distance = abs(CHOSEN_Y - ARRAY_Y) + abs(CHOSEN_X - ARRAY_X);

    if (distance == 0)
    {
        updateNoticeWin("You have to move to another square! - Press 'a' to cancel selection");
        return false;
    }
    else if (distance > s->num_pieces)
    {
        updateNoticeWin("You can't move that far!");
        return false;
    }
    return true;
}

void checkStackSize(player *p, square *s)
{
    if (s->num_pieces > 5)
    {
        int count = 1;
        piece *curr = s->stack; 
        piece *last;            
        piece *toRemove;        

        while (curr != NULL)
        {
            if (count < 5)
            {
                curr = curr->next;
                count++;
            }
            else
            {
                last = curr;

                if (last != NULL)
                {
                    curr = curr->next;
                    while (curr != NULL)
                    {
                        (curr->pce_color == PLAYER_COLOR) ? p->own_pieces++ : p->opp_pieces++;
                        toRemove = curr; 
                        curr = curr->next;
                        free(toRemove);
                    }
                    last->next = NULL;
                }
            }
        }
        s->num_pieces = 5;
    }
}

void merge(square board[BOARD_SIZE][BOARD_SIZE], player *p, play *info, cursorLocation *cursor)
{
    piece *top = CHOSEN_PIECE.stack;
    piece *curr = top;

    CHOSEN_PIECE.stack = NULL;

    while (curr->next != NULL)
        curr = curr->next;

    curr->next = CURRENT_PIECE.stack;
    CURRENT_PIECE.stack = top;

    CURRENT_PIECE.num_pieces += CHOSEN_PIECE.num_pieces;
    CHOSEN_PIECE.num_pieces = 0;

    checkStackSize(p, &CURRENT_PIECE);
}

void returnPiece(player *p, square *s)
{
    piece *returnPiece = (piece *)malloc(sizeof(piece));

    returnPiece->pce_color = PLAYER_COLOR;

    returnPiece->next = s->stack;
    s->stack = returnPiece;

    s->num_pieces++;
    p->own_pieces--;

    checkStackSize(p, s);
}

void changeTurn(play *info)
{
    info->player_turn++;
    info->player_turn %= 2; 
    HAS_MOVED = false;
    HAS_CHOSEN = false;
    CHOSEN_X = CHOSEN_Y = 0;
}

bool checkWin(square board[BOARD_SIZE][BOARD_SIZE], play *info, player players[PLAYERS_NUM])
{
    if (CURRENT_PLAYER.own_pieces == 0)
    {
        for (int i = 0; i < BOARD_SIZE; i++)
            for (int j = 0; j < BOARD_SIZE; j++)
                if (board[i][j].stack != NULL &&
                    board[i][j].stack->pce_color == CURRENT_PLAYER.player_color)
                    return false;
        changeTurn(info);
        winScreen(&CURRENT_PLAYER);
    }
    return false; 
}

void runGame(square board[BOARD_SIZE][BOARD_SIZE], player players[PLAYERS_NUM], play *info, cursorLocation *cursor)
{
    updateNameWin(&CURRENT_PLAYER);
    updateStatsWin(&CURRENT_PLAYER);
    updateNoticeWin("New turn, choose move!");

    drawBoard(board, cursor);
    drawCursor(CURSOR_Y, CURSOR_X, &CURRENT_PIECE);

    while (HAS_MOVED != true)
    {
        int inputChar = getch();

        getArrayValues(cursor);
        switch (tolower(inputChar))
        {

        case KEY_UP:
            if (validMove(&board[ARRAY_Y - 1][ARRAY_X], ARRAY_Y - 1, ARRAY_X))
            {
                isChosenTile(&CURRENT_PIECE, info, cursor);
                CURSOR_Y -= 3;
                drawCursor(CURSOR_Y, CURSOR_X, &board[ARRAY_Y - 1][ARRAY_X]);
            }
            break;
        case KEY_DOWN:
            if (validMove(&board[ARRAY_Y + 1][ARRAY_X], ARRAY_Y + 1, ARRAY_X))
            {
                isChosenTile(&CURRENT_PIECE, info, cursor);
                CURSOR_Y += 3;
                drawCursor(CURSOR_Y, CURSOR_X, &board[ARRAY_Y + 1][ARRAY_X]);
            }
            break;
        case KEY_LEFT:
            if (validMove(&board[ARRAY_Y][ARRAY_X - 1], ARRAY_Y, ARRAY_X - 1))
            {
                isChosenTile(&CURRENT_PIECE, info, cursor);
                CURSOR_X -= 5;
                drawCursor(CURSOR_Y, CURSOR_X, &board[ARRAY_Y][ARRAY_X - 1]);
            }
            break;
        case KEY_RIGHT:
            if (validMove(&board[ARRAY_Y][ARRAY_X + 1], ARRAY_Y, ARRAY_X + 1))
            {
                isChosenTile(&CURRENT_PIECE, info, cursor);
                CURSOR_X += 5;
                drawCursor(CURSOR_Y, CURSOR_X, &board[ARRAY_Y][ARRAY_X + 1]);
            }
            break;
        case 's':
            if (HAS_CHOSEN == false)
            {
                if (canChoosePiece(&board[ARRAY_Y][ARRAY_X], &CURRENT_PLAYER, info))
                {
                    CHOSEN_X = ARRAY_X;
                    CHOSEN_Y = ARRAY_Y;
                    printColor(g_boardWin, CURSOR_Y, CURSOR_X, BLUE_COLOR, 2, 4);
                }
            }
            else 
            {
                if (PLACING_METHOD == RESERVES)
                    updateNoticeWin("You already decided to use your reserves. Cancel if you change your mind.");
                else
                    updateNoticeWin("Just waiting for you to choose a piece to move to.");
            }
            break;
        case 'w':
            if (HAS_CHOSEN == false)
            {
                if (CURRENT_PLAYER.own_pieces > 0)
                {
                    HAS_CHOSEN = true;
                    PLACING_METHOD = RESERVES;
                    updateNoticeWin("Select which piece to return on!");
                }
                else
                    updateNoticeWin("You have no reserves to use!");
            }
            else
            {
                if (PLACING_METHOD == RESERVES)
                    updateNoticeWin("Just waiting for you to choose a piece to ressurect on.");
                else
                    updateNoticeWin("You already selected to move a piece. Cancel if you change your mind.");
            }
            break;
        case 'd':
            if (HAS_CHOSEN == true)
            {
                if (PLACING_METHOD == PIECES)
                {
                    if (checkDistance(cursor, info, &CHOSEN_PIECE))
                    {
                        merge(board, &CURRENT_PLAYER, info, cursor);
                        HAS_MOVED = true;
                    }
                }
                else
                {
                    returnPiece(&CURRENT_PLAYER, &CURRENT_PIECE);
                    HAS_MOVED = true;
                }
            }
            else
                updateNoticeWin("You have nothing to place!");
            break;
        case 'a':
            if (HAS_CHOSEN == true)
            {
                if (PLACING_METHOD == PIECES)
                {
                    revertColor(CHOSEN_Y_LOC, CHOSEN_X_LOC, &CHOSEN_PIECE);
                    drawCursor(CURSOR_Y, CURSOR_X, &CHOSEN_PIECE);
                    updateStackWin(&CURRENT_PIECE);
                    CHOSEN_X = CHOSEN_Y = 0;
                }
                updateNoticeWin("Succesfully cancelled move!");
                HAS_CHOSEN = false;
            }
            else
                updateNoticeWin("You have to choose what move to do first!");
            break;
        case '\t':
            if (!panel_hidden(g_panel_2))
            {
                hide_panel(g_panel_2);
                hide_panel(g_panel_3);
            }
            else 
            {
                show_panel(g_panel_3);
                show_panel(g_panel_2);
            }
            update_panels(); 
            doupdate();      
            break;
        case 'q':
            updateNoticeWin("Press q - quit || Press ANY other key - continue game");
            int quit = getch();
            if (tolower(quit) == 'q')
            {
                endwin(); 
                puts("\nThankyou for playing.\nMaybe try a full game next time!\n");
                exit(1); 
            }
            updateNoticeWin("Continue your turn...");
            break;
        }
    }
    changeTurn(info);
}