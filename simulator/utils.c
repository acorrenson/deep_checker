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
        if (player1 & 1) {
          printf("\033[1;36m");
          printf(" x ");
          printf("\033[0m");
        } else if (player2 & 1) {
          printf("\033[1;31m");
          printf(" x ");
          printf("\033[0m");
        } else
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
  unsigned pos_next = next_left(pos, direction);
  *player ^= (1 << pos) | (1 << pos_next);
  return pos_next;
}

unsigned do_move_right(unsigned pos, unsigned *player, int direction) {
  unsigned pos_next = next_right(pos, direction);
  *player ^= (1 << pos) | (1 << pos_next);
  return pos_next;
}

void move_choices(vector *player_choice, unsigned player, unsigned opponent,
                  int direction) {
  vector *pos_choice = potential_moves(player, opponent, direction);
  unsigned pos;
  unsigned player_next;
  for (int i = 0; i < pos_choice->len; i++) {
    pos = pos_choice->tab[i];
    if (can_move_left(pos, player | opponent, direction)) {
      player_next = player;
      do_move_left(pos, &player_next, direction);
      vector_insert(player_choice, player_next);
    }
    if (can_move_right(pos, player | opponent, direction)) {
      player_next = player;
      do_move_right(pos, &player_next, direction);
      vector_insert(player_choice, player_next);
    }
  }
  vector_delete(pos_choice);
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
  unsigned pos_next = next_left(take, direction);
  *player ^= (1 << pos) | (1 << pos_next);
  *opponent ^= (1 << take);
  return pos_next;
}

unsigned do_take_right(unsigned pos, unsigned *player, unsigned *opponent,
                       int direction) {
  unsigned take = next_right(pos, direction);
  unsigned pos_next = next_right(take, direction);
  *player ^= (1 << pos) | (1 << pos_next);
  *opponent ^= (1 << take);
  return pos_next;
}

int max2(int a, int b) { return (a >= b ? a : b); }

int max4(int a, int b, int c, int d) { return max2(max2(a, b), max2(c, d)); }

int count_max_takes(unsigned pos, unsigned player, unsigned opponent) {
  int count[4] = {0, 0, 0, 0};
  unsigned pos_next;
  unsigned player_next;
  unsigned opponent_next;
  if (can_take_left(pos, player, opponent, 0)) {
    player_next = player;
    opponent_next = opponent;
    pos_next = do_take_left(pos, &player_next, &opponent_next, 0);
    count[0] = 1 + count_max_takes(pos_next, player_next, opponent_next);
  }
  if (can_take_right(pos, player, opponent, 0)) {
    player_next = player;
    opponent_next = opponent;
    pos_next = do_take_right(pos, &player_next, &opponent_next, 0);
    count[1] = 1 + count_max_takes(pos_next, player_next, opponent_next);
  }
  if (can_take_left(pos, player, opponent, 1)) {
    player_next = player;
    opponent_next = opponent;
    pos_next = do_take_left(pos, &player_next, &opponent_next, 1);
    count[2] = 1 + count_max_takes(pos_next, player_next, opponent_next);
  }
  if (can_take_right(pos, player, opponent, 1)) {
    player_next = player;
    opponent_next = opponent;
    pos_next = do_take_right(pos, &player_next, &opponent_next, 1);
    count[3] = 1 + count_max_takes(pos_next, player_next, opponent_next);
  }
  return max4(count[0], count[1], count[2], count[3]);
}

vector *potential_max_takes(unsigned player, unsigned opponent) {
  vector *takes = potential_takes(player, opponent);
  vector *max_takes = vector_create();
  int max = 1;
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
  vector_delete(takes);
  return max_takes;
}

int pos_take_choices(vector *player_choice, vector *opponent_choice,
                     unsigned pos, unsigned player, unsigned opponent) {
  int count[4] = {0, 0, 0, 0};
  vector *p_choice[4];
  vector *o_choice[4];
  for (int i = 0; i < 4; i++) {
    p_choice[i] = vector_create();
    o_choice[i] = vector_create();
  }
  unsigned pos_next;
  unsigned player_next;
  unsigned opponent_next;
  if (can_take_left(pos, player, opponent, 0)) {
    player_next = player;
    opponent_next = opponent;
    pos_next = do_take_left(pos, &player_next, &opponent_next, 0);
    count[0] = 1 + pos_take_choices(p_choice[0], o_choice[0], pos_next,
                                    player_next, opponent_next);
  }
  if (can_take_right(pos, player, opponent, 0)) {
    player_next = player;
    opponent_next = opponent;
    pos_next = do_take_right(pos, &player_next, &opponent_next, 0);
    count[1] = 1 + pos_take_choices(p_choice[1], o_choice[1], pos_next,
                                    player_next, opponent_next);
  }
  if (can_take_left(pos, player, opponent, 1)) {
    player_next = player;
    opponent_next = opponent;
    pos_next = do_take_left(pos, &player_next, &opponent_next, 1);
    count[2] = 1 + pos_take_choices(p_choice[2], o_choice[2], pos_next,
                                    player_next, opponent_next);
  }
  if (can_take_right(pos, player, opponent, 1)) {
    player_next = player;
    opponent_next = opponent;
    pos_next = do_take_right(pos, &player_next, &opponent_next, 1);
    count[3] = 1 + pos_take_choices(p_choice[3], o_choice[3], pos_next,
                                    player_next, opponent_next);
  }

  int max_count = max4(count[0], count[1], count[2], count[3]);
  if (max_count > 0) {
    for (int i = 0; i < 4; i++) {
      if (count[i] == max_count) {
        for (int j = 0; j < p_choice[i]->len; j++) {
          vector_insert(player_choice, p_choice[i]->tab[j]);
          vector_insert(opponent_choice, o_choice[i]->tab[j]);
        }
      }
    }
  } else {
    vector_insert(player_choice, player);
    vector_insert(opponent_choice, opponent);
  }
  for (int i = 0; i < 4; i++) {
    vector_delete(p_choice[i]);
    vector_delete(o_choice[i]);
  }
  return max_count;
}

void take_choices(vector *player_choice, vector *opponent_choice,
                  unsigned player, unsigned opponent) {
  vector *pos_choice = potential_max_takes(player, opponent);
  for (int i = 0; i < pos_choice->len; i++)
    pos_take_choices(player_choice, opponent_choice, pos_choice->tab[i], player,
                     opponent);
  vector_delete(pos_choice);
};
