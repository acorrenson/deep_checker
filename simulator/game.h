#ifndef GAME_H
#define GAME_H

#include "utils.h"
#include "vector.h"

typedef void (*strategy)(unsigned *, unsigned *, int);

vector *generate_moves(vector *moves, unsigned, unsigned, int);

vector *generate_takes(vector *moves, unsigned, unsigned);

unsigned ask_knn(vector *moves);

void play_random(unsigned *player, unsigned *opponent, int direction);

void play_knn(unsigned *player, unsigned *opponent, int direction);

void play_match(strategy strat1, strategy strat2, vector *boards,
                int show_board);

#endif // GAME_H
