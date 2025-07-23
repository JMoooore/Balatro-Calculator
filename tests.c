/* tests.c – minimal unit tests for the poker engine */
#include "deck.h"
#include <assert.h>
#include <stdlib.h>

/* ---------- tiny helpers ---------- */

/* Build a 5-card hand (or fewer—see makeN below) from parallel arrays. */
static Hand *make5(const enum Rank r[5], const enum Suit s[5]) {
  Hand *h = createHand(5);
  for (int i = 0; i < 5; ++i) {
    h->cards[i].rank = r[i];
    h->cards[i].suit = s[i];
  }
  return h;
}

/* Build an N-card hand (N ≤ 5) so we can test short straights/flushes. */
// static Hand *makeN(int n, const enum Rank *r, const enum Suit *s) {
//   Hand *h = createHand(n);
//   for (int i = 0; i < n; ++i) {
//     h->cards[i].rank = r[i];
//     h->cards[i].suit = s[i];
//   }
//   return h;
// }

/* Convenience macro so we don’t leak every hand in the test runner. */
#define WITH_HAND(ranks, suits, code)                                          \
  do {                                                                         \
    Hand *h = make5(ranks, suits);                                             \
    {code} free(h);                                                            \
  } while (0)

/* Handy variant for the variable-length helper. */
// #define WITH_NHAND(n, ranks, suits, code)                                      \
//   do {                                                                         \
//     Hand *h = makeN(n, ranks, suits);                                          \
//     {code} free(h);                                                            \
//   } while (0)
//
/* ---------- original tests (unchanged) ---------- */

static void test_label_helpers(void) {
  assert(labelToSuit('s') == SPADES);
  assert(labelToSuit('d') == DIAMONDS);
  assert(labelToRank("a") == ACE);
  assert(labelToRank("10") == TEN);
}

static void test_flush_and_straight(void) {
  /* ♥ 2-3-4-5-9  (flush only) */
  const enum Rank rf[5] = {TWO, THREE, FOUR, FIVE, NINE};
  const enum Suit sf[5] = {HEARTS, HEARTS, HEARTS, HEARTS, HEARTS};
  WITH_HAND(rf, sf, {
    assert(isFlush(h) == true);
    assert(isStraight(h) == false);
  });

  /* Mixed 5-6-7-8-9  (straight only) */
  const enum Rank rs[5] = {FIVE, SIX, SEVEN, EIGHT, NINE};
  const enum Suit ss[5] = {CLUBS, DIAMONDS, HEARTS, SPADES, CLUBS};
  WITH_HAND(rs, ss, {
    assert(isFlush(h) == false);
    assert(isStraight(h) == true);
  });

  /* 10-J-Q-K-A all ♠ (royal flush) */
  const enum Rank rr[5] = {TEN, JACK, QUEEN, KING, ACE};
  const enum Suit sr[5] = {SPADES, SPADES, SPADES, SPADES, SPADES};
  WITH_HAND(rr, sr, {
    assert(isFlush(h) == true);
    assert(isStraight(h) == true);
  });
}

static void test_scoring(void) {
  /* Four of a kind: 9-9-9-9-K */
  const enum Rank r4[5] = {NINE, NINE, NINE, NINE, KING};
  const enum Suit s4[5] = {CLUBS, DIAMONDS, HEARTS, SPADES, CLUBS};
  WITH_HAND(r4, s4, { assert(findBaseHand(h) == FOUR_OF_A_KIND); });

  /* Full house: Q-Q-Q-7-7 */
  const enum Rank rfh[5] = {QUEEN, QUEEN, QUEEN, SEVEN, SEVEN};
  const enum Suit sfh[5] = {CLUBS, DIAMONDS, HEARTS, SPADES, CLUBS};
  WITH_HAND(rfh, sfh, { assert(findBaseHand(h) == FULL_HOUSE); });

  /* Pair only: A-A-3-8-J */
  const enum Rank rp[5] = {ACE, ACE, THREE, EIGHT, JACK};
  const enum Suit sp[5] = {CLUBS, DIAMONDS, HEARTS, SPADES, CLUBS};
  WITH_HAND(rp, sp, { assert(findBaseHand(h) == PAIR); });
}

/* ---------- NEW edge-case tests ---------- */

static void test_edge_straights(void) {
  /* A-2-3-4-5 (wheel) */
  const enum Rank rw[5] = {ACE, TWO, THREE, FOUR, FIVE};
  const enum Suit sw[5] = {CLUBS, DIAMONDS, HEARTS, SPADES, CLUBS};
  WITH_HAND(rw, sw, { assert(isStraight(h) == true); });

  /* Duplicate ranks 2-2-3-4-5 should NOT be straight */
  const enum Rank rd[5] = {TWO, TWO, THREE, FOUR, FIVE};
  const enum Suit sd[5] = {CLUBS, DIAMONDS, HEARTS, SPADES, CLUBS};
  WITH_HAND(rd, sd, { assert(isStraight(h) == false); });

  /* Non-consecutive gap 3-4-6-7-8 should fail */
  const enum Rank rg[5] = {THREE, FOUR, SIX, SEVEN, EIGHT};
  const enum Suit sg[5] = {CLUBS, DIAMONDS, HEARTS, SPADES, CLUBS};
  WITH_HAND(rg, sg, { assert(isStraight(h) == false); });
}

static void test_short_variants(void) {
#ifdef STRAIGHT_NEED
#if STRAIGHT_NEED == 4
  /* 4-card straight 6-7-8-9 (clubs) */
  const enum Rank r4s[4] = {SIX, SEVEN, EIGHT, NINE};
  const enum Suit s4s[4] = {CLUBS, CLUBS, CLUBS, CLUBS};
  WITH_NHAND(4, r4s, s4s, {
    assert(isStraight(h) == true);
    /* Also flush if FLUSH_NEED <= 4 */
#ifdef FLUSH_NEED
    assert(isFlush(h) == (FLUSH_NEED <= 4));
#endif
  });
#endif /* STRAIGHT_NEED == 4 */
#endif /* STRAIGHT_NEED */
}

/* ---------- runner ---------- */

int main(void) {
  test_label_helpers();
  test_flush_and_straight();
  test_scoring();
  test_edge_straights();
  test_short_variants(); /* no-op if macros not set */
  return 0;              /* silence is success */
}
