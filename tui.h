#ifndef TUI_H
#define TUI_H 1

#include <stdio.h>

#include <termios.h>

#include "con4.h"

#define EMPTY_STR "  "
#define PLAYER_X "██"
#define PLAYER_O "▒▒"

/* grabs keyboard keypress */
char getch();

int select_col(struct game *game, int gamer);

void clear_scr(void);

/* selected col is 0 indexed
 * if selected col is < 0 the selector isn't rendered */
void render_game(struct game *game, int selected_col, int gamer);

#endif

