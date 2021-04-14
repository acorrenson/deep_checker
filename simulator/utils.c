#include "utils.h"
#include <stdio.h>

void print_board(unsigned player1, unsigned player2) {
  for (int row = 0; row < BOARD_SIZE; row++) {

    for (int col = 0; col < BOARD_SIZE; col++)
      printf("+---");
    printf("+\n");

    for (int col = 0; col < BOARD_SIZE; col++) {
      printf("|");
      if ((row ^ col) & 1) {
        if (player1 & 1)
          printf(" x ");
        else if (player2 & 1)
          printf(" o ");
        else
          printf("   ");
        player1 >>= 1;
        player2 >>= 1;
      } else
        printf("   ");
    }
    printf("|\n");
  }
  for (int col = 0; col < BOARD_SIZE; col++)
    printf("+---");
  printf("+\n");
}

int can_move_left(int pos, unsigned board, int direction) {
  int row = pos / ROW_SIZE;
  int col = pos % ROW_SIZE;
  if (direction == 0) {
    if (row == BOARD_SIZE - 1 || (row % 2 == 0 && col == ROW_SIZE - 1))
      return 0;
    pos += (row % 2 == 0) ? ROW_SIZE + 1 : ROW_SIZE;
  } else {
    if (row == 0 || (row % 2 == 1 && col == 0))
      return 0;
    pos -= (row % 2 == 0) ? ROW_SIZE : ROW_SIZE + 1;
  }
  return !(board & (1 << pos));
}

int can_move_right(int pos, unsigned board, int direction) {
  int row = pos / ROW_SIZE;
  int col = pos % ROW_SIZE;
  if (direction == 0) {
    if (row == BOARD_SIZE - 1 || (row % 2 == 1 && col == 0))
      return 0;
    pos += (row % 2 == 0) ? ROW_SIZE : ROW_SIZE - 1;
  } else {
    if (row == 0 || (row % 2 == 0 && col == ROW_SIZE - 1))
      return 0;
    pos -= (row % 2 == 0) ? ROW_SIZE - 1 : ROW_SIZE;
  }
  return !(board & (1 << pos));
}

int can_take_left(int pos, unsigned player, unsigned opponent, int direction) {
  int row = pos / ROW_SIZE;
  int col = pos % ROW_SIZE;
  if (direction == 0) {
    if (row == BOARD_SIZE - 1 || (row % 2 == 0 && col == ROW_SIZE - 1))
      return 0;
    pos += (row % 2 == 0) ? ROW_SIZE + 1 : ROW_SIZE;
  } else {
    if (row == 0 || (row % 2 == 1 && col == 0))
      return 0;
    pos -= (row % 2 == 0) ? ROW_SIZE : ROW_SIZE + 1;
  }
  return (opponent & (1 << pos)) &&
         can_move_left(pos, player | opponent, direction);
}

int can_take_right(int pos, unsigned player, unsigned opponent, int direction) {
  int row = pos / ROW_SIZE;
  int col = pos % ROW_SIZE;
  if (direction == 0) {
    if (row == BOARD_SIZE - 1 || (row % 2 == 1 && col == 0))
      return 0;
    pos += (row % 2 == 0) ? ROW_SIZE : ROW_SIZE - 1;
  } else {
    if (row == 0 || (row % 2 == 0 && col == ROW_SIZE - 1))
      return 0;
    pos -= (row % 2 == 0) ? ROW_SIZE - 1 : ROW_SIZE;
  }
  return (opponent & (1 << pos)) &&
         can_move_right(pos, player | opponent, direction);
}

int can_take(int pos, unsigned player, unsigned opponent) {
  return can_take_left(pos, player, opponent, 0) ||
         can_take_left(pos, player, opponent, 1) ||
         can_take_right(pos, player, opponent, 0) ||
         can_take_right(pos, player, opponent, 1);
}

int can_move(int pos, unsigned player, unsigned opponent, int direction) {
  return can_move_left(pos, player | opponent, direction) ||
         can_move_right(pos, player | opponent, direction);
}

vector *potential_takes(unsigned player, unsigned opponent) {
  vector *takes = vector_create();
  for (int pos = 0; pos < BOARD_SIZE * ROW_SIZE; pos++)
    if (player & (1 << pos) && can_take(pos, player, opponent))
      vector_insert(takes, pos);
  return takes;
}

vector *potential_moves(unsigned player, unsigned opponent, int direction) {
  vector *moves = vector_create();
  for (int pos = 0; pos < BOARD_SIZE * ROW_SIZE; pos++)
    if (player & (1 << pos) && can_move(pos, player, opponent, direction))
      vector_insert(moves, pos);
  return moves;
}

int next_left(int pos, int dir) {
  if ((pos / ROW_SIZE) & 1)
    return pos + (dir ? -5 : 4);
  return pos + (dir ? -4 : 4);
}

int next_right(int pos, int dir) {
  if ((pos / ROW_SIZE) & 1)
    return pos + (dir ? -4 : 4);
  return pos + (dir ? -3 : 5);
}

int do_take_left(int pos, unsigned *p1, unsigned *p2, int dir) {
  int kill = next_left(pos, dir);
  int new_pos = next_left(kill, dir);
  *p1 ^= (1 << pos);
  *p1 |= (1 << new_pos);
  *p2 ^= (1 << kill);
  return new_pos;
}

int do_take_right(int pos, unsigned *p1, unsigned *p2, int dir) {
  int kill = next_right(pos, dir);
  int new_pos = next_right(kill, dir);
  *p1 ^= (1 << pos);
  *p1 |= (1 << new_pos);
  *p2 ^= (1 << kill);
  return new_pos;
}

int max2(int a, int b) { return (a >= b ? a : b); }

int max4(int a, int b, int c, int d) { return max2(max2(a, b), max2(c, d)); }

int count_max_takes(int pos, unsigned player, unsigned opponent) {
  int count1 = 0;
  int count2 = 0;
  int count3 = 0;
  int count4 = 0;
  if (can_take_left(pos, player, opponent, 0)) {
    unsigned new_player = player;
    unsigned new_opponent = opponent;
    do_take_left(pos, &new_player, &new_opponent, 0);
    count1 = count_max_takes(pos, new_player, new_opponent);
  }
  if (can_take_right(pos, player, opponent, 0)) {
    unsigned new_player = player;
    unsigned new_opponent = opponent;
    do_take_right(pos, &new_player, &new_opponent, 0);
    count2 = count_max_takes(pos, new_player, new_opponent);
  }
  if (can_take_left(pos, player, opponent, 1)) {
    unsigned new_player = player;
    unsigned new_opponent = opponent;
    do_take_left(pos, &new_player, &new_opponent, 1);
    count3 = count_max_takes(pos, new_player, new_opponent);
  }
  if (can_take_right(pos, player, opponent, 1)) {
    unsigned new_player = player;
    unsigned new_opponent = opponent;
    do_take_right(pos, &new_player, &new_opponent, 1);
    count4 = count_max_takes(pos, new_player, new_opponent);
  }
  return max4(count1, count2, count3, count4);
}

vector *max_takes(unsigned player, unsigned opponent) {
  vector *max_takes = vector_create();
  vector *takes = potential_takes(player, opponent);
  int max = 0;
  for (int i = 0; i < takes->len; i++) {
    int count = count_max_takes(takes->tab[i], player, opponent);
    if (count > max) {
      max = count;
      max_takes->len = 0;
      vector_insert(max_takes, takes->tab[i]);
    } else if (count == max) {
      vector_insert(max_takes, takes->tab[i]);
    }
  }
  return max_takes;
}