#include "deck.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#define DEFAULTDECKSIZE 52
#define DEFAULTHANDSIZE 5
#define DEFAULTFLUSHSIZE 5
#define DEFAULTSTRAIGHTSIZE 5

const char suit_labels[] = {'c', 'd', 'h', 's'};

const char *rank_labels[] = {"INVALID", "ACE",  "TWO",   "THREE", "FOUR",
                             "FIVE",    "SIX",  "SEVEN", "EIGHT", "NINE",
                             "TEN",     "JACK", "QUEEN", "KING"};

const char *const scoring_hand_names[SCORING_HANDS_COUNT] = {
    "FLUSH FIVE",     "FLUSH HOUSE",     "FIVE OF A KIND", "ROYAL FLUSH",
    "STRAIGHT FLUSH", "FOUR OF A KIND",  "FULL HOUSE",     "FLUSH",
    "STRAIGHT",       "THREE OF A KIND", "TWO PAIR",       "PAIR",
    "HIGH CARD"};

const Score score_table[SCORING_HANDS_COUNT] = {
    [FLUSH_FIVE] = {160, 16},     [FLUSH_HOUSE] = {140, 14},
    [FIVE_OF_A_KIND] = {120, 12}, [ROYAL_FLUSH] = {100, 8},
    [STRAIGHT_FLUSH] = {100, 8},  [FOUR_OF_A_KIND] = {60, 7},
    [FULL_HOUSE] = {40, 4},       [FLUSH] = {35, 4},
    [STRAIGHT] = {30, 4},         [THREE_OF_A_KIND] = {30, 3},
    [TWO_PAIR] = {20, 2},         [PAIR] = {10, 2},
    [HIGH_CARD] = {5, 1}};

enum Suit labelToSuit(char label) {
  for (int i = 0; i < 4; i++) {
    if (label == suit_labels[i]) {
      return (enum Suit)i;
    }
  }
  printf("Invalid label passed to label to suit\n");
  return INVALID_SUIT;
}

char *suitToChar(enum Suit suit) {
  switch (suit) {
  case CLUBS:
    return "\u2663";
  case DIAMONDS:
    return "\u2662";
  case HEARTS:
    return "\u2661";
  case SPADES:
    return "\u2660";
  case INVALID_SUIT:
    printf("Invalid suit passed to suit to char\n");
    return "X";
  }
}

enum Rank labelToRank(char *label) {
  if (label[0] == 'a') {
    return (enum Rank)1;
  }
  if (label[0] == 'j') {
    return (enum Rank)11;
  }
  if (label[0] == 'q') {
    return (enum Rank)12;
  }
  if (label[0] == 'k') {
    return (enum Rank)13;
  }
  int i_label = atoi(label);
  if (i_label >= 1 && i_label <= 13) {
    return (enum Rank)i_label;
  }
  printf("Invalid label passed to label to rank\n");
  return INVALID_RANK;
}

const char *rankToChar(enum Rank rank) {
  if (rank >= ACE && rank <= KING) {
    return rank_labels[rank];
  }
  printf("Invalid rank passed to rank to char\n");
  return "-1";
}

bool isFlush(const Hand *h) {
  int suit_cnt[4] = {0};

  for (int i = 0; i < h->size; ++i) {
    enum Suit s = h->cards[i].suit;
    if (s >= CLUBS && s <= SPADES)
      ++suit_cnt[s];
    else
      exit(EXIT_FAILURE);
  }

  for (int i = 0; i < 4; ++i) {
    if (suit_cnt[i] >= DEFAULTFLUSHSIZE)
      return true;
  }

  return false;
}

// static int compare_ranks(const void *a, const void *b) {
//   int ra = *(const enum Rank *)a;
//   int rb = *(const enum Rank *)b;
//   return (ra > rb) - (ra < rb);
// }

