#ifndef UTILS_H
#define UTILS_H

#include "vector.h"

#define BOARD_SIZE 8
#define ROW_SIZE 4

void print_board(unsigned player1, unsigned player2);

int next_left(int pos, int direction);
int next_right(int pos, int direction);

int can_move_left(int pos, unsigned board, int direction);
int can_move_right(int pos, unsigned board, int direction);
int can_move(int pos, unsigned player, unsigned opponent, int direction);
vector *potential_moves(unsigned player, unsigned opponent, int direction);

int can_take_left(int pos, unsigned player, unsigned opponent, int direction);
int can_take_right(int pos, unsigned player, unsigned opponent, int direction);
int can_take(int pos, unsigned player, unsigned opponent);
vector *potential_takes(unsigned player, unsigned opponent);

int do_take_left(int pos, unsigned *player, unsigned *opponent, int direction);
int do_take_right(int pos, unsigned *player, unsigned *opponent, int direction);

int max2(int a, int b);
int max4(int a, int b, int c, int d);

int count_max_takes(int pos, unsigned player, unsigned opponent);
vector *max_takes(unsigned player, unsigned opponent);
int do_max_takes(int *pos, unsigned *player, unsigned *opponent);

#endif // UTILS_H
