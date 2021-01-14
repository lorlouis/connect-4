#ifndef TUI_H
#define TUI_H 1
#if defined(_WIN32) || defined(WIN32) || defined(__CYGWIN__) || defined(__MINGW32__) || defined(__BORLANDC__)
#define OS_WIN
#endif

#include <stdio.h>


#include "con4.h"
#include "net.h"

#define EMPTY_STR "  "
#define PLAYER_X "><"
#define PLAYER_O "()"

#ifndef OS_WIN
/* grabs keyboard keypress */
char getch();
void clrscr(void);
#endif

int select_col_net(struct game *game, struct net_dat *net_dat);

int select_col(struct game *game);


/* selected col is 0 indexed
 * if selected col is < 0 the selector isn't rendered */
void render_game(struct game *game, int selected_col);

#endif

