#include "utils.h"
#include "vector.h"
#include <stdio.h>

int main() {
  // unsigned player1 = 0b00000000000001100000111111111111;
  // unsigned player2 = 0b11111111111100000110000000000000;
  unsigned player1 = 0b01000000000000000000000000000000;
  unsigned player2 = 0b00000100000001000000000000000000;
  print_board(player1, player2);
  printf("x can take (right) o ? %d\n",
         can_take_right(30, player1, player2, 1));
  int pos = do_take_right(30, &player1, &player2, 1);
  print_board(player1, player2);
  printf("x can take (left) o ? %d\n", can_take_left(pos, player1, player2, 1));
  do_take_left(pos, &player1, &player2, 1);
  print_board(player1, player2);
  return 0;
}
