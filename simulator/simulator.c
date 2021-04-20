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

  for (int i = 0; i < runs; i++) {
    play_match(play_random, play_random, boards, 0);
  }

  FILE *fd = fopen("boards.csv", "a");
  if (fd == NULL) {
    fprintf(stderr, "Invalid filename");
    exit(1);
  }
  for (int i = 0; i < boards->len; i += 2) {
    unsigned player1 = boards->tab[i];
    unsigned player2 = boards->tab[i + 1];
    fprintf(fd, "%x %x", player1, player2);
    if (player1 && player2)
      fprintf(fd, " ");
    else
      fprintf(fd, "\n");
  }
  fclose(fd);

  vector_delete(boards);
  return 0;
}
