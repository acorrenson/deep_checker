#include "utils.h"
#include <stdio.h>

void print_board(unsigned board)
{
    for (int row = 0; row < BOARD_SIZE; row++)
    {
        for (int col = 0; col < BOARD_SIZE; col++)
        {
            if ((row ^ col) & 1)
            {
                if (board & 1)
                    printf("x");
                else
                    printf(".");
                board >>= 1;
            }
            else
                printf(" ");
        }
        printf("\n");
    }
}

int can_move_left(int pos, unsigned board, int direction)
{
    int row = pos / ROW_SIZE;
    int col = pos % ROW_SIZE;
    if (direction == 0)
    {
        if (row == BOARD_SIZE - 1 || (row % 2 == 0 && col == ROW_SIZE - 1))
            return 0;
        pos += (row % 2 == 0) ? ROW_SIZE + 1 : ROW_SIZE;
    }
    else
    {
        if (row == 0 || (row % 2 == 1 && col == 0))
            return 0;
        pos -= (row % 2 == 0) ? ROW_SIZE : ROW_SIZE + 1;
    }
    return !(board & (1 << pos));
}

int can_move_right(int pos, unsigned board, int direction)
{
    int row = pos / ROW_SIZE;
    int col = pos % ROW_SIZE;
    if (direction == 0)
    {
        if (row == BOARD_SIZE - 1 || (row % 2 == 1 && col == 0))
            return 0;
        pos += (row % 2 == 0) ? ROW_SIZE : ROW_SIZE - 1;
    }
    else
    {
        if (row == 0 || (row % 2 == 0 && col == ROW_SIZE - 1))
            return 0;
        pos -= (row % 2 == 0) ? ROW_SIZE - 1 : ROW_SIZE;
    }
    return !(board & (1 << pos));
}

int can_take_left(int pos, unsigned player, unsigned opponent, int direction)
{
    int row = pos / ROW_SIZE;
    int col = pos % ROW_SIZE;
    if (direction == 0)
    {
        if (row == BOARD_SIZE - 1 || (row % 2 == 0 && col == ROW_SIZE - 1))
            return 0;
        pos += (row % 2 == 0) ? ROW_SIZE + 1 : ROW_SIZE;
    }
    else
    {
        if (row == 0 || (row % 2 == 1 && col == 0))
            return 0;
        pos -= (row % 2 == 0) ? ROW_SIZE : ROW_SIZE + 1;
    }
    return (opponent & (1 << pos)) && can_move_left(pos, player | opponent, direction);
}

int can_take_right(int pos, unsigned player, unsigned opponent, int direction)
{
    int row = pos / ROW_SIZE;
    int col = pos % ROW_SIZE;
    if (direction == 0)
    {
        if (row == BOARD_SIZE - 1 || (row % 2 == 1 && col == 0))
            return 0;
        pos += (row % 2 == 0) ? ROW_SIZE : ROW_SIZE - 1;
    }
    else
    {
        if (row == 0 || (row % 2 == 0 && col == ROW_SIZE - 1))
            return 0;
        pos -= (row % 2 == 0) ? ROW_SIZE - 1 : ROW_SIZE;
    }
    return (opponent & (1 << pos)) && can_move_right(pos, player | opponent, direction);
}
