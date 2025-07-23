#define _GNU_SOURCE // for getline
#include "deck.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int (*cmd_fn)(int argc, char **argv);

struct command {
  const char *name;
  cmd_fn fn;
  const char *help;
};

Hand *player_hand;

/* ---------- command handlers ---------- */

int cmd_help(int argc, char **argv); // forward decl

// int cmd_echo(int argc, char **argv) {
//   for (int i = 0; i < argc; ++i)
//     printf("%s%s", argv[i], i == argc - 1 ? "\n" : " ");
//   return 0;
// }

int cmd_add(int argc, char **argv) {
  (void)argc;
  (void)argv; // unused
  inputCards(player_hand);
  return 0;
}

int cmd_quit(int argc, char **argv) {
  (void)argc;
  (void)argv; // unused
  return -1;  // signal main loop to exit
}

int cmd_print(int argc, char **argv) {
  (void)argc;
  (void)argv; // unused
  enum Scoring_Hands scoring_hand = findBaseHand(player_hand);
  Score score = score_table[scoring_hand];
  printf("%s ", scoring_hand_names[scoring_hand]);
  printf("%d * %d = %d\n", score.chips, score.multiplier,
         score.chips * score.multiplier);

  return 0; // signal main loop to exit
}
/* ---------- command table ---------- */

struct command commands[] = {
    {"help", cmd_help, "Show this help text"},
    // {"echo", cmd_echo, "Echo the arguments"},
    {"add", cmd_add, "Add cards to your hand"},
    {"quit", cmd_quit, "Exit the program"},
    {"print", cmd_print, "Print your best hand and score"},
    {NULL, NULL, NULL} // sentinel
};

int cmd_help(int argc, char **argv) {
  (void)argc;
  (void)argv;
  for (struct command *c = commands; c->name; ++c)
    printf("%-5s – %s\n", c->name, c->help);
  return 0;
}

/* ---------- main REPL ---------- */

int main(void) {
  char *line = NULL;
  size_t sz = 0;
  player_hand = createDefaultHand();

  puts("Balculator CLI – type 'help' for commands, 'quit' to exit");

  while (1) {
    printf("> ");
    fflush(stdout);

    ssize_t len = getline(&line, &sz, stdin);
    if (len == -1)
      break; // EOF / Ctrl-D
    if (len && line[len - 1] == '\n')
      line[len - 1] = '\0';

    /* tokenise */
    const int MAXTOK = 32;
    char *argv[MAXTOK];
    int argc = 0;
    char *tok = strtok(line, " ");
    while (tok && argc < MAXTOK) {
      argv[argc++] = tok;
      tok = strtok(NULL, " ");
    }
    if (argc == 0)
      continue; // blank line

    /* dispatch */
    struct command *cmd = NULL;
    for (struct command *c = commands; c->name; ++c)
      if (strcmp(c->name, argv[0]) == 0) {
        cmd = c;
        break;
      }

    if (!cmd) {
      printf("unknown command: %s (try 'help')\n", argv[0]);
      continue;
    }
    int status = cmd->fn(argc - 1, argv + 1);
    if (status == -1)
      break;
  }
  free(line);
  puts("Thanks for trying out the Balculator!");
  return 0;
}
