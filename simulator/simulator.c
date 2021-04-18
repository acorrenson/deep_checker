#include "utils.h"
#include "vector.h"
#include <stdio.h>

void play_turn(unsigned *player, unsigned *opponent, int direction) {
  vector *candidates = max_takes(*player, *opponent);
  if (candidates->len > 0) {
    // TODO : choose i randomly
    int i = 0;
    unsigned pos = candidates->tab[i];
    do_max_takes(&pos, player, opponent);
  } else {
    vector_delete(candidates);
    candidates = potential_moves(*player, *opponent, direction);
    if (candidates->len > 0) {
      // TODO : choose i randomly
      int i = 0;
      unsigned pos = candidates->tab[i];
      // /!\ PAS ALEATOIRE
      if (can_move_left(pos, *player | *opponent, direction))
        do_move_left(pos, player, direction);
      else
        do_move_right(pos, player, direction);
    } else
      *player = 0;
  }
  vector_delete(candidates);
}

void play_match(int show_board) {
  unsigned player1 = 0b11111111111100000000000000000000;
  unsigned player2 = 0b00000000000000000000111111111111;
  int turn = 1;
  while (player1 && player2) {
    if (turn & 1)
      play_turn(&player1, &player2, 1);
    else
      play_turn(&player2, &player1, 0);
    if (show_board)
      print_board(player1, player2);
    turn++;
  }
}

int main() {
  play_match(1);
  return 0;
}
