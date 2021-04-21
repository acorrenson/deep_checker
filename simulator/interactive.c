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

  play_match(play_knn, play_random, boards, 1);

  if (boards->tab[boards->len - 2])
    printf("Player 1 wins!\n");
  else
    printf("Player 2 wins!\n");

  vector_delete(boards);
  return 0;
}
