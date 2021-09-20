#include "game.h"
#include <stdio.h>

void setInvalid(square *s)
{
    s->type = INVALID;
    s->stack = NULL;
}

void setEmpty(square *s)
{
    s->type = VALID;
    s->stack = NULL;
    s->num_pieces = 0;
}

void setGreen(square *s)
{
    s->num_pieces = 1;
    s->type = VALID;
    s->stack = (piece *)malloc(sizeof(piece));
    s->stack->pce_color = GREEN;
    s->stack->next = NULL;
}

void setRed(square *s)
{
    s->num_pieces = 1;
    s->type = VALID;
    s->stack = (piece *)malloc(sizeof(piece));
    s->stack->pce_color = RED;
    s->stack->next = NULL;
}

void initializeInfo(play *info, cursorLocation *cursor)
{
    info->player_turn = 0;
    HAS_MOVED = false;
    HAS_CHOSEN = false;
    CURSOR_X = 6;
    CURSOR_Y = 4;
    CHOSEN_Y = CHOSEN_X = 0; 

    // Inform players of how to check controls
    printf("\n***********************************"
           "\nPress TAB key to check the controls\n"
           "***********************************\n");
}

void initializePlayers(player players[PLAYERS_NUM])
{
    printf("\nInput name of max 12 characters!\n");
    for (int i = 0; i < PLAYERS_NUM; i++)
    {
        player *p = &players[i];

        while (strlen(p->name) > 12 || strlen(p->name) == 0)
        {
            printf("Enter name for player %d:", i + 1);
            scanf(" %s", p->name);
        }

        p->own_pieces = 0;
        p->opp_pieces = 0;

        if (i == 0)
            PLAYER_COLOR = GREEN;
        else
            PLAYER_COLOR = RED;
    }
}

void initializeBoard(square board[BOARD_SIZE][BOARD_SIZE])
{
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            if ((i == 0 && (j == 0 || j == 1 || j == 6 || j == 7)) ||
                (i == 1 && (j == 0 || j == 7)) ||
                (i == 6 && (j == 0 || j == 7)) ||
                (i == 7 && (j == 0 || j == 1 || j == 6 || j == 7)))
                setInvalid(&board[i][j]);

            else
            {
                if (i == 0 || i == 7 || j == 0 || j == 7)
                    setEmpty(&board[i][j]);
                else
                {
                    if ((i % 2 == 1 && (j < 3 || j > 4)) ||
                        (i % 2 == 0 && (j == 3 || j == 4)))
                        setRed(&board[i][j]);
                    else
                        setGreen(&board[i][j]);
                }
            }
        }
    }
}
