#include "game.h"
#include "utils.h"
#include "vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void play_random(unsigned *player, unsigned *opponent, int direction) {
  vector *player_choice = vector_create();
  vector_insert(player_choice, *player);
  vector *opponent_choice = vector_create();
  vector_insert(opponent_choice, *opponent);
  vector *candidates = potential_max_takes(*player, *opponent);
  if (candidates->len > 0) {
    int i = rand() % candidates->len;
    unsigned pos = candidates->tab[i];
    do_max_takes(&pos, player, opponent);
  } else {
    move_choices(player_choice, *opponent, direction);
    if (player_choice->len > 1) {
      int i = 1 + (rand() % (player_choice->len - 1));
      *player = player_choice->tab[i];
    } else {
      *player = 0;
    }
  }
  vector_delete(candidates);
  vector_delete(player_choice);
  vector_delete(opponent_choice);
}

vector *generate_takes(vector *candidates, unsigned player, unsigned opponent) {
  vector *moves = vector_create();
  unsigned player_next;
  unsigned opponent_next;
  vector_insert(moves, player);
  vector_insert(moves, opponent);
  for (int i = 0; i < candidates->len; i++) {
    unsigned pos = candidates->tab[i];
    if (can_take_left(pos, player, opponent, 0)) {
      player_next = player;
      opponent_next = opponent;
      do_take_left(pos, &player_next, &opponent_next, 0);
      vector_insert(moves, player_next);
      vector_insert(moves, opponent_next);
    }
    if (can_take_right(pos, player, opponent, 0)) {
      player_next = player;
      opponent_next = opponent;
      do_take_right(pos, &player_next, &opponent_next, 0);
      vector_insert(moves, player_next);
      vector_insert(moves, opponent_next);
    }
    if (can_take_left(pos, player, opponent, 1)) {
      player_next = player;
      opponent_next = opponent;
      do_take_left(pos, &player_next, &opponent_next, 1);
      vector_insert(moves, player_next);
      vector_insert(moves, opponent_next);
    }
    if (can_take_right(pos, player, opponent, 1)) {
      player_next = player;
      opponent_next = opponent;
      do_take_right(pos, &player_next, &opponent_next, 1);
      vector_insert(moves, player_next);
      vector_insert(moves, opponent_next);
    }
  }
  return moves;
}

vector *generate_moves(vector *candidates, unsigned player, unsigned opponent,
                       int dir) {
  vector *moves = vector_create();
  unsigned player_next;
  unsigned opponent_next;
  vector_insert(moves, player);
  vector_insert(moves, opponent);
  for (int i = 0; i < candidates->len; i++) {
    if (can_move_left(candidates->tab[i], player | opponent, dir)) {
      player_next = player;
      opponent_next = opponent;
      do_move_left(candidates->tab[i], &player_next, dir);
      vector_insert(moves, player_next);
      vector_insert(moves, opponent_next);
    }
    if (can_move_right(candidates->tab[i], player | opponent, dir)) {
      player_next = player;
      opponent_next = opponent;
      do_move_right(candidates->tab[i], &player_next, dir);
      vector_insert(moves, player_next);
      vector_insert(moves, opponent_next);
    }
  }
  return moves;
}

unsigned ask_knn(vector *moves) {
  // building command line
  char *cmd = "knn.py";
  size_t cmd_len = strlen(cmd);
  size_t buff_size = cmd_len + moves->len * 9 * 4;
  char *args = malloc(buff_size + 1);
  memcpy(args, cmd, cmd_len);
  args[cmd_len] = ' ';

  unsigned player = moves->tab[0];
  unsigned opponent = moves->tab[1];

  // adding args
  char *pos = args + cmd_len + 1;
  for (int i = 2; i < moves->len; i += 2) {
    printf("candidate : %x %x -> %x %x\n", player, opponent, moves->tab[i],
           moves->tab[i + 1]);
    pos += sprintf(pos, "%x %x %x %x ", player, opponent, moves->tab[i],
                   moves->tab[i + 1]);
  }

  // executing the command
  *pos = '\0';
  printf("cmd : %s\n", args);
  return 0;
}

void play_knn(unsigned *player, unsigned *opponent, int direction) {
  vector *candidates = potential_max_takes(*player, *opponent);
  vector *takes = generate_takes(candidates, *player, *opponent);
  if (candidates->len > 0) {
    ask_knn(takes);
    vector_delete(takes);
    unsigned pos = candidates->tab[0];
    if (candidates->len > 1)
      // TODO : choose with KNN
      pos = candidates->tab[0];
    do_max_takes(&pos, player, opponent);
  } else {
    vector_delete(candidates);
    candidates = potential_moves(*player, *opponent, direction);
    vector *moves = generate_moves(candidates, *player, *opponent, direction);
    if (candidates->len > 0) {
      ask_knn(moves);
      vector_delete(moves);
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
