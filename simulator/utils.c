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

int next_left(int pos, int direction) {
  int odd_row = (pos / ROW_SIZE) & 1;
  if (direction == 0)
    pos += odd_row ? ROW_SIZE : ROW_SIZE + 1;
  else
    pos -= odd_row ? ROW_SIZE + 1 : ROW_SIZE;
  return pos;
}

int next_right(int pos, int direction) {
  int odd_row = (pos / ROW_SIZE) & 1;
  if (direction == 0)
    pos += odd_row ? ROW_SIZE - 1 : ROW_SIZE;
  else
    pos -= odd_row ? ROW_SIZE : ROW_SIZE - 1;
  return pos;
}

int can_move_left(int pos, unsigned board, int direction) {
  int row = pos / ROW_SIZE;
  int col = pos % ROW_SIZE;
  int odd_row = row & 1;
  if (direction == 0 &&
      (row == BOARD_SIZE - 1 || (!odd_row && col == ROW_SIZE - 1)))
    return 0;
  else if (row == 0 || (odd_row && col == 0))
    return 0;
  return !(board & (1 << next_left(pos, direction)));
}

int can_move_right(int pos, unsigned board, int direction) {
  int row = pos / ROW_SIZE;
  int col = pos % ROW_SIZE;
  int odd_row = row & 1;
  if (direction == 0 && (row == BOARD_SIZE - 1 || (odd_row && col == 0)))
    return 0;
  else if (row == 0 || (!odd_row && col == ROW_SIZE - 1))
    return 0;
  return !(board & (1 << next_right(pos, direction)));
}

int can_move(int pos, unsigned player, unsigned opponent, int direction) {
  return can_move_left(pos, player | opponent, direction) ||
         can_move_right(pos, player | opponent, direction);
}

vector *potential_moves(unsigned player, unsigned opponent, int direction) {
  vector *moves = vector_create();
  for (int pos = 0; pos < BOARD_SIZE * ROW_SIZE; pos++)
    if (player & (1 << pos) && can_move(pos, player, opponent, direction))
      vector_insert(moves, pos);
  return moves;
}

int can_take_left(int pos, unsigned player, unsigned opponent, int direction) {
  int row = pos / ROW_SIZE;
  int col = pos % ROW_SIZE;
  int odd_row = row & 1;
  if (direction == 0 &&
      (row == BOARD_SIZE - 1 || (!odd_row && col == ROW_SIZE - 1)))
    return 0;
  else if (row == 0 || (odd_row && col == 0))
    return 0;
  pos = next_left(pos, direction);
  return (opponent & (1 << pos)) &&
         can_move_left(pos, player | opponent, direction);
}

int can_take_right(int pos, unsigned player, unsigned opponent, int direction) {
  int row = pos / ROW_SIZE;
  int col = pos % ROW_SIZE;
  int odd_row = row & 1;
  if (direction == 0 && (row == BOARD_SIZE - 1 || (odd_row && col == 0)))
    return 0;
  else if (row == 0 || (!odd_row && col == ROW_SIZE - 1))
    return 0;
  pos = next_right(pos, direction);
  return (opponent & (1 << pos)) &&
         can_move_right(pos, player | opponent, direction);
}

int can_take(int pos, unsigned player, unsigned opponent) {
  return can_take_left(pos, player, opponent, 0) ||
         can_take_left(pos, player, opponent, 1) ||
         can_take_right(pos, player, opponent, 0) ||
         can_take_right(pos, player, opponent, 1);
}

vector *potential_takes(unsigned player, unsigned opponent) {
  vector *takes = vector_create();
  for (int pos = 0; pos < BOARD_SIZE * ROW_SIZE; pos++)
    if (player & (1 << pos) && can_take(pos, player, opponent))
      vector_insert(takes, pos);
  return takes;
}

int do_take_left(int pos, unsigned *player, unsigned *opponent, int direction) {
  int take = next_left(pos, direction);
  int new_pos = next_left(take, direction);
  *player ^= (1 << pos) | (1 << new_pos);
  *opponent ^= (1 << take);
  return new_pos;
}

