#include "deck.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#define DEFAULTDECKSIZE 52
#define DEFAULTHANDSIZE 5
#define DEFAULTFLUSHSIZE 5
#define DEFAULTSTRAIGHTSIZE 5

const char suit_labels[4] = {'c', 'h', 'd', 's'};
const char *rank_labels[] = {"INVALID", "ACE",  "TWO",   "THREE", "FOUR",
                             "FIVE",    "SIX",  "SEVEN", "EIGHT", "NINE",
                             "TEN",     "JACK", "QUEEN", "KING"};

enum Suit labelToSuit(char label) {
  int length = sizeof(suit_labels);
  for (int i = 0; i < length; i++) {
    if (label == suit_labels[i]) {
      return (enum Suit)i;
    }
  }
  printf("Invalid label passed to label to suit\n");
  return INVALID_SUIT;
}

char *suitToChar(enum Suit suit) {
  switch (suit) {
  case SPADES:
    return "\u2660";
  case CLUBS:
    return "\u2663";
  case DIAMONDS:
    return "\u2661";
  case HEARTS:
    return "\u2662";
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

bool isFlush(Hand *h) {
  int diamondCount = 5;
  int spadeCount = 5;
  int clubCount = 5;
  int heartCount = 5;

  for (int i = 0; i < h->size; i++) {
    Card c = h->cards[i];
    switch (c.suit) {
    case SPADES:
      spadeCount--;
      break;
    case CLUBS:
      clubCount--;
      break;
    case DIAMONDS:
      diamondCount--;
      break;
    case HEARTS:
      heartCount--;
      break;
    case INVALID_SUIT:
      exit(-1);
    }
  }

  if (diamondCount == 0 || spadeCount == 0 || clubCount == 0 ||
      heartCount == 0) {
    return true;
  }

  return false;
}

int compare_ranks(const void *a, const void *b) {
  enum Rank ra = *(const enum Rank *)a;
  enum Rank rb = *(const enum Rank *)b;
  return (ra - rb);
}

bool isStraight(Hand *h) {
  enum Rank ranks[h->size];

  for (int i = 0; i < h->size; i++) {
    ranks[i] = h->cards[i].rank;
  }

  qsort(ranks, h->size, sizeof(enum Rank), compare_ranks);

  if (ranks[0] == ACE && ranks[1] == TWO && ranks[2] == THREE &&
      ranks[3] == FOUR && ranks[4] == FIVE) {
    return true;
  }

  for (int i = 1; i < h->size; i++) {
    if (ranks[i] != ranks[i - 1] + 1) {
      return false;
    }
  }
  return true;
}

int scoreBaseHand(Hand *h) {
  int max = 0;
  int secondMax = 0;
  int rank[15] = {0};
  bool flush, straight;
  flush = isFlush(h);
  straight = isStraight(h);

  for (int i = 0; i < h->size; i++) {
    rank[h->cards[i].rank]++;
    if (rank[h->cards[i].rank] > max) {
      max = rank[h->cards[i].rank];
    }
  }

  for (int i = 0; i < 15; i++) {
    if (rank[i] > secondMax && rank[i] != max) {
      secondMax = rank[i];
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

  if (straight && h->cards[0].rank == 1 && h->cards[1].rank == 11) {
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

Hand *inputHandAndPrint(void) {
  printf("Welcome, input the cards in your hand:\n");
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
    printf("Input a rank and suit:\n");
    scanf(" %s %c", s_rank, &s_suit);
    // rank = labelToRank("a");
    // suit = labelToSuit('s');
    rank = labelToRank(s_rank);
    suit = labelToSuit(s_suit);
    printf("Rank %s\n", rankToChar(rank));
    printf("Suit %s\n", suitToChar(suit));
  }

  Card *c = malloc(sizeof(Card));
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

Deck *createDefaultDeck() { return createDeck(DEFAULTDECKSIZE); }

Hand *createHand(int size) {
  Hand *hand = malloc(sizeof(Hand) + size * sizeof(Card));
  if (!hand) {
    perror("malloc failed");
    exit(1);
  }
  hand->size = size;
  return hand;
}

Hand *createDefaultHand() { return createHand(DEFAULTHANDSIZE); }
