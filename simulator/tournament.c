#include "game.h"
#include "vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char **argv) {
  int runs = 10;
  if (argc > 1)
    runs = atoi(argv[1]);
  srand(time(NULL));
  vector *boards = vector_create();

  int win1 = 0;
  int win2 = 0;
  for (int i = 0; i < runs; i++) {
    boards->len = 0;
    play_match(play_random, play_random, boards, 0);
    if (boards->tab[boards->len - 2])
      win1++;
    else
      win2++;
  }

  printf("Player 1 : %d\nPlayer 2 : %d\n", win1, win2);

  vector_delete(boards);
  return 0;
}
