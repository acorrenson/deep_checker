#ifndef UTILS_H
#define UTILS_H

#include "vector.h"

#define BOARD_SIZE 8
#define ROW_SIZE 4

void print_board(unsigned board);

int can_move_left(int pos, unsigned board, int direction);
int can_move_right(int pos, unsigned board, int direction);
int can_move(int pos, unsigned player, unsigned opponent, int direction);

int can_take_left(int pos, unsigned player, unsigned opponent, int direction);
int can_take_right(int pos, unsigned player, unsigned opponent, int direction);
int can_take(int pos, unsigned player, unsigned opponent);

vector *potential_takes(unsigned player, unsigned opponent);
vector *potential_moves(unsigned player, unsigned opponent, int direction);

void do_take_left(int pos, unsigned *p1, unsigned *p2, int dir);
void do_take_right(int pos, unsigned *p1, unsigned *p2, int dir);

int max2(int a, int b);

int max4(int a, int b, int c, int d);

int count_max_takes(int pos, unsigned player, unsigned opponent);

vector *max_takes(unsigned player, unsigned opponent);

#endif // UTILS_H
