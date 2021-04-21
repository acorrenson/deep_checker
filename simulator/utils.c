#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

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

unsigned next_left(unsigned pos, int direction) {
  int odd_row = (pos / ROW_SIZE) & 1;
  if (direction == 0)
    pos += odd_row ? ROW_SIZE : ROW_SIZE + 1;
  else
    pos -= odd_row ? ROW_SIZE + 1 : ROW_SIZE;
  return pos;
}

unsigned next_right(unsigned pos, int direction) {
  int odd_row = (pos / ROW_SIZE) & 1;
  if (direction == 0)
    pos += odd_row ? ROW_SIZE - 1 : ROW_SIZE;
  else
    pos -= odd_row ? ROW_SIZE : ROW_SIZE - 1;
  return pos;
}

int can_move_left(unsigned pos, unsigned board, int direction) {
  int row = pos / ROW_SIZE;
  int col = pos % ROW_SIZE;
  int odd_row = row & 1;
  if (direction == 0 &&
      (row == BOARD_SIZE - 1 || (!odd_row && col == ROW_SIZE - 1)))
    return 0;
  else if (direction == 1 && (row == 0 || (odd_row && col == 0)))
    return 0;
  return !((board >> next_left(pos, direction)) & 1);
}

int can_move_right(unsigned pos, unsigned board, int direction) {
  int row = pos / ROW_SIZE;
  int col = pos % ROW_SIZE;
  int odd_row = row & 1;
  if (direction == 0 && (row == BOARD_SIZE - 1 || (odd_row && col == 0)))
    return 0;
  else if (direction == 1 && (row == 0 || (!odd_row && col == ROW_SIZE - 1)))
    return 0;
  return !((board >> next_right(pos, direction)) & 1);
}

int can_move(unsigned pos, unsigned board, int direction) {
  return can_move_left(pos, board, direction) ||
         can_move_right(pos, board, direction);
}

vector *potential_moves(unsigned player, unsigned opponent, int direction) {
  vector *moves = vector_create();
  for (unsigned pos = 0; pos < BOARD_SIZE * ROW_SIZE; pos++)
    if (((player >> pos) & 1) && can_move(pos, player | opponent, direction))
      vector_insert(moves, pos);
  return moves;
}

unsigned do_move_left(unsigned pos, unsigned *player, int direction) {
  unsigned new_pos = next_left(pos, direction);
  *player ^= (1 << pos) | (1 << new_pos);
  return new_pos;
}

unsigned do_move_right(unsigned pos, unsigned *player, int direction) {
  unsigned new_pos = next_right(pos, direction);
  *player ^= (1 << pos) | (1 << new_pos);
  return new_pos;
}

void move_choices(vector *players, vector *opponents, int direction) {
  if (players->len != 1 || opponents->len != 1) {
    fprintf(stderr, "Invalid vector size");
    exit(1);
  }
  unsigned player = players->tab[0];
  unsigned opponent = opponents->tab[0];
  vector *pos_choice = potential_moves(player, opponent, direction);
  unsigned pos;
  unsigned player_next;
  for (int i = 0; i < pos_choice->len; i++) {
    pos = pos_choice->tab[i];
    if (can_move_left(pos, player | opponent, direction)) {
      player_next = player;
      do_move_left(pos, &player_next, direction);
      vector_insert(players, player_next);
      vector_insert(opponents, opponent);
    }
    if (can_move_right(pos, player | opponent, direction)) {
      player_next = player;
      do_move_right(pos, &player_next, direction);
      vector_insert(players, player_next);
      vector_insert(opponents, opponent);
    }
  }
};

int can_take_left(unsigned pos, unsigned player, unsigned opponent,
                  int direction) {
  int row = pos / ROW_SIZE;
  int col = pos % ROW_SIZE;
  int odd_row = row & 1;
  if (direction == 0 &&
      (row == BOARD_SIZE - 1 || (!odd_row && col == ROW_SIZE - 1)))
    return 0;
  else if (direction == 1 && (row == 0 || (odd_row && col == 0)))
    return 0;
  pos = next_left(pos, direction);
  return ((opponent >> pos) & 1) &&
         can_move_left(pos, player | opponent, direction);
}

int can_take_right(unsigned pos, unsigned player, unsigned opponent,
                   int direction) {
  int row = pos / ROW_SIZE;
  int col = pos % ROW_SIZE;
  int odd_row = row & 1;
  if (direction == 0 && (row == BOARD_SIZE - 1 || (odd_row && col == 0)))
    return 0;
  else if (direction == 1 && (row == 0 || (!odd_row && col == ROW_SIZE - 1)))
    return 0;
  pos = next_right(pos, direction);
  return ((opponent >> pos) & 1) &&
         can_move_right(pos, player | opponent, direction);
}