bool isStraight(const Hand *h) {
  bool seen[15] = {false};

  for (int i = 0; i < h->size; ++i) {
    enum Rank r = h->cards[i].rank;
    if (r < ACE || r > KING)
      exit(EXIT_FAILURE);
    seen[r] = true;
  }

  if (seen[ACE])
    seen[14] = true;

  int run = 0;
  for (int r = ACE; r <= 14; ++r) {
    if (seen[r]) {
      if (++run >= DEFAULTSTRAIGHTSIZE)
        return true;
    } else {
      run = 0;
    }
  }
  return false;
}

enum Scoring_Hands findBaseHand(Hand *h) {
  int max = 0, secondMax = 0;
  // Rank histogram
  int rank[15] = {0};
  const bool flush = isFlush(h);
  const bool straight = isStraight(h);

  // Count each rank of cards in hand and find og max
  for (int i = 0; i < h->size; i++)
    rank[h->cards[i].rank]++;

  for (int r = ACE; r <= KING; ++r) {
    if (rank[r] > max) {
      secondMax = max;
      max = rank[r];
    } else if (rank[r] > secondMax) {
      secondMax = rank[r];
    }
  }

  if (max >= 5 && flush) {
    return FLUSH_FIVE;
  }

  if (max >= 3 && secondMax >= 2 && flush) {
    return FLUSH_HOUSE;
  }

  if (max >= 5) {
    return FIVE_OF_A_KIND;
  }

  if (straight && flush && rank[ACE] != 0 && rank[KING] != 0) {
    return ROYAL_FLUSH;
  }

  if (straight && flush) {
    return STRAIGHT_FLUSH;
  }

  if (max >= 4) {
    return FOUR_OF_A_KIND;
  }

  if (max >= 3 && secondMax >= 2) {
    return FULL_HOUSE;
  }

  if (flush) {
    return FLUSH;
  }

  if (straight) {
    return STRAIGHT;
  }

  if (max >= 3) {
    return THREE_OF_A_KIND;
  }

  if (max >= 2) {
    return PAIR;
  }

  return HIGH_CARD;
}

int inputCards(Hand *player_hand) {
  printf("Input the cards in your hand:\n");

  for (int i = 0; i < player_hand->size; i++) {
    player_hand->cards[i] = *inputCardAndPrint();
  }
  return 0;
}

Hand *inputHandAndPrint(void) {
  printf("Input the cards in your hand:\n");
  Hand *h = createDefaultHand();

  for (int i = 0; i < h->size; i++) {
    h->cards[i] = *inputCardAndPrint();
  }
  return h;
}

Card *inputCardAndPrint(void) {
  char s_rank[3];
  char s_suit;
  int suit = INVALID_SUIT;
  int rank = INVALID_RANK;

  while (suit == INVALID_SUIT || rank == INVALID_RANK) {
    printf("Input a rank (e.g., A, 2-10, J, Q, K) and a suit (S, C, H, D):\n");
    if (scanf("%2s %c", s_rank, &s_suit) != 2) { // %2s to avoid overflow
      // Clear bad input
      int ch;
      while ((ch = getchar()) != '\n' && ch != EOF)
        ;
      printf("Invalid input. Try again.\n");
      continue;
    }
    // rank = labelToRank("a");
    // suit = labelToSuit('s');
    rank = labelToRank(s_rank);
    suit = labelToSuit(s_suit);
    printf("Rank %s\n", rankToChar(rank));
    printf("Suit %s\n", suitToChar(suit));
  }
  Card *c = malloc(sizeof(Card));
  if (!c) {
    perror("malloc");
    exit(EXIT_FAILURE);
  }
  c->rank = rank;
  c->suit = suit;

  return c;
}

Deck *createDeck(int size) {
  Deck *deck = malloc(sizeof(Deck) + size * sizeof(Card));
  if (!deck) {
    perror("malloc failed");
    exit(1);
  }
  deck->size = size;
  return deck;
}

Deck *createDefaultDeck(void) { return createDeck(DEFAULTDECKSIZE); }

Hand *createHand(int size) {
  Hand *hand = malloc(sizeof(Hand) + size * sizeof(Card));
  if (!hand) {
    perror("malloc failed");
    exit(1);
  }
  hand->size = size;
  return hand;
}

Hand *createDefaultHand(void) { return createHand(DEFAULTHANDSIZE); }
