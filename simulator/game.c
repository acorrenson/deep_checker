#include "game.h"
#include "utils.h"
#include "vector.h"
#include <stdio.h>
#include <stdlib.h>

void play_random(unsigned *player, unsigned *opponent, int direction) {
  vector *candidates = max_takes(*player, *opponent);
  if (candidates->len > 0) {
    int i = rand() % candidates->len;
    unsigned pos = candidates->tab[i];
    do_max_takes(&pos, player, opponent);
  } else {
    vector_delete(candidates);
    candidates = potential_moves(*player, *opponent, direction);
    if (candidates->len > 0) {
      int i = rand() % candidates->len;
      unsigned pos = candidates->tab[i];
      vector *move_choice = vector_create();
      if (can_move_left(pos, *player | *opponent, direction))
        vector_insert(move_choice, 0);
      if (can_move_right(pos, *player | *opponent, direction))
        vector_insert(move_choice, 1);
      int j = rand() % move_choice->len;
      if (move_choice->tab[j] == 0)
        do_move_left(pos, player, direction);
      else
        do_move_right(pos, player, direction);
      vector_delete(move_choice);
    } else {
      *player = 0;
    }
  }
  vector_delete(candidates);
}

void play_knn(unsigned *player, unsigned *opponent, int direction) {
  vector *candidates = max_takes(*player, *opponent);
  if (candidates->len > 0) {
    unsigned pos = candidates->tab[0];
    if (candidates->len > 1)
      // TODO : choose with KNN
      pos = candidates->tab[0];
    do_max_takes(&pos, player, opponent);
  } else {
    vector_delete(candidates);
    candidates = potential_moves(*player, *opponent, direction);
    if (candidates->len > 0) {
      unsigned pos = candidates->tab[0];
      if (candidates->len > 1)
        // TODO : choose with KNN
        pos = candidates->tab[0];
      vector *move_choice = vector_create();
      if (can_move_left(pos, *player | *opponent, direction))
        vector_insert(move_choice, 0);
      if (can_move_right(pos, *player | *opponent, direction))
        vector_insert(move_choice, 1);
      // TODO : choose with KNN
      int j = 0;
      if (move_choice->tab[j] == 0)
        do_move_left(pos, player, direction);
      else
        do_move_right(pos, player, direction);
      vector_delete(move_choice);
    } else {
      *player = 0;
    }
  }
  vector_delete(candidates);
}

void play_match(strategy strat1, strategy strat2, vector *boards,
                int show_board) {
  unsigned player1 = 0b11111111111100000000000000000000;
  unsigned player2 = 0b00000000000000000000111111111111;
  vector_insert(boards, player1);
  vector_insert(boards, player2);
  int turn = 1;
  while (player1 && player2) {
    if (turn & 1)
      (strat1)(&player1, &player2, 1);
    else
      (strat2)(&player2, &player1, 0);
    if (show_board)
      print_board(player1, player2);
    vector_insert(boards, player1);
    vector_insert(boards, player2);
    turn++;
  }
}