int can_take(unsigned pos, unsigned player, unsigned opponent) {
  return can_take_left(pos, player, opponent, 0) ||
         can_take_left(pos, player, opponent, 1) ||
         can_take_right(pos, player, opponent, 0) ||
         can_take_right(pos, player, opponent, 1);
}

vector *potential_takes(unsigned player, unsigned opponent) {
  vector *takes = vector_create();
  for (unsigned pos = 0; pos < BOARD_SIZE * ROW_SIZE; pos++)
    if (((player >> pos) & 1) && can_take(pos, player, opponent))
      vector_insert(takes, pos);
  return takes;
}

unsigned do_take_left(unsigned pos, unsigned *player, unsigned *opponent,
                      int direction) {
  unsigned take = next_left(pos, direction);
  unsigned new_pos = next_left(take, direction);
  *player ^= (1 << pos) | (1 << new_pos);
  *opponent ^= (1 << take);
  return new_pos;
}

unsigned do_take_right(unsigned pos, unsigned *player, unsigned *opponent,
                       int direction) {
  unsigned take = next_right(pos, direction);
  unsigned new_pos = next_right(take, direction);
  *player ^= (1 << pos) | (1 << new_pos);
  *opponent ^= (1 << take);
  return new_pos;
}

int max2(int a, int b) { return (a >= b ? a : b); }

int max4(int a, int b, int c, int d) { return max2(max2(a, b), max2(c, d)); }

int count_max_takes(unsigned pos, unsigned player, unsigned opponent) {
  int count[4] = {0, 0, 0, 0};
  if (can_take_left(pos, player, opponent, 0)) {
    unsigned new_player = player;
    unsigned new_opponent = opponent;
    do_take_left(pos, &new_player, &new_opponent, 0);
    count[0] = 1 + count_max_takes(pos, new_player, new_opponent);
  }
  if (can_take_right(pos, player, opponent, 0)) {
    unsigned new_player = player;
    unsigned new_opponent = opponent;
    do_take_right(pos, &new_player, &new_opponent, 0);
    count[1] = 1 + count_max_takes(pos, new_player, new_opponent);
  }
  if (can_take_left(pos, player, opponent, 1)) {
    unsigned new_player = player;
    unsigned new_opponent = opponent;
    do_take_left(pos, &new_player, &new_opponent, 1);
    count[2] = 1 + count_max_takes(pos, new_player, new_opponent);
  }
  if (can_take_right(pos, player, opponent, 1)) {
    unsigned new_player = player;
    unsigned new_opponent = opponent;
    do_take_right(pos, &new_player, &new_opponent, 1);
    count[3] = 1 + count_max_takes(pos, new_player, new_opponent);
  }
  return max4(count[0], count[1], count[2], count[3]);
}

vector *potential_max_takes(unsigned player, unsigned opponent) {
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

int do_max_takes(unsigned *pos, unsigned *player, unsigned *opponent) {
  int count[4] = {0, 0, 0, 0};
  unsigned pos_[4] = {*pos, *pos, *pos, *pos};
  unsigned player_[4] = {*player, *player, *player, *player};
  unsigned opponent_[4] = {*opponent, *opponent, *opponent, *opponent};

  if (can_take_left(*pos, *player, *opponent, 0)) {
    pos_[0] = do_take_left(*pos, player_, opponent_, 0);
    count[0] = 1 + do_max_takes(pos_, player_, opponent_);
  }
  if (can_take_right(*pos, *player, *opponent, 0)) {
    pos_[1] = do_take_right(*pos, player_ + 1, opponent_ + 1, 0);
    count[1] = 1 + do_max_takes(pos_ + 1, player_ + 1, opponent_ + 1);
  }
  if (can_take_left(*pos, *player, *opponent, 1)) {
    pos_[2] = do_take_left(*pos, player_ + 2, opponent_ + 2, 1);
    count[2] = 1 + do_max_takes(pos_ + 2, player_ + 2, opponent_ + 2);
  }
  if (can_take_right(*pos, *player, *opponent, 1)) {
    pos_[3] = do_take_right(*pos, player_ + 3, opponent_ + 3, 1);
    count[3] = 1 + do_max_takes(pos_ + 3, player_ + 3, opponent_ + 3);
  }

  int max_count = max4(count[0], count[1], count[2], count[3]);
  vector *take_choice = vector_create();
  for (int i = 0; i < 4; i++)
    if (count[i] == max_count)
      vector_insert(take_choice, i);
  int i = rand() % take_choice->len;
  int j = take_choice->tab[i];
  *pos = pos_[j];
  *player = player_[j];
  *opponent = opponent_[j];
  vector_delete(take_choice);

  return max_count;
}
