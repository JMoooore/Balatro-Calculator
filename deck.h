#include <stdbool.h>
#include <stddef.h>

typedef struct Hand Hand;

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
  HIGH_CARD,
  SCORING_HANDS_COUNT
};

enum Rank {
  INVALID_RANK = 0,
  ACE,
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
  KING
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

struct Hand {
  int size;
  Card cards[];
};

typedef struct {
  int chips;
  int multiplier;
} Score;

enum Scoring_Hands findBaseHand(Hand *h);
int inputCards(Hand *h);
Hand *inputHandAndPrint(void);
Card *inputCardAndPrint(void);
Deck *createDeck(int size);
Deck *createDefaultDeck(void);
Hand *createHand(int size);
Hand *createDefaultHand(void);
enum Suit labelToSuit(char label);
char *suitToChar(enum Suit suit);
enum Rank labelToRank(char *label);
bool isStraight(const Hand *h);
bool isFlush(const Hand *h);
const char *rankToChar(enum Rank rank);
extern const char *const scoring_hand_names[SCORING_HANDS_COUNT];
extern const Score score_table[SCORING_HANDS_COUNT];
