CC = gcc
CFLAGS = -I. -Wall -Wextra

DEPS = deck.h
OBJ = main.o deckfunc.o

# Compile .c to .o
%.o: %.c $(DEPS)
	$(CC) -g -c -o $@ $< $(CFLAGS)

# Link object files into executable
main: $(OBJ)
	$(CC) -g -o $@ $^ $(CFLAGS)

# Clean up build artifacts
.PHONY: clean
clean:
	rm -f *.o deck
