#include <stddef.h>

enum Scoring_Hands {
  FLUSH_FIVE,
  FLUSH_HOUSE,
  FIVE_OF_A_KIND,
  ROYAL_FLUSH,
  STRAIGHT_FLUSH,
  FOUR_OF_A_KIND,
  FULL_HOUSE,
  FLUSH,
  STRAIGHT,
  THREE_OF_A_KIND,
  TWO_PAIR,
  PAIR,
  HIGH_CARD
};

enum Rank {
  ACE = 1,
  TWO,
  THREE,
  FOUR,
  FIVE,
  SIX,
  SEVEN,
  EIGHT,
  NINE,
  TEN,
  JACK,
  QUEEN,
  KING,
  INVALID_RANK
};

enum Suit { CLUBS, DIAMONDS, HEARTS, SPADES, INVALID_SUIT };

typedef struct {
  enum Rank rank;
  enum Suit suit;
} Card;

typedef struct {
  int size;
  Card cards[];
} Deck;

typedef struct {
  int size;
  Card cards[];
} Hand;

typedef struct {
  int chips;
  int multiplier;
} Score;

int scoreBaseHand(Hand *h);
Hand *inputHandAndPrint(void);
Card *inputCardAndPrint(void);
Deck *createDeck(int size);
Deck *createDefaultDeck(void);
Hand *createHand(int size);
Hand *createDefaultHand(void);
