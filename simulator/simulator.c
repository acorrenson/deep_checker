#include "utils.h"
#include "vector.h"
#include <stdio.h>

vector *candidates(unsigned player, unsigned opponent, int direction)
{
    vector *take = vector_create();
    vector *move = vector_create();
    for (int pos = 0; pos < BOARD_SIZE * ROW_SIZE; pos++)
    {
        if (player & (1 << pos))
        {
            if (can_take_left(pos, player, opponent, direction) || can_take_right(pos, player, opponent, direction))
                vector_insert(take, pos);
            else if (take->len == 0 && (can_move_left(pos, player | opponent, direction) || can_move_right(pos, player | opponent, direction)))
                vector_insert(move, pos);
        }
    }
    if (take->len > 0)
    {
        vector_delete(move);
        return take;
    }
    else
    {
        vector_delete(take);
        return move;
    }
}

int main()
{
    unsigned player1 = 0b00000000000001100000111111111111;
    unsigned player2 = 0b11111111111100000110000000000000;
    print_board(player1 | player2);
    vector *v = candidates(player1, player2, 0);
    vector_print(v);
    vector_delete(v);
    v = candidates(player2, player1, 1);
    vector_print(v);
    vector_delete(v);
    return 0;
}
