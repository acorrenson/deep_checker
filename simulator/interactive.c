#include "game.h"
#include "vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char **argv) {
  int runs = 1;
  if (argc > 1)
    runs = atoi(argv[1]);
  srand(time(NULL));
  vector *boards = vector_create();

  // unsigned player1 = 0b11111111111100000000000000000000;
  // unsigned player2 = 0b00000000000000000000111111111111;
  // ask_knn(potential_moves(player1, player2, 1));

  play_match(play_random, play_knn, boards, 1);

  if (boards->tab[boards->len - 2])
    printf("Player 1 wins!\n");
  else
    printf("Player 2 wins!\n");

  vector_delete(boards);
  return 0;
}
