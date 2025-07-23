# ─────────────────────────────
#  build settings
# ─────────────────────────────
CC      = gcc
CFLAGS  = -std=c11 -Wall -Wextra -pedantic -g -I.

# source & object lists
SRC     := main.c deckfunc.c          # add more *.c files here
OBJ     := $(SRC:.c=.o)

TESTSRC := tests.c                    # <- the unit-tests file
TESTOBJ := $(TESTSRC:.c=.o)

DEPS    := deck.h                     # headers that every .c depends on

# ─────────────────────────────
#  default target
# ─────────────────────────────
.PHONY: all
all: main

# ─────────────────────────────
#  program
# ─────────────────────────────
main: $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

# ─────────────────────────────
#  tests  (make check)
# ─────────────────────────────
.PHONY: check
check: tests
	./tests               # <- exits non-zero if any assert fails

tests: $(TESTOBJ) $(filter-out main.o,$(OBJ))
	$(CC) $(CFLAGS) $^ -o $@

# ─────────────────────────────
#  pattern rules
# ─────────────────────────────
# Compile .c -> .o (depends on headers too)
%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@

# ─────────────────────────────
#  housekeeping
# ─────────────────────────────
.PHONY: clean
clean:
	$(RM) $(OBJ) $(TESTOBJ) main tests
