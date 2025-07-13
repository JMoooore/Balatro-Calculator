#include "deck.h"
#include <stdio.h>

// int main(int argc, char *argv[]) {
//   // call a function in another file
//   myPrintHelloMake();
//
//   return (0);
// }

int main() {
  const char *scoring_hand_names[] = {
      "FLUSH_FIVE",      /* 0 */
      "FLUSH_HOUSE",     /* 1 */
      "FIVE_OF_A_KIND",  /* 2 */
      "ROYAL_FLUSH",     /* 3 */
      "STRAIGHT_FLUSH",  /* 4 */
      "FOUR_OF_A_KIND",  /* 5 */
      "FULL_HOUSE",      /* 6 */
      "FLUSH",           /* 7 */
      "STRAIGHT",        /* 8 */
      "THREE_OF_A_KIND", /* 9 */
      "TWO_PAIR",        /* 10 */
      "PAIR",            /* 11 */
      "HIGH_CARD"        /* 12 */
  };
  Hand *h = inputHandAndPrint();
  printf("Score: %d\n", scoreBaseHand(h));
  printf("Score: %s\n", scoring_hand_names[scoreBaseHand(h)]);
  return 0;
}
