#include "game.h"
#include "utils.h"
#include "vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void play_random(unsigned *player, unsigned *opponent, int direction) {
  vector *player_choice = vector_create();
  vector *opponent_choice = vector_create();
  take_choices(player_choice, opponent_choice, *player, *opponent);
  if (player_choice->len > 0) {
    // if we can take
    int i = rand() % player_choice->len;
    *player = player_choice->tab[i];
    *opponent = opponent_choice->tab[i];
  } else {
    move_choices(player_choice, *player, *opponent, direction);
    if (player_choice->len > 0) {
      // if we can move
      int i = rand() % player_choice->len;
      *player = player_choice->tab[i];
    } else {
      *player = 0;
    }
  }
  vector_delete(player_choice);
  vector_delete(opponent_choice);
}

vector *generate_moves(vector *player_choice, unsigned player,
                       unsigned opponent, int direction) {
  vector *moves = vector_create();
  vector_insert(moves, player);
  vector_insert(moves, opponent);
  for (int i = 0; i < player_choice->len; i++) {
    vector_insert(moves, player_choice->tab[i]);
    vector_insert(moves, opponent);
  }
  return moves;
}

vector *generate_takes(vector *player_choice, vector *opponent_choice,
                       unsigned player, unsigned opponent) {
  vector *moves = vector_create();
  vector_insert(moves, player);
  vector_insert(moves, opponent);
  for (int i = 0; i < player_choice->len; i++) {
    vector_insert(moves, player_choice->tab[i]);
    vector_insert(moves, opponent_choice->tab[i]);
  }
  return moves;
}

unsigned ask_program(vector *moves, char *cmd) {
  // building command line
  size_t cmd_len = strlen(cmd);
  size_t buff_size = cmd_len + moves->len * 9 * 4;

  char *args = malloc(buff_size + 1);
  memcpy(args, cmd, cmd_len);
  args[cmd_len] = ' ';

  unsigned player = moves->tab[0];
  unsigned opponent = moves->tab[1];
  unsigned player_next;
  unsigned opponent_next;

  // adding args
  char *pos = args + cmd_len + 1;
  pos += sprintf(pos, "%x %x ", player, opponent);
  for (int i = 2; i < moves->len; i += 2) {
    player_next = moves->tab[i];
    opponent_next = moves->tab[i + 1];
    pos += sprintf(pos, "%x %x ", moves->tab[i], moves->tab[i + 1]);
  }
  *pos = '\0';
  printf("cmd : %s\n", args);

  // executing the command
  FILE *fd = popen(args, "r");
  if (fd == NULL) {
    fprintf(stderr, "Invalid command");
    exit(1);
  }
  int i = 0;
  fscanf(fd, "%d", &i);
  pclose(fd);

  return i;
}

void play_program(unsigned *player, unsigned *opponent, int direction,
                  char *cmd) {
  vector *player_choice = vector_create();
  vector *opponent_choice = vector_create();
  take_choices(player_choice, opponent_choice, *player, *opponent);
  if (player_choice->len > 0) {
    // if we can take
    vector *takes =
        generate_takes(player_choice, opponent_choice, *player, *opponent);
    int i = ask_program(takes, cmd);
    vector_delete(takes);
    *player = player_choice->tab[i];
    *opponent = opponent_choice->tab[i];
  } else {
    move_choices(player_choice, *player, *opponent, direction);
    if (player_choice->len > 0) {
      // if we can move
      vector *moves =
          generate_moves(player_choice, *player, *opponent, direction);
      int i = ask_program(moves, cmd);
      vector_delete(moves);
      *player = player_choice->tab[i];
    } else {
      *player = 0;
    }
  }
  vector_delete(player_choice);
  vector_delete(opponent_choice);
}

void play_knn(unsigned *player, unsigned *opponent, int direction) {
  char *cmd = "python3 knn.py";
  play_program(player, opponent, direction, cmd);
}

void play_mlp(unsigned *player, unsigned *opponent, int direction) {
  char *cmd = "python3 mlp.py";
  play_program(player, opponent, direction, cmd);
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
