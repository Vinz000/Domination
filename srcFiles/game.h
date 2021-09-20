#ifndef SW2_GAME_H
#define SW2_GAME_H

#include <stdbool.h>
#include <stdlib.h>
#include <string.h> 
#include <panel.h>

#define BOARD_SIZE 8
#define PLAYERS_NUM 2
#define WINDOWS_NUM 6
#define PANELS_NUM 2

#define ARRAY_Y cursor->arrayY
#define ARRAY_X cursor->arrayX
#define CURSOR_X cursor->cursorX
#define CURSOR_Y cursor->cursorY

#define CURRENT_PIECE board[ARRAY_Y][ARRAY_X]
#define CURRENT_STACK_HEIGHT board[ARRAY_Y][ARRAY_X].num_pieces
#define CURRENT_PIECE_COLOR board[ARRAY_Y][ARRAY_X].stack->pce_color
#define CURRENT_PLAYER players[info->player_turn]
#define PLAYER_COLOR p->player_color

#define CHOSEN_X info->chosen.x
#define CHOSEN_Y info->chosen.y
#define CHOSEN_X_LOC ((5 * CHOSEN_X) + 1)
#define CHOSEN_Y_LOC ((3 * CHOSEN_Y) + 1)
#define CHOSEN_PIECE board[CHOSEN_Y][CHOSEN_X]

#define HAS_MOVED info->madeMove
#define PLACING_METHOD info->method
#define HAS_CHOSEN info->didChoose

typedef enum color
{
    RED,
    GREEN
} color;

typedef enum square_type
{
    VALID,
    INVALID
} square_type;

typedef struct piece
{
    color pce_color;
    struct piece *next;
} piece;

//square information
typedef struct square
{
    square_type type;
    piece *stack;
    int num_pieces;
} square;

// player information
typedef struct player
{
    char name[16];
    color player_color;
    int own_pieces;
    int opp_pieces;
} player;

typedef struct chosenPieceLocation
{
    int x;
    int y;
    int numPieces;
} chosenPieceLocation;

typedef struct cursorLocation
{
    int cursorX;
    int cursorY;
    int arrayX;
    int arrayY;
} cursorLocation;

typedef enum mergeMethod
{
    PIECES,
    RESERVES, 
} mergeMethod;

typedef struct play
{
    int player_turn;    
    bool madeMove;       
    bool didChoose;      
    chosenPieceLocation chosen;   
    mergeMethod method; 
} play;

typedef struct visual
{
    WINDOW *win[WINDOWS_NUM];
    PANEL *panel[PANELS_NUM];
} visual;

void initializeInfo(play *info, cursorLocation *cursor);
void initializePlayers(player players[PLAYERS_NUM]);
void initializeBoard(square board[BOARD_SIZE][BOARD_SIZE]);

#endif 