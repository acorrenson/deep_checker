#ifndef UTILS_H
#define UTILS_H

#define BOARD_SIZE 8
#define ROW_SIZE 4

void print_board(unsigned board);

int can_move_left(int pos, unsigned board, int direction);
int can_move_right(int pos, unsigned board, int direction);

int can_take_left(int pos, unsigned player, unsigned opponent, int direction);
int can_take_right(int pos, unsigned player, unsigned opponent, int direction);

#endif // UTILS_H