int do_take_right(int pos, unsigned *player, unsigned *opponent,
                  int direction) {
  int take = next_right(pos, direction);
  int new_pos = next_right(take, direction);
  *player ^= (1 << pos) | (1 << new_pos);
  *opponent ^= (1 << take);
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
    count1 = 1 + count_max_takes(pos, new_player, new_opponent);
  }
  if (can_take_right(pos, player, opponent, 0)) {
    unsigned new_player = player;
    unsigned new_opponent = opponent;
    do_take_right(pos, &new_player, &new_opponent, 0);
    count2 = 1 + count_max_takes(pos, new_player, new_opponent);
  }
  if (can_take_left(pos, player, opponent, 1)) {
    unsigned new_player = player;
    unsigned new_opponent = opponent;
    do_take_left(pos, &new_player, &new_opponent, 1);
    count3 = 1 + count_max_takes(pos, new_player, new_opponent);
  }
  if (can_take_right(pos, player, opponent, 1)) {
    unsigned new_player = player;
    unsigned new_opponent = opponent;
    do_take_right(pos, &new_player, &new_opponent, 1);
    count4 = 1 + count_max_takes(pos, new_player, new_opponent);
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

int do_max_takes(int *pos, unsigned *player, unsigned *opponent) {
  int count1 = 0;
  int count2 = 0;
  int count3 = 0;
  int count4 = 0;
  int pos1 = *pos;
  int pos2 = *pos;
  int pos3 = *pos;
  int pos4 = *pos;
  unsigned player1 = *player;
  unsigned player2 = *player;
  unsigned player3 = *player;
  unsigned player4 = *player;
  unsigned opponent1 = *opponent;
  unsigned opponent2 = *opponent;
  unsigned opponent3 = *opponent;
  unsigned opponent4 = *opponent;

  if (can_take_left(pos1, player1, opponent1, 0)) {
    pos1 = do_take_left(pos1, &player1, &opponent1, 0);
    count1 = 1 + do_max_takes(&pos1, &player1, &opponent1);
  }
  if (can_take_right(pos2, player2, opponent3, 0)) {
    pos2 = do_take_right(pos2, &player2, &opponent2, 0);
    count2 = 1 + do_max_takes(&pos2, &player2, &opponent2);
  }
  if (can_take_left(pos3, player3, opponent3, 1)) {
    pos3 = do_take_left(pos3, &player3, &opponent3, 1);
    count3 = 1 + do_max_takes(&pos3, &player3, &opponent3);
  }
  if (can_take_right(pos4, player4, opponent4, 1)) {
    pos4 = do_take_right(pos4, &player4, &opponent4, 1);
    count4 = 1 + do_max_takes(&pos4, &player4, &opponent4);
  }

  if (count1 > count2) {
    if (count3 > count4) {
      if (count1 > count3) {
        *pos = pos1;
        *player = player1;
        *opponent = opponent1;
        return count1;
      } else {
        *pos = pos3;
        *player = player3;
        *opponent = opponent3;
        return count3;
      }
    } else {
      if (count1 > count4) {
        *pos = pos1;
        *player = player1;
        *opponent = opponent1;
        return count1;
      } else {
        *pos = pos4;
        *player = player4;
        *opponent = opponent4;
        return count4;
      }
    }
  } else {
    if (count3 > count4) {
      if (count2 > count3) {
        *pos = pos2;
        *player = player2;
        *opponent = opponent2;
        return count2;
      } else {
        *pos = pos3;
        *player = player3;
        *opponent = opponent3;
        return count3;
      }
    } else {
      if (count2 > count4) {
        *pos = pos2;
        *player = player2;
        *opponent = opponent2;
        return count2;
      } else {
        *pos = pos4;
        *player = player4;
        *opponent = opponent4;
        return count4;
      }
    }
  }
}
