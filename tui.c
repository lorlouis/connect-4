#include <stdio.h>

#ifdef _WIN32
#include <io.h>
#include <conio.h>

#define clrscr() system("cls")

#else
#include <unistd.h>
#include <termios.h>

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

void clrscr(void) {
    printf("\e[1;1H\e[2J");
}

/* grabs keyboard keypress */
char getch() {
    char ch;
    initTermios(0);
    ch = getchar();
    resetTermios();
    return ch;
}

#endif

#include "con4.h"
#include "tui.h"



int select_col(struct game *game, int gamer) {
    char c;
    int selected_col = COL_WIDTH / 2;
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
                if(selected_col < COL_WIDTH-1) selected_col++;
                break;
            case 'q':
                goto force_quit;
        }
#ifdef OS_WIN
    } while (c != 0x0D);
#else
    } while (c != '\n');
#endif
    return selected_col;
force_quit:
    return -1;
}

/* selected col is 0 indexed
 * if selected col is < 0 the selector isn't rendered */
void render_game(struct game *game, int selected_col, int gamer) {
    clrscr();
    if(gamer > 0) {
        printf("Player%d\'s turn\n", gamer);
    }
    if(selected_col >= 0) {
        for(int i = 0; i < selected_col; i++) printf("   ");
        puts(" \\/");
    }

    for(int y = 0; y < ROW_WIDTH; y++) {
        putchar('|');
        for(int x = 0; x < COL_WIDTH; x++) {
            switch(game->field[y][x]) {
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
