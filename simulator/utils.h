#ifndef UTILS_H
#define UTILS_H

#include "vector.h"

#define BOARD_SIZE 8
#define ROW_SIZE 4

void print_board(unsigned player1, unsigned player2);

unsigned next_left(unsigned pos, int direction);
unsigned next_right(unsigned pos, int direction);

int can_move_left(unsigned pos, unsigned board, int direction);
int can_move_right(unsigned pos, unsigned board, int direction);
int can_move(unsigned pos, unsigned board, int direction);
vector *potential_moves(unsigned player, unsigned opponent, int direction);

unsigned do_move_left(unsigned pos, unsigned *player, int direction);
unsigned do_move_right(unsigned pos, unsigned *player, int direction);

int can_take_left(unsigned pos, unsigned player, unsigned opponent,
                  int direction);
int can_take_right(unsigned pos, unsigned player, unsigned opponent,
                   int direction);
int can_take(unsigned pos, unsigned player, unsigned opponent);
vector *potential_takes(unsigned player, unsigned opponent);

unsigned do_take_left(unsigned pos, unsigned *player, unsigned *opponent,
                      int direction);
unsigned do_take_right(unsigned pos, unsigned *player, unsigned *opponent,
                       int direction);

int max2(int a, int b);
int max4(int a, int b, int c, int d);

int count_max_takes(unsigned pos, unsigned player, unsigned opponent);
vector *max_takes(unsigned player, unsigned opponent);
int do_max_takes(unsigned *pos, unsigned *player, unsigned *opponent);

#endif // UTILS_H
