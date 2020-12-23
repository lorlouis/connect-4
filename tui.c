#include <stdio.h>
#include <unistd.h>

#include <termios.h>

#include "con4.h"
#include "tui.h"

static struct termios old, new;

static void initTermios(int echo) {
  tcgetattr(0, &old); /* grab old terminal i/o settings */
  new = old; /* make new settings same as old settings */
  new.c_lflag &= ~ICANON; /* disable buffered i/o */
  new.c_lflag &= echo ? ECHO : ~ECHO; /* set echo mode */
  tcsetattr(0, TCSANOW, &new); /* apply terminal io settings */
}

/* Restore old terminal i/o settings */
static void resetTermios(void) {
  tcsetattr(0, TCSANOW, &old);
}

void clear_scr(void) {
    fputs("\033[2J\033[1;1H", stdout);
    rewind(stdout);
    ftruncate(1,0);
}

/* grabs keyboard keypress */
char getch() {
  char ch;
  initTermios(0);
  ch = getchar();
  resetTermios();
  return ch;
}

int select_col(struct game *game, int gamer) {
    char c;
    int selected_col = 0;
    do {
        render_game(game, selected_col, gamer);
        c = getch();
        switch(c) {
            case 'h':
            case 'a':
                if(selected_col > 0) selected_col--;
                break;
            case 'l':
            case 'd':
                if(selected_col < ROW_WIDTH-1) selected_col++;
                break;
            case 'q':
                goto force_quit;
        }
    } while(c != '\n');
    return selected_col;
force_quit:
    return -1;
}

/* selected col is 0 indexed
 * if selected col is < 0 the selector isn't rendered */
void render_game(struct game *game, int selected_col, int gamer) {
    clear_scr();
    if(game > 0) {
        printf("Player%d\'s turn\n", gamer);
    }
    if(selected_col >= 0) {
        for(int i = 0; i < selected_col; i++) printf("   ");
        puts(" ╲╱");
    }

    for(int y = 0; y < COL_WIDTH; y++) {
        putchar('|');
        for(int x = 0; x < ROW_WIDTH; x++) {
            switch(game->field[x][y]) {
                case(0):
                    printf("%s", EMPTY_STR);
                    break;
                case(1):
                    printf("%s", PLAYER_X);
                    break;
                case(2):
                    printf("%s", PLAYER_O);
                    break;
            }
            putchar('|');
        }
        putchar('\n');
    }
}
