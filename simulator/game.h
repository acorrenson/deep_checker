#ifndef GAME_H
#define GAME_H

#include "utils.h"
#include "vector.h"

typedef void (*strategy)(unsigned *, unsigned *, int);

void play_random(unsigned *player, unsigned *opponent, int direction);

vector *generate_moves(vector *player_choice, unsigned player,
                       unsigned opponent, int direction);

vector *generate_takes(vector *player_choice, vector *opponent_choice,
                       unsigned player, unsigned opponent);

unsigned ask_program(vector *moves, char *cmd);

void play_program(unsigned *player, unsigned *opponent, int direction,
                  char *cmd);

void play_knn(unsigned *player, unsigned *opponent, int direction);

void play_mlp(unsigned *player, unsigned *opponent, int direction);

void play_match(strategy strat1, strategy strat2, vector *boards,
                int show_board);

#endif // GAME_H
