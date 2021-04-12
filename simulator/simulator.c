#include "utils.h"
#include "vector.h"
#include <stdio.h>

int main() {
  unsigned player1 = 0b00000000000001100000111111111111;
  unsigned player2 = 0b11111111111100000110000000000000;
  print_board(player1 | player2);
  //   vector *v = candidates(player1, player2, 0);
  //   vector_print(v);
  //   vector_delete(v);
  //   v = candidates(player2, player1, 1);
  //   vector_print(v);
  //   vector_delete(v);
  return 0;
}
